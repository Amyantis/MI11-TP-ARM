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
#define SIZE_PIPE 32

typedef struct {
	short pr_w , pr_r ;			/* redacteur & lecteur du tube */
	short sleep_w, sleep_r;		/* indicateur pour savoir si dort à cause du pipe */
	short ocupp;				/* donnees restantes */
	int size;					/* donnes dans le pipe */
	char is, ie;				/* pointeurs d’entree / sortie */
	char tube[SIZE_PIPE];		/* Tampon */
} PIPE;

PIPE _pipe[MAX_PIPES] ; 		/* Variables tubes */

void p_init();
unsigned p_open(unsigned redacteur, unsigned lecteur);
void p_close (unsigned conduit);
void p_read (unsigned conduit, char* donnees, unsigned nb);
void p_write(unsigned conduit, char* donnees, unsigned nb);

#endif
