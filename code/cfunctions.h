struct Capteur {
  double x;
  double y;
  double observation;
  int paquets;
};

double randomNumber(double max);

double plausibilite(double detectionsSimulees, double detectionsObservees);

void simulePaquet(struct Capteur * capteurs, double x, double y, double r);

double simulePaquets(struct Capteur* capteurs, double x, double y, double r, int iterations);
  
