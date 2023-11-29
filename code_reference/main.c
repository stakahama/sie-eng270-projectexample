#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Toute la simulation est en mètres.

double randomDomaine = RAND_MAX + 1.0;

double randomNumber(double max) {
    return random() / randomDomaine * max;
}

// B. Capteurs
struct Capteur {
    double x;
    double y;
    double observation;
    int paquets;
};

struct Capteur capteurs[6];

// A. Propagation d'un paquet (x, y, r)
void simulePaquet(double x, double y, double r) {
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

double plausibilite(double detectionsSimulees, double detectionsObservees) {
    double difference = detectionsSimulees - detectionsObservees;
    double variance = detectionsSimulees;
    if (variance < 0.1) variance = 0.1;
    return exp(-difference * difference / (2 * variance));
}

double simulePaquets(double x, double y, double r, int iterations) {
    // Remettre les compteurs à 0
    for (int i = 0; i < 6; i++) {
        capteurs[i].paquets = 0;
    }

    // Simuler des paquets
    for (int i = 0; i < iterations; i++) {
        simulePaquet(x, y, r);
    }

    // E. Calcul de la plausibilite
    double p = 1;
    for (int i = 0; i < 6; i++) {
        double paquetsParIteration = (double) capteurs[i].paquets / iterations;
        p *= plausibilite(paquetsParIteration, capteurs[i].observation);
    }

    return p;
}

int main(int argc, char * argv[]) {
    srandom(time(NULL));

    // B. Capteurs
    capteurs[0].x = 7340;
    capteurs[0].y = 4020;
    capteurs[0].observation = 0;

    capteurs[1].x = 7500;
    capteurs[1].y = 2880;
    capteurs[1].observation = 0;

    capteurs[2].x = 7420;
    capteurs[2].y = 1910;
    capteurs[2].observation = 3.0;

    capteurs[3].x = 7250;
    capteurs[3].y = 1340;
    capteurs[3].observation = 0;

    capteurs[4].x = 7380;
    capteurs[4].y = 720;
    capteurs[4].observation = 0;

    capteurs[5].x = 6200;
    capteurs[5].y = 280;
    capteurs[5].observation = 0;

    // D. Un point
    double p = simulePaquets(2500, 3100, 10, 100);
    for (int i = 0; i < 6; i++) {
        printf("S%d  %d paquets\n", i, capteurs[i].paquets);
    }

    printf("Plausibilité: %0.5f\n", p);

    // E. Toute la grille
    FILE * file = fopen("plausibilite.csv", "w");
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 40; x++) {
            // C. Simuler
            double p = simulePaquets(100 + x * 200, 4900 - y * 200, 10, 100);

            // F. Ecrire un fichier CSV
            if (x > 0) fprintf(file, ", ");
            fprintf(file, "%.5f", p);
        }

        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}
