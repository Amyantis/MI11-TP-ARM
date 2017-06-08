/* SEM.H */
/*--------------------------------------------------------------------------*
 *  librairie implémentant les sémaphores pour MC9328MXL                    *
 *		                                                                    *
 ****************************************************************************
 * On definit dans ce fichier toutes les constantes et les structures       *
 * necessaires au fonctionnement du noyau
 *--------------------------------------------------------------------------*/

#include "fifo.h"

#define MAX_SEM 8

typedef struct {
	FIFO file;			/* File circulaire des tâches en attente */
	short valeur ;		/* compteur du sémaphore e(s) */
} SEMAPHORE;

SEMAPHORE _sem[MAX_SEM];
ushort _sem_libre[MAX_SEM];

void s_init();
ushort s_cree(short v);
void s_close(ushort n);
void s_wait(ushort n);
void s_signal(ushort n);