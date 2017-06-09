/* NOYAUTEST.C */
/*--------------------------------------------------------------------------*
 *			      Programme de tests			    *
 *--------------------------------------------------------------------------*/

#include "serialio.h"
#include "noyau.h"
#include "fifo.h"

/*
 ** Test du noyau preemptif. Lier noyautes.c avec noyau.c et noyaufil.c
 */

/* Création de la file du producteur/consommateur */
FIFO fifo;

/* Création des tâches */
TACHE launch(void);
TACHE Producteur(void);
TACHE Consommateur(void);

uint16_t Prod;
uint16_t Cons;

TACHE launch(void) {
  puts("------> EXEC tache launch");
  init_fifo(&fifo);

  Prod = cree(Producteur);
  Cons = cree(Consommateur);
  active(Prod);
  active(Cons);
  fin_tache();
}

TACHE Producteur(void) {
  int i = 0;
  long j;
  puts("------> DEBUT tache Producteur");
  while(1) {
    push_fifo(&fifo, 1);
    puts("-- Prod -- Production");
    if (size_fifo(&fifo) == MAX_FIFO)
    {
        puts("-- Prod -- Sleep");
     	dort();
    }
    if (size_fifo(&fifo) >= 1)
    {
        puts("-- Prod -- Reveille Cons");
    	reveille(Cons);
    }
  }
  fin_tache();
}

TACHE Consommateur(void) {
  int i = 0;
  long j;
  puts("------> DEBUT tache Consommateur");
  while(1) {
    pop_fifo(&fifo);
    puts("-- Cons -- Consommation");
    if(size_fifo(&fifo) == 0)
    {
      puts("-- Cons -- Sleep");
      dort();
    }
    if(size_fifo(&fifo) <= MAX_FIFO - 1)
    {
      puts("-- Cons -- Reveille Prod");
      reveille(Prod);
    }
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

