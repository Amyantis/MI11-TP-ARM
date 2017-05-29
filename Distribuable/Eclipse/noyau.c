/* NOYAU.C */
/*--------------------------------------------------------------------------*
 *               Code C du noyau preemptif qui tourne sur ARM                *
 *                                 NOYAU.C                                  *
 *--------------------------------------------------------------------------*/

#include <stdint.h>

#include "serialio.h"
#include "imx_timers.h"
#include "imx_aitc.h"
#include "noyau.h"

/*--------------------------------------------------------------------------*
 *            Variables internes du noyau                                   *
 *--------------------------------------------------------------------------*/
static int compteurs[MAX_TACHES]; /* Compteurs d'activations */
CONTEXTE _contexte[MAX_TACHES];   /* tableau des contextes */
volatile uint16_t _tache_c;       /* num?ro de tache courante */
uint32_t _tos;                   /* adresse du sommet de pile */
int _ack_timer = 1;              /* = 1 si il faut acquitter le timer */

/*--------------------------------------------------------------------------*
 *                        Fin de l'execution                                *
 *----------------------------------------------------------------------- --*/
void noyau_exit(void) {
    int j;
    _irq_disable_();                /* D?sactiver les interruptions */
    printf("Sortie du noyau\n");
    for (j = 0; j < MAX_TACHES; j++)
        printf("\nActivations tache %d : %d", j, compteurs[j]);
    for (;;);                        /* Terminer l'ex?cution */
}

/*--------------------------------------------------------------------------*
 *                        --- Fin d'une tache ---                           *
 * Entree : Neant                                                           *
 * Sortie : Neant                                                           *
 * Descrip: Cette proc. doit etre appelee a la fin des taches               *
 *                                                                          *
 *----------------------------------------------------------------------- --*/
void fin_tache(void) {
    /* on interdit les interruptions */
    _irq_disable_();
    /* la tache est enlevee de la file des taches */
    _contexte[_tache_c].status = CREE;
    retire(_tache_c);
    schedule();
}

/*--------------------------------------------------------------------------*
 *                        --- Creer une tache ---                           *
 * Entree : Adresse de la tache                                             *
 * Sortie : Numero de la tache creee                                        *
 * Descrip: Cette procedure cree une tache en lui allouant                  *
 *    - une pile                                                            *
 *    - un numero                                                           *
 * Err. fatale: priorite erronnee, depassement du nb. maximal de taches     *
 *	                                                                        *
 *--------------------------------------------------------------------------*/
uint16_t cree(TACHE_ADR adr_tache) {
    /* pointeur d'une case de _contexte */
    CONTEXTE *p;
    /* contient numero dernier cree */
    static uint16_t tache = -1;


    /* debut section critique */
    _lock_();
    /* numero de tache suivant */
    tache++;

    /* sortie si depassement */
    if (tache >= MAX_TACHES)
        noyau_exit();

    /* contexte de la nouvelle tache */
    p = &_contexte[tache];

    /* allocation d'une pile a la tache */
    p->sp_ini = _tos;
    /* decrementation du pointeur de pile pour*/
    /* la prochaine tache. */
    _tos -= PILE_TACHE + PILE_IRQ;


    /* fin section critique */
    _unlock_();

    /* memorisation adresse debut de tache */
    p->tache_adr = adr_tache;
    /* mise a l'etat CREE */
    p->status = CREE;
    /* tache est un uint16_t */
    return (tache);
}

/*--------------------------------------------------------------------------*
 *                          --- Elire une tache ---                         *
 * Entree : Numero de la tache                                              *
 * Sortie : Neant                                                           *
 * Descrip: Cette procedure place une tache dans la file d'attente des      *
 *	    taches eligibles.                                                   *
 *	    Les activations ne sont pas memorisee                               *
 * Err. fatale: Tache inconnue	                                            *
 *                                                                          *
 *--------------------------------------------------------------------------*/
