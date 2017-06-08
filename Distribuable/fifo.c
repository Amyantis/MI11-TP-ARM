/* FIFO.C */
/*--------------------------------------------------------------------------*
 *                      Implémentation des FIFO en C                        *
 *                                 FIFO.C                                   *
 *--------------------------------------------------------------------------*/

#include "fifo.h"

void init_fifo(FIFO* f)
{
	f->head = 0;
	f->tail = 0;
	f->size = 0;
}

int push_fifo(FIFO* f, int v)
{
	if(size_fifo(f) == MAX_FIFO)
		return 0;

	f->file[f->tail] = v;
    f->tail = (f->tail + 1) % MAX_FIFO;
    f->size = f->size + 1;
    return 1;
}

int pop_fifo(FIFO* f)
{
	if(empty_fifo(f))
		return 0;

    f->head = (f->head + 1) % MAX_FILE;
    f->size = f->size - 1;
    return 1;
}

int top_fifo(FIFO* f)
{
    return f->file[f->head];
}

int size_fifo(FIFO* f)
{
	return f->size;
}

int empty_fifo(FIFO* f)
{
	return f->size == 0;
}