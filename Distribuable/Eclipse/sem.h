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

typedef struct {
	FIFO file;			/* File circulaire des tâches en attente */
	short valeur ;		/* compteur du sémaphore e(s) */
} SEMAPHORE;

SEMAPHORE _sem[MAX_SEM];
short _sem_libre[MAX_SEM];

void s_init();
short s_cree(short v);
void s_close(short n);
void s_wait(short n);
void s_signal(short n);

#endif