#include <stdio.h>
#include <stdlib.h>
#include "cfunctions.h"

void print_sensor_pos(int len, struct Capteur* capteurs) {
    
  for (int i = 0; i < 6; i++) {
    printf("S%d x=%.1f y=%.1f observation=%.1f paquest=%d\n",
	   i,
	   capteurs[i].x,
	   capteurs[i].y,
	   capteurs[i].observation,
	   capteurs[i].paquets);
  }
    
}

double simpoint(int len, struct Capteur* capteurs, double x, double y, double r, int iterations, int seed) {
  srandom(seed);
  double p = simulePaquets(capteurs, x, y, r, iterations);
  return p;
}

void simgrid(int len, struct Capteur* capteurs, struct Grille grille, double r, int iterations, int seed, const char* fname) {
  srandom(seed);

  // E. Toute la grille
  FILE * file = fopen(fname, "w");
  for (int j = 0; j < grille.n[1]; j++) {
    for (int i = 0; i < grille.n[0]; i++) {
      // C. Simuler
      double x = grille.r0[0] + i * grille.dr[0];
      double y = grille.r0[1] + j * grille.dr[1];
      double p = simulePaquets(capteurs, x, y, r, iterations);
      // F. Ecrire un fichier CSV
      if (x > 0) fprintf(file, ", ");
      fprintf(file, "%.5f", p);
    }
    fprintf(file, "\n");
  }
  fclose(file);
    
}
