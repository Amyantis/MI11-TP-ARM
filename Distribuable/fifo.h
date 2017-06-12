/* FIFO.H */
/*--------------------------------------------------------------------------*
 *  librairie implémentant les FIFO pour MC9328MXL                          *
 *		                                                                    *
 ****************************************************************************
 * On definit dans ce fichier toutes les constantes et les structures       *
 * necessaires au fonctionnement du noyau
 *--------------------------------------------------------------------------*/

#ifndef __FIFO_H__
#define __FIFO_H__

#define MAX_FIFO 8

// Structure de la FIFO circulaire d'entiers
typedef struct FIFO
{
  int file[MAX_FIFO];		// file circulaire
  int head;					// pointeur de tête
  int tail;					// pointeur de queue
  int size;					// taille de la file
} FIFO;

// Initialise les FIFO
void init_fifo(FIFO* f);

// Ajoute un élément dans la FIFO
int push_fifo(FIFO* f, int v);

// Retire un élément de la FIFO
int pop_fifo(FIFO* f);

// Accesseur à l'élément courant de la FIFO
int top_fifo(FIFO* f);

// Taille de la FIFO
int size_fifo(FIFO* f);

// La FIFO est-elle vide ?
int empty_fifo(FIFO* f);

#endif