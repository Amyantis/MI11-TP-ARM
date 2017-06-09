/* PIPE.H */
/*--------------------------------------------------------------------------*
 *  librairie implémentant les pipes pour MC9328MXL                         *
 *		                                                                    *
 ****************************************************************************
 * On definit dans ce fichier toutes les constantes et les structures       *
 * necessaires au fonctionnement du noyau
 *--------------------------------------------------------------------------*/

#ifndef __PIPE_H__
#define __PIPE_H__

#define MAX_PIPES 8
#define SIZE_PIPE 8

typedef struct {
	ushort pr_w , pr_r ;		/* redacteur & lecteur du tube */
	ushort sleep_w, sleep_r;	/* indicateur pour savoir si dort à cause du pipe */
	ushort ocupp;				/* donnees restantes */
	int size;					/* donnes dans le pipe */
	uchar is, ie;				/* pointeurs d’entree / sortie */
	uchar tube[SIZE_PIPE];		/* Tampon */
} PIPE;

PIPE _pipe[MAX_PIPES] ; 		/* Variables tubes */

void p_init();
unsigned p_open(unsigned redacteur, unsigned lecteur);
void p_close (unsigned conduit);
void p_read (unsigned conduit, uchar* donnees, unsigned nb);
void p_write(unsigned conduit, uchar* donnees, unsigned nb);

#endif