void active(uint16_t tache) {
    /* acces au contexte tache */
    CONTEXTE *p = &_contexte[tache];

    if (p->status == NCREE)
        /* sortie du noyau */
        noyau_exit();

    /* debut section critique */
    _lock_();
    /* n'active que si receptif */
    if (p->status == CREE) {
        /* changement d'etat, mise a l'etat PRET */
        p->status = PRET;
        /* ajouter la tache dans la liste */
        ajoute(tache);
        /* activation d'une tache prete */
        schedule();
    }
    /* fin section critique */
    _unlock_();
}

/*--------------------------------------------------------------------------*
 *                  ORDONNANCEUR preemptif optimise                         *
 *                                                                          *
 *             !! Cette fonction doit s'ex?cuter en mode IRQ !!             *
 *  !! Pas d'appel direct ! Utiliser schedule pour provoquer une            *
 *  commutation !!                                                          *
 *--------------------------------------------------------------------------*/
void __attribute__((naked)) scheduler(void) {
    register CONTEXTE *p;
    /* Pointeur de pile */
    register unsigned int sp asm("sp");

    /* Sauvegarder le contexte complet sur la pile IRQ */

    __asm__ __volatile__(
    /* Sauvegarde registres mode system */
            "stmfd  sp, {r0-r14}^\t\n"
            /* Attendre un cycle */
            "nop\t\n"
            /* Ajustement pointeur de pile */
            "sub    sp, sp, #60\t\n"
            /* Sauvegarde de spsr_irq */
            "mrs    r0, spsr\t\n"
            /* et de lr_irq */
            "stmfd  sp!, {r0, lr}\t\n");

    /* Réinitialiser le timer si n?cessaire */
    if (_ack_timer) {
        register struct imx_timer *tim1 = (struct imx_timer *) TIMER1_BASE;
        tim1->tstat &= ~TSTAT_COMP;
    } else {
        _ack_timer = 1;
    }

    /* memoriser le pointeur de pile */
    _contexte[_tache_c].sp_irq = sp;
    /* recherche du suivant */
    _tache_c = suivant();
    if (_tache_c == F_VIDE) {
        printf("Plus rien à ordonnancer.\n");
        /* Sortie du noyau */
        noyau_exit();
    }
    /* Incrémenter le compteur d'activations  */
    compteurs[_tache_c]++;
    /* p pointe sur la nouvelle tache courante*/
    p = &_contexte[_tache_c];

    /* tache prete ? */
    if (p->status == PRET) {
        /* Charger sp_irq initial */
        sp = p->sp_ini;
        /* Passer en mode syst?me */
        _set_arm_mode_(ARMMODE_SYS);
        /* Charger sp_sys initial */
        sp = p->sp_ini - PILE_IRQ;
        /* status tache -> execution */
        p->status = EXEC;
        /* autoriser les interuptions   */
        _irq_enable_();
        /* lancement de la tâche */
        (*p->tache_adr)();
    } else {
        /* tache deja en execution, restaurer sp_irq */
        sp = p->sp_irq;
    }

    /* Restaurer le contexte complet depuis la pile IRQ */

    __asm__ __volatile__(
    /* Restaurer lr_irq */
            "ldmfd  sp!, {r0, lr}\t\n"
            /* et spsr_irq */
            "msr    spsr, r0\t\n"
            /* Restaurer registres mode system */
            "ldmfd  sp, {r0-r14}^\t\n"
            /* Attendre un cycle */
            "nop\t\n"
            /* Ajuster pointeur de pile irq */
            "add    sp, sp, #60\t\n"
            /* Retour d'exception */
            "subs   pc, lr, #4\t\n");
}

/*--------------------------------------------------------------------------*
 *                  --- Provoquer une commutation ---                       *
 *                                                                          *
 *             !! Cette fonction doit s'ex?cuter en mode IRQ !!             *
 *  !! Pas d'appel direct ! Utiliser schedule pour provoquer une            *
 *  commutation !!                                                          *
 *--------------------------------------------------------------------------*/
