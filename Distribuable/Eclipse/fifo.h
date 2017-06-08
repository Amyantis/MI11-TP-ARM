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

typedef struct FIFO
{
  int file[MAX_FIFO];
  int head;
  int tail;
  int size;
} FIFO;

void init_fifo(FIFO* f);
int push_fifo(FIFO* f, int v);
int pop_fifo(FIFO* f);
int top_fifo(FIFO* f);	
int size_fifo(FIFO* f);
int empty_fifo(FIFO* f);

#endif