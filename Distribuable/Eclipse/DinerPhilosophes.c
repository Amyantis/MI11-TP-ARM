#include "imx_serial.h"
#include "serialio.h"
#include "noyau.h"
#include "sem.h"

#define NB_PHILOSOPHES = 5

void comportement_philosophe(int id_philosophe);

TACHE tache_philosophe0(void);
TACHE tache_philosophe1(void);
TACHE tache_philosophe2(void);
TACHE tache_philosophe3(void);
TACHE tache_philosophe4(void);

typedef struct {
  short mutex; // index du sémaphore
} fourchette;

typedef struct {
  fourchette *main_droite;
  fourchette *main_gauche;
} philosophe;

philosophe *philosophes[NB_PHILOSOPHES];
fourchette *fourchettes[NB_PHILOSOPHES];

TACHE launch(void) {
  puts("------> EXEC tache launch");

  s_init();

  int i;
  for (i = 0; i < NB_PHILOSOPHES; i++) {
    fourchettes[i].mutex = s_cree(1);
  }

  active(cree(tache_philosophe0));
  active(cree(tache_philosophe1));
  active(cree(tache_philosophe2));
  active(cree(tache_philosophe3));
  active(cree(tache_philosophe4));

  fin_tache();
}

TACHE tache_philosophe_0(void) {
  comportement_philosophe(0);
  fin_tache();
}

TACHE tache_philosophe_1(void) {
  comportement_philosophe(1);
  fin_tache();
}

TACHE tache_philosophe_2(void) {
  comportement_philosophe(2);
  fin_tache();
}

TACHE tache_philosophe_3(void) {
  comportement_philosophe(3);
  fin_tache();
}

TACHE tache_philosophe_4(void) {
  comportement_philosophe(4);
  fin_tache();
}

void comportement_philosophe(int id_philosophe) {
  printf("------> EXEC tache philosophe_%d\n", id_philosophe);

  int nb_repas_necessaire = 30;

  fourchette *fourchette_gauche = &fourchettes[id_philosophe];
  fourchette *fourchette_droite = 0;

  philosophe* moi = &philosophes[id_philosophe];

  philosophe* philosophe_gauche =
      id_philosophe == 0 ? NB_PHILOSOPHES : id_philosophe - 1;
  philosophe* philosophe_droit =
      id_philosophe == NB_PHILOSOPHES - 1 ? 0 : id_philosophe + 1;

  int j;
  while (nb_repas_necessaire > 0) {
    nb_repas_necessaire--;

    printf("======> Philosophe %d pense.\n", id_philosophe);
    for (j = 0; j < 30000L; j++);

    printf("======> Philosophe %d est affame.\n", id_philosophe);
    if(fourchette_gauche == 0) {
      s_wait(philosophe_gauche->main_droite->mutex);
    }
    if(fourchette_droite == 0) {
      s_wait(philosophe_droit->main_gauche->mutex);
    }

    printf("======> Philosophe %d mange.\n", id_philosophe);
    for (j = 0; j < 30000L; j++);
    s_signal(fourchette_gauche->mutex);
    s_signal(fourchette_droite->mutex);
  }

  printf("======> Philosophe %d ne mangera plus.\n", id_philosophe);
}


void close_semaphores() {
  puts("Suppression de tous les semaphores.\n");

  int i;
  for (i = 0; i < NB_PHILOSOPHES; i++) {
    s_close(fourchettes[i].mutex);
  }
}

int main() {
  serial_init(115200);
  puts("Début du diner des philosophes.\n");

  start(launch);

  close_semaphores();

  puts("Fin du diner des philosophes.\n");

  return 0;
}