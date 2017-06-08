// /* PIPE.C */
// /*--------------------------------------------------------------------------*
//  *                      Implémentation des PIPE en C                        *
//  *                                 PIPE.C                                   *
//  *--------------------------------------------------------------------------*/

// #include "pipe.h"
// #include "noyau.h"

// void p_init()
// {
// 	int i;
// 	for (i = 0; i < MAX_PIPES; ++i)
// 	{
// 		_pipe_libre[i] = 1;
// 	}
// }

// unsigned p_open(unsigned redacteur, unsigned lecteur)
// {
// 	// vérification que les tâches existent
// 	if (_contexte[redacteur].status == NCREE)
// 		return MAX_PIPES;
// 	if (_contexte[lecteur].status == NCREE)
// 		return MAX_PIPES;

// 	// vérification que les deux tâches ne partagent pas déjà un pipe
// 	int i;
// 	for (i = 0; i < MAX_PIPES; ++i)
// 	{
// 		if (_pipe_libre[i] == 0)
// 		{
// 			if (_pipe[i].pr_w == redacteur && _pipe[i].pr_r == lecteur)
// 			{
// 				return MAX_PIPES;
// 			}
// 		}
// 	}

// 	// trouver un tube non initialisé
// 	int p = 0;
// 	while(_pipe_libre[p] == 0 && p < MAX_PIPES) ++p;
// 	if(p == MAX_PIPES) return MAX_PIPES;

// 	return p;
// }

// void p_close (unsigned conduit)
// {

// }

// void p_read (tube, donnees, quantité)
// {

// }

// void p_write(tube, donnees, quantité)
// {

// }
