// Ce programme a pour but de tester les différentes fonctions de NOYAUFIL.C
// de la partie 1 de "Réalisation d’un mini noyau temps réel ARM"

#include <stdlib.h>
#include <noyau.h>

int main(int argc, char **argv) {
  file_init();

  TACHE_ADR adr_tache = NULL;
  uint16_t tache = cree(adr_tache);
  ajoute(tache);

  uint16_t tache_suivante = suivant();

  retire(tache);

  return EXIT_SUCCESS;
}