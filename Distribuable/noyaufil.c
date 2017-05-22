/* NOYAUFILE.C */
/*--------------------------------------------------------------------------*
 *  gestion de la file d'attente des taches pretes et actives               *
 *  la file est rangee dans un tableau. ce fichier decrit toutes            *
 *  les primitives de base                                                  *
 *--------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "serialio.h"
#include "noyau.h"

/* variables communes a toutes les procedures *
 *--------------------------------------------*/

static uint16_t _file[MAX_TACHES];   /* indice=numero de tache */
/* valeur=tache suivante  */
static uint16_t _queue;              /* valeur de la derniere tache */
/* pointe la prochaine tache a activer */

/*     initialisation de la file      *
 *------------------------------------*
entre  : sans
sortie : sans
description : la queue est initialisee vide, queue prend la valeur de tache
	      impossible
*/

void file_init(void) {
  _queue = F_VIDE;
  for (int i = 0; i < MAX_TACHES; i++) {
    _file[i] = F_VIDE;
  }
}

/*        ajouter une tache dans la pile      *
 *--------------------------------------------*
entree : n numero de la tache a entrer
sortie : sans
description : ajoute la tache n en fin de pile
*/

void ajoute(uint16_t n) {
  if (_queue == F_VIDE) {
    _file[n] = n;
  }

  if (_file[n] == F_VIDE) {
    _file[n] = suivant();
    _file[_queue] = n;
  } else {
    fprintf(stderr, "Tâche déjà existante.");
  }

}

uint16_t predecesseur(uint16_t t);
/*           retire une tache de la file        *
 *----------------------------------------------*
entree : t numero de la tache a sortir
sortie : sans
description: sort la tache t de la file. L'ordre de la file n'est pas
	     modifie
*/

void retire(uint16_t t) {
  if (_file[t] == F_VIDE) {
    fprintf(stderr, "Tâche inexistante.");
  }

  uint16_t pred_t = predecesseur(t);
  _file[pred_t] = _file[t];

  _file[t] = F_VIDE;
}

uint16_t predecesseur(uint16_t t) {
  uint16_t pred_t;
  for (int i = 0; i < MAX_TACHES; i++) {
    if (_file[i] == t) {
      pred_t = i;
      break;
    }
  }
  return pred_t;
}

/*        recherche du suivant a executer       *
 *----------------------------------------------*
entree : sans
sortie : t numero de la tache a activer
description : la tache a activer est sortie de la file. queue pointe la
	      suivante
*/
uint16_t suivant(void) {
  if (_queue == F_VIDE) {
    fprintf(stderr, "Aucune tâche.");
    return F_VIDE;
  } else {
    return _file[_queue];
  }
}

/*     affichage du dernier element     *
 *--------------------------------------*
entree : sans
sortie : sans
description : affiche la valeur de queue
*/

void affic_queue(void) {
  char *format = "Queue:\t%d\tValeur:\t%d\n";
  printf(format, _queue, _file[_queue]);
}

/*     affichage de la file     *
 *------------------------------*
entree : sans
sortie : sans
description : affiche les valeurs de la file
*/

void affic_file(void) {
  char *format = "Indice:\t%d\tValeur:\t%d\n";
  for (int i = 0; i < MAX_TACHES; i++) {
    printf(format, i, _file[i]);
  }
}
