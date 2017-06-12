/* PRODCONS_PIPE.C */
/*--------------------------------------------------------------------------*
 *			      Programme de tests							      	    *
 *--------------------------------------------------------------------------*/

#include "serialio.h"
#include "noyau.h"
#include "pipe.h"

/*
 ** Test du noyau preemptif. Lier noyautes.c avec noyau.c et noyaufil.c
 */

/* Création de la file du producteur/consommateur */
unsigned pipe;

/* Création des tâches */
TACHE launch(void);
TACHE Producteur(void);
TACHE Consommateur(void);

uint16_t Prod;
uint16_t Cons;

TACHE launch(void) {
  puts("------> EXEC tache launch");
  p_init();

  Prod = cree(Producteur);
  Cons = cree(Consommateur);
  pipe = p_open(Prod, Cons);
  active(Prod);
  active(Cons);
  fin_tache();
}

TACHE Producteur(void) {
  puts("------> DEBUT tache Producteur");
  char a = '1';
  while(1) {
    p_write(pipe, &a, 1);
    puts("-- Prod -- Production");
  }
  fin_tache();
}

TACHE Consommateur(void) {
  puts("------> DEBUT tache Consommateur");
  char a = ' ';
  while(1) {
    p_read(pipe, &a, 1);
    puts("-- Cons -- Consommation");
  }
  fin_tache();
}

int main() {
  serial_init(115200);
  puts("Test noyau");
  puts("Noyau preemptif");
  start(launch);
  getchar();
  return (0);
}

