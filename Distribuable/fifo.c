/* FIFO.C */
/*--------------------------------------------------------------------------*
 *                      Implémentation des FIFO en C                        *
 *                                 FIFO.C                                   *
 *--------------------------------------------------------------------------*/

#include "fifo.h"

// Initialise les FIFO
void init_fifo(FIFO* f)
{
	f->head = 0;
	f->tail = 0;
	f->size = 0;
}

// Ajoute un élément dans la FIFO
int push_fifo(FIFO* f, int v)
{
	if(size_fifo(f) == MAX_FIFO)
		return 0;

	f->file[f->tail] = v;
    f->tail = (f->tail + 1) % MAX_FIFO;
    f->size = f->size + 1;
    return 1;
}

// Retire un élément de la FIFO
int pop_fifo(FIFO* f)
{
	if(empty_fifo(f))
		return 0;

    f->head = (f->head + 1) % MAX_FIFO;
    f->size = f->size - 1;
    return 1;
}

// Accesseur à l'élément courant de la FIFO
int top_fifo(FIFO* f)
{
    return f->file[f->head];
}

// Taille de la FIFO
int size_fifo(FIFO* f)
{
	return f->size;
}

// La FIFO est-elle vide ?
int empty_fifo(FIFO* f)
{
	return f->size == 0;
}