void schedule(void) {
    /* Debut section critique */
    _lock_();

    /* On simule une exception irq pour forcer un appel correct à scheduler().*/

    _ack_timer = 0;
    /* Passer en mode IRQ */
    _set_arm_mode_(ARMMODE_IRQ);
    __asm__ __volatile__(
    /* Sauvegarder cpsr dans spsr */
    "mrs  r0, cpsr\t\n"
            "msr  spsr, r0\t\n"
            /* Sauvegarder pc dans lr et l'ajuster */
            "add  lr, pc, #4\t\n"
            /* Saut à scheduler */
            "b    scheduler\t\n"
    );
    /* Repasser en mode system */
    _set_arm_mode_(ARMMODE_SYS);

    /* Fin section critique */
    _unlock_();
}

/*--------------------------------------------------------------------------*
 *                        --- Lancer le systeme ---                         *
 * Entree : Adresse de la premiere tache a lancer                           *
 * Sortie : Neant                                                           *
 * Descrip: Active la tache et lance le systeme                             *
 *                                                                          *
 *                                                                          *
 * Err. fatale: Neant                                                       *
 *                                                                          *
 *--------------------------------------------------------------------------*/
void start(TACHE_ADR adr_tache) {
    short j;
    register unsigned int sp asm("sp");
    struct imx_timer *tim1 = (struct imx_timer *) TIMER1_BASE;
    struct imx_aitc *aitc = (struct imx_aitc *) AITC_BASE;

    for (j = 0; j < MAX_TACHES; j++) {
        /* initialisation de l'etat des taches */
        _contexte[j].status = NCREE;
    }
    /* initialisation de la tache courante */
    _tache_c = 0;
    /* initialisation de la file           */
    file_init();

    /* Haut de la pile des tâches */
    _tos = sp;
    /* Passer en mode IRQ */
    _set_arm_mode_(ARMMODE_IRQ);
    /* sp_irq initial */
    sp = _tos;
    /* Repasser en mode SYS */
    _set_arm_mode_(ARMMODE_SYS);

    /* on interdit les interruptions */
    _irq_disable_();

    /* Initialisation du timer à 100 Hz */

    tim1->tcmp = 10000;
    tim1->tprer = 0;
    tim1->tctl |= TCTL_TEN | TCTL_IRQEN | TCTL_CLKSOURCE_PERCLK16;

    /* Initialisation de l'AITC */

    aitc->intennum = TIMER1_INT;

    /* creation et activation premiere tache */
    active(cree(adr_tache));
}

/*-------------------------------------------------------------------------*
 *                    --- Endormir la t?che courante ---                   *
 * Entree : Neant                                                          *
 * Sortie : Neant                                                          *
 * Descrip: Endort la t?che courante et attribue le processeur ? la t?che  *
 *          suivante.                                                      *
 *                                                                         *
 * Err. fatale:Neant                                                       *
 *                                                                         *
 *-------------------------------------------------------------------------*/

void dort(void) {
    _contexte[_tache_c].status = SUSP;
    
    _lock_();
    
    retire(_tache_c);
    schedule();
    
    _unlock_();
}

/*-------------------------------------------------------------------------*
 *                    --- R?veille une t?che ---                           *
 * Entree : num?ro de la t?che ? r?veiller                                 *
 * Sortie : Neant                                                          *
 * Descrip: R?veille une t?che. Les signaux de r?veil ne sont pas m?moris?s*
 *                                                                         *
 * Err. fatale:t?che non cr??e                                             *
 *                                                                         *
 *-------------------------------------------------------------------------*/


void reveille(uint16_t t) {
    CONTEXTE *p = &_contexte[t];
    if (p->status == SUSP)
    {
        _lock_();

        p->status = EXEC;
        ajoute(t);
        schedule();

        _unlock_();
    }
}
