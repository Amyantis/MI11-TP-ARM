// /* PIPE.H */
// /*--------------------------------------------------------------------------*
//  *  librairie implémentant les pipes pour MC9328MXL                         *
//  *		                                                                    *
//  ****************************************************************************
//  * On definit dans ce fichier toutes les constantes et les structures       *
//  * necessaires au fonctionnement du noyau
//  *--------------------------------------------------------------------------*/

// #ifndef __PIPE_H__
// #define __PIPE_H__

// #define MAX_PIPES 8
// #define SIZE_PIPE 8

// typedef struct {
// 	ushort pr_w , pr_r ;		/* redacteur & lecteur du tube */
// 	ushort ocupp ;				/* donnees restantes */
// 	uchar is, ie ;				/* pointeurs d’entree / sortie */
// 	uchar tube[SIZE_PIPE] ;		/* Tampon */
// } PIPE;

// PIPE _pipe[MAX_PIPES] ; 		/* Variables tubes */
// short _pipe_libre[MAX_PIPES];	/* Pipes libres */

// void p_init();
// unsigned p_open(unsigned redacteur, unsigned lecteur);
// void p_close (unsigned conduit);
// void p_read (tube, donnees, quantité);
// void p_write(tube, donnees, quantité);

// #endif
