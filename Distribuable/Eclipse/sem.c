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
		_sem[i].ocupp = 0;
	}
}

short s_cree(short v)
{
	short i = 0;
	while(i < MAX_SEM && _sem[i].ocupp == 1) ++i;
	if(i < MAX_SEM)
	{
		init_fifo(&_sem[i].file);
		_sem[i].valeur = v;
		_sem[i].ocupp = 1;
	}
	return i;
}

void s_close(short n)
{
	if(_sem[n].ocupp == 1)
	{
		_sem[n].ocupp = 0;
	}
}

void s_wait(short n)
{
	_lock_();
	if(_sem[n].valeur <= 0)
	{
		push_fifo(&_sem[n].file, _tache_c);
		dort();
	}
	else
	{
		_sem[n].valeur = _sem[n].valeur - 1;
	}
	_unlock_();
}

void s_signal(short n)
{
	_lock_();
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
	_unlock_();
}
