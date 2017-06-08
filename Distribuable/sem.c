/* SEM.C */
/*--------------------------------------------------------------------------*
 *                      Implémentation des SEM en C                         *
 *                                 SEM.C                                    *
 *--------------------------------------------------------------------------*/

#include "sem.h"
#include "noyau.h"

void s_init()
{
	for (int i = 0; i < MAX_SEM; ++i)
	{
		_sem_libre[i] == 1;
	}
}

ushort s_cree(short v)
{
	ushort i = 0;
	while(i < MAX_SEM || _sem_libre[i] == 0) ++i;
	if(i < MAX_SEM)
	{
		init_fifo(&_sem[i].file);
		_sem[i].valeur = v;
		_sem_libre[i] = 0;
	}
	return i;
}

void s_close(ushort n)
{
	_sem_libre[n] = 1;
}

void s_wait(ushort n)
{
	if(_sem[n].valeur <= 0)
	{
		push_fifo(&_sem[n].file, _tache_c);
		dort();
	}
	else
	{
		_sem[n].valeur = _sem[n].valeur - 1;
	}
}

void s_signal(ushort n)
{
	int i, wokeup = 0;
	for (i = 0; i < MAX_FIFO; ++i)
	{
		if (empty_fifo(&_sem[n].file) != 1)
		{
			wokeup = 1;
			int t = top_fifo(&_sem[n].file);
			pop_fifo(&_sem[n].file);
			reveille(t);
		}
	}
	if(wokeup == 0) _sem[n].valeur = _sem[n].valeur + 1;
}
