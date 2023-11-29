#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

double simpoint(int len, struct Capteur* capteurs) {
  srandom(time(NULL));
  double p = simulePaquets(capteurs, 2500, 3100, 10, 100);
  return p;
}

void simgrid(int len, struct Capteur* capteurs, const char* fname) {
  srandom(time(NULL));

  // E. Toute la grille
  FILE * file = fopen(fname, "w");
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 40; x++) {
      // C. Simuler
      double p = simulePaquets(capteurs, 100 + x * 200, 4900 - y * 200, 10, 100);

      // F. Ecrire un fichier CSV
      if (x > 0) fprintf(file, ", ");
      fprintf(file, "%.5f", p);
    }
    fprintf(file, "\n");
  }
  fclose(file);
    
}
