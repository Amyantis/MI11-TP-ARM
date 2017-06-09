/* PIPE.C */
/*--------------------------------------------------------------------------*
 *                      Implémentation des PIPE en C                        *
 *                                 PIPE.C                                   *
 *--------------------------------------------------------------------------*/

#include "pipe.h"
#include "noyau.h"

void p_init()
{
	int i;
	for (i = 0; i < MAX_PIPES; ++i)
	{
		_pipe[i].ocupp = 0;
	}
}

unsigned p_open(unsigned redacteur, unsigned lecteur)
{
	// vérification que les tâches existent
	if (_contexte[redacteur].status == NCREE)
		return MAX_PIPES;
	if (_contexte[lecteur].status == NCREE)
		return MAX_PIPES;

	// vérification que les deux tâches ne partagent pas déjà un pipe
	int i;
	for (i = 0; i < MAX_PIPES; ++i)
	{
		if (_pipe[i].ocupp == 1)
		{
			if (_pipe[i].pr_w == redacteur && _pipe[i].pr_r == lecteur)
			{
				return MAX_PIPES;
			}
		}
	}

	// trouver un tube non initialisé
	int p = 0;
	while(_pipe[p].ocupp == 1 && p < MAX_PIPES) ++p;
	if(p == MAX_PIPES) return MAX_PIPES;

	__lock__();

	// Initialiser le tube
	_pipe[p].pr_w = redacteur;
	_pipe[p].pr_r = lecteur;
	_pipe[p].sleep_r = 0;
	_pipe[p].sleep_w = 0;
	_pipe[p].is = 0;
	_pipe[p].ie = 0;
	_pipe[p].size = 0;
	_pipe[p].ocupp = 1;

	__unlock__();

	// retour le numéro du pipe
	return p;
}

void p_close (unsigned conduit)
{
	if(_pipe[conduit].ocupp == 1)
	{
		__lock__();
		_pipe[conduit].ocupp = 0;
		__unlock__();
	}
}

void p_read (unsigned conduit, uchar* donnees, unsigned nb)
{
	if(_pipe[conduit].ocupp == 1 && _pipe[conduit].pr_r == _tache_c)
	{
		// copier les données
		int i;
		for(i = 0; i < nb; ++i)
		{
			if(_pipe[conduit].size == 0)
			{
				_pipe[conduit].sleep_r = 1;
				dort();
			}
			else
			{
				__lock__():
				donnees[i] = _pipe[conduit].tube[_pipe[conduit].is];
				_pipe[conduit].is = (_pipe[conduit].is + 1) % SIZE_PIPE;
				_pipe[conduit].size = _pipe[conduit].size - 1;
				__unlock__();
			}
			
			if(_pipe[conduit].size != SIZE_PIPE && _pipe[conduit].sleep_w == 1)
			{
				_pipe[conduit].sleep_w = 0;
				reveille(_pipe[conduit].pr_w);
			}
		}
	}
}

void p_write(unsigned conduit, uchar* donnees, unsigned nb)
{
	if(_pipe[conduit].ocupp == 1 && _pipe[conduit].pr_w == _tache_c)
	{
		// copier les données
		int i;
		for(i = 0; i < nb; ++i)
		{
			if(_pipe[conduit].size == SIZE_PIPE)
			{
				_pipe[conduit].sleep_w = 1;
				dort();
			}
			else
			{
				__lock__():
				_pipe[conduit].tube[_pipe[conduit].ie] = donnees[i];
				_pipe[conduit].ie = (_pipe[conduit].ie + 1) % SIZE_PIPE;
				_pipe[conduit].size = _pipe[conduit].size + 1;
				__unlock__();
			}

			if(_pipe[conduit].size != 0 && _pipe[conduit].sleep_r == 1)
			{
				_pipe[conduit].sleep_r = 0;
				reveille(_pipe[conduit].pr_r);
			}
		}
	}
}
