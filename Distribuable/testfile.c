// Ce programme a pour but de tester les différentes fonctions de NOYAUFIL.C
// de la partie 1 de "Réalisation d’un mini noyau temps réel ARM"

#include <noyau.h>
#include <serialio.h>

int main(int argc, char **argv) {
  file_init();

  ajoute(3);
  ajoute(5);
  ajoute(1);
  ajoute(0);
  ajoute(2);

  affic_file();
  affic_queue();

  uint16_t tache_suivante = suivant();
  printf("Tâche suivante:\t%d\n", tache_suivante);

  affic_file();
  affic_queue();

  retire(0);

  affic_file();
  affic_queue();

  ajoute(6);

  affic_file();
  affic_queue();

  return 0;
}