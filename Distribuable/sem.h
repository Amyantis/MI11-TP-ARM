/* SEM.H */
/*--------------------------------------------------------------------------*
 *  librairie implémentant les sémaphores pour MC9328MXL                    *
 *		                                                                    *
 ****************************************************************************
 * On definit dans ce fichier toutes les constantes et les structures       *
 * necessaires au fonctionnement du noyau
 *--------------------------------------------------------------------------*/

#ifndef __SEM_H__
#define __SEM_H__

#include <stdint.h>
#include "fifo.h"

#define MAX_SEM 8

// Structure du sémaphore
typedef struct {
	FIFO file;			/* File circulaire des tâches en attente */
	short valeur ;		/* compteur du sémaphore e(s) */
	short ocupp;		/* si le semaphore est libre */
} SEMAPHORE;

// Tableau de sémaphores
SEMAPHORE _sem[MAX_SEM];

// Initialise les sémaphores
void s_init();

// Crée un sémaphore avec la valeur v
short s_cree(short v);

// Ferme un le sémaphore n
void s_close(short n);

// Requête d'accès au sémaphore
void s_wait(short n);

// Libération du sémaphore
void s_signal(short n);

#endif
