// Ce programme a pour but de tester les différentes fonctions de NOYAUFIL.C
// de la partie 1 de "Réalisation d’un mini noyau temps réel ARM"

#include "noyau.h"
#include "serialio.h"

int main(int argc, char **argv) {
  serial_init(115200);

  puts("Appel à file_init");
  file_init();

  ajoute(3);
  ajoute(5);
  ajoute(1);
  ajoute(0);
  ajoute(2);

  affic_file();
  puts("");
  affic_queue();
  puts("");

  uint16_t tache_suivante = suivant();
  printf("Tâche suivante:\t%d\n", tache_suivante);
  puts("");

  affic_file();
  puts("");
  affic_queue();
  puts("");

  retire(0);

  affic_file();
  puts("");
  affic_queue();
  puts("");

  ajoute(6);

  affic_file();
  puts("");
  affic_queue();
  puts("");

  return 0;
}
