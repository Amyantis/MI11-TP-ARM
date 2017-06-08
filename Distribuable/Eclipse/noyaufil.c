/* NOYAUFILE.C */
/*--------------------------------------------------------------------------*
 *  gestion de la file d'attente des taches pretes et actives               *
 *  la file est rangee dans un tableau. ce fichier decrit toutes            *
 *  les primitives de base                                                  *
 *--------------------------------------------------------------------------*/

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
  printf("ajoute(%d)\n", n);

  if (_queue != F_VIDE && _file[n] != F_VIDE) {
	printf("Error: Tâche déjà existante.\n");
	return;
  }
  if (_queue == F_VIDE) {
	  _file[n] = n;
  } else {
	  _file[n] = _file[_queue];
	  _file[_queue] = n;
  }
  _queue = n;
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
  printf("retire(%d)\n", t);
  if (_file[t] == F_VIDE) {
    printf("Error: Tâche inexistante.\n");
  }

  if(_queue == t) {
	  _queue = _file[t];
  }

  uint16_t pred_t = predecesseur(t);
  _file[pred_t] = _file[t];

  _file[t] = F_VIDE;
}

uint16_t predecesseur(uint16_t t) {
  uint16_t pred_t;
  for (int i = 0; i < MAX_TACHES; i++) {
    if (_file[i] == t) {
	  return i;
    }
  }
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
    printf("Error: Aucune tâche.");
    return F_VIDE;
  } else {
	uint16_t suivant = _file[_queue];
	_queue = suivant;
    return suivant;
  }
}

/*     affichage du dernier element     *
 *--------------------------------------*
entree : sans
sortie : sans
description : affiche la valeur de queue
*/

void affic_queue(void) {
  printf("Affichage de la queue:\n");

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
  printf("Affichage de la file:\n");

  char *format = "Indice:\t%d\tValeur:\t%d\n";
  for (int i = 0; i < MAX_TACHES; i++) {
    printf(format, i, _file[i]);
  }
}
