#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "cfunctions.h"

double randomDomaine = RAND_MAX + 1.0;

double randomNumber(double max) {
  return random() / randomDomaine * max;
}

double plausibilite(double detectionsSimulees, double detectionsObservees) {
  double difference = detectionsSimulees - detectionsObservees;
  double variance = detectionsSimulees;
  if (variance < 0.1) variance = 0.1;
  return exp(-difference * difference / (2 * variance));
}

// A. Propagation d'un paquet (x, y, r)
void simulePaquet(struct Capteur * capteurs, double x, double y, double r) {
      
  for (int i = 0; i < 1000; i++) {
    // Vent
    double ventX = 10;
    double ventY = -0.001 * (x - 2000);

    // Aléa
    double ax = randomNumber(30) - 15;
    double ay = randomNumber(30) - 15;

    // Déplacer le paquet
    x += ventX + ax;
    y += ventY + ay;
    r += 20 / r;

    // C. Capteurs
    for (int s = 0; s < 6; s++) {
      double dx = x - capteurs[s].x;
      double dy = y - capteurs[s].y;
      double distance2 = dx * dx + dy * dy;
      if (distance2 < r * r) {
	capteurs[s].paquets += 1;
      }
    }

    // Paquet toujours dans la zone d'intérêt?
    if (x < 0) break;
    if (y < 0) break;
    if (x > 8000) break;
    if (y > 5000) break;
  }

  // Pour la verification d'un paquet
  // printf("%f %f %f\n", x, y, r);
}

double simulePaquets(struct Capteur* capteurs, double x, double y, double r, int iterations) {
  // Remettre les compteurs à 0
  for (int i = 0; i < 6; i++) {
    capteurs[i].paquets = 0;
  }

  // Simuler des paquets
  for (int i = 0; i < iterations; i++) {
    simulePaquet(capteurs, x, y, r);
  }

  // E. Calcul de la plausibilite
  double p = 1;
  for (int i = 0; i < 6; i++) {
    double paquetsParIteration = (double) capteurs[i].paquets / iterations;
    p *= plausibilite(paquetsParIteration, capteurs[i].observation);
  }

  return p;
}
