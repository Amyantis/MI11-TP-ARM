/* SEM.C */
/*--------------------------------------------------------------------------*
 *                      Implémentation des SEM en C                         *
 *                                 SEM.C                                    *
 *--------------------------------------------------------------------------*/

#include "sem.h"
#include "noyau.h"

// Initialise les sémaphores
void s_init()
{
	for (int i = 0; i < MAX_SEM; ++i)
	{
		_sem[i].ocupp = 0;
	}
}

// Crée un sémaphore avec la valeur v
short s_cree(short v)
{
	short i = 0;
	_lock_();
	while(i < MAX_SEM && _sem[i].ocupp == 1) ++i;
	if(i < MAX_SEM)
	{
		init_fifo(&_sem[i].file);
		_sem[i].valeur = v;
		_sem[i].ocupp = 1;
	}
	_unlock_();
	return i;
}

// Ferme un le sémaphore n
void s_close(short n)
{
	if(_sem[n].ocupp == 1)
	{
		_sem[n].ocupp = 0;
	}
}

// Requête d'accès au sémaphore
void s_wait(short n)
{
	_lock_();

	// Se bloque sur le sémaphore ou décrémente la valeur

	if(_sem[n].valeur <= 0)
	{
		push_fifo(&_sem[n].file, _tache_c);
		_unlock_();
		dort();
	}
	else
	{
		_sem[n].valeur = _sem[n].valeur - 1;
		_unlock_();
	}
}

// Libération du sémaphore
void s_signal(short n)
{
	_lock_();
	
	// Incrémente la valeur
	// Ou réveille une tâche bloquée

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
