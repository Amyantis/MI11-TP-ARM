/* PIPE.C */
/*--------------------------------------------------------------------------*
 *                      Implémentation des PIPE en C                        *
 *                                 PIPE.C                                   *
 *--------------------------------------------------------------------------*/

#include "pipe.h"
#include "noyau.h"

// Initialise les pipes
void p_init()
{
	int i;
	for (i = 0; i < MAX_PIPES; ++i)
	{
		_pipe[i].ocupp = 0;
	}
}

// Ouvre un pipe à deux tâches
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

	_lock_();

	// trouver un tube non initialisé
	int p = 0;
	while(_pipe[p].ocupp == 1 && p < MAX_PIPES) ++p;
	if(p < MAX_PIPES)
	{
		// Initialiser le tube
		_pipe[p].pr_w = redacteur;
		_pipe[p].pr_r = lecteur;
		_pipe[p].sleep_r = 0;
		_pipe[p].sleep_w = 0;
		_pipe[p].is = 0;
		_pipe[p].ie = 0;
		_pipe[p].size = 0;
		_pipe[p].ocupp = 1;
	}

	_unlock_();

	// retour le numéro du pipe
	return p;
}

// Ferme un pipe
void p_close (unsigned conduit)
{
	if(_pipe[conduit].ocupp == 1)
	{
		_pipe[conduit].ocupp = 0;
	}
}

// Lecture bloquante d'une donnée dans le pipe
void p_read (unsigned conduit, char* donnees, unsigned nb)
{
	// Si le pipe est initialisé et que la tâche courante est bien lecteur
	if(_pipe[conduit].ocupp == 1 && _pipe[conduit].pr_r == _tache_c)
	{
		// On traite les données une par une
		int i;
		for(i = 0; i < nb; ++i)
		{
			// Si il n'y a rien à lire
			if(_pipe[conduit].size == 0)
			{
				// Lecture bloquée
				_pipe[conduit].sleep_r = 1;
				dort();
			}
			
			// Lecture des données
			_lock_();
			donnees[i] = _pipe[conduit].tube[_pipe[conduit].is];
			_pipe[conduit].is = (_pipe[conduit].is + 1) % SIZE_PIPE;
			_pipe[conduit].size = _pipe[conduit].size - 1;
			_unlock_();

			// Si la tâche rédactrice dort et que le pipe n'est pas plein
			if(_pipe[conduit].size != SIZE_PIPE && _pipe[conduit].sleep_w == 1)
			{
				// Réveiller la tâche
				_pipe[conduit].sleep_w = 0;
				reveille(_pipe[conduit].pr_w);
			}
		}
	}
}

// Ecriture bloquante d'une donnée dans le pipe
void p_write(unsigned conduit, char* donnees, unsigned nb)
{
	// Si le pipe est initialisé et que la tâche courante est bien rédactrice
	if(_pipe[conduit].ocupp == 1 && _pipe[conduit].pr_w == _tache_c)
	{
		// On traite les données une par une
		int i;
		for(i = 0; i < nb; ++i)
		{
			// Si le pipe est plein
			if(_pipe[conduit].size == SIZE_PIPE)
			{
				// Ecriture bloquante
				_pipe[conduit].sleep_w = 1;
				dort();
			}

			// Ecriture des données
			_lock_();
			_pipe[conduit].tube[_pipe[conduit].ie] = donnees[i];
			_pipe[conduit].ie = (_pipe[conduit].ie + 1) % SIZE_PIPE;
			_pipe[conduit].size = _pipe[conduit].size + 1;
			_unlock_();

			// Si la tâche lectrice est endormie sur une lecture et que le pipe n'est pas vide
			if(_pipe[conduit].size != 0 && _pipe[conduit].sleep_r == 1)
			{
				// Réveiller la tâche lectrice
				_pipe[conduit].sleep_r = 0;
				reveille(_pipe[conduit].pr_r);
			}
		}
	}
}
