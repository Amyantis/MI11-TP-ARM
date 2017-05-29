/* NOYAUTEST.C */
/*--------------------------------------------------------------------------*
 *			      Programme de tests			    *
 *--------------------------------------------------------------------------*/

#include "serialio.h"
#include "noyau.h"

/*
 ** Test du noyau preemptif. Lier noyautes.c avec noyau.c et noyaufil.c
 */

/* Création de la file du producteur/consommateur */

#define MAX_FILE  6

typedef struct FIFO
{
  int file[MAX_FILE];
  int head;
  int tail;
  int size;
} FIFO;
FIFO fifo;

/* Création des tâches */
TACHE launch(void);
TACHE Producteur(void);
TACHE Consommateur(void);

uint16_t Prod;
uint16_t Cons;

TACHE launch(void) {
  puts("------> EXEC tache launch");
  fifo.head = 0;
  fifo.tail = 0;
  fifo.size = 0;
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
    fifo.file[fifo.tail] = 1;
    fifo.tail = (fifo.tail + 1) % MAX_FILE;
    fifo.size = fifo.size + 1;

    if (fifo.size == MAX_FILE)
      dort();
    if (fifo.size == 1)
      reveille(Cons);
  }
  fin_tache();
}

TACHE Consommateur(void) {
  int i = 0;
  long j;
  puts("------> DEBUT tache Consommateur");
  while(1) {
    fifo.file[fifo.head] = 0;
    fifo.head = (fifo.head + 1) % MAX_FILE;
    fifo.size = fifo.size - 1;

    if(fifo.size == 0)
      dort();
    if(fifo.size == MAX_FILE - 1)
      reveille(Prod);
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

