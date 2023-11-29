#! /usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

plausibilite = np.genfromtxt('plausibilite.csv', delimiter = ',')
x = np.arange(0, plausibilite.shape[1] + 1) * 0.2
y = 5 - np.arange(0, plausibilite.shape[0] + 1) * 0.2

plt.axes().set_aspect('equal')
plt.title('Plausibilité')
plt.pcolormesh(x, y, plausibilite)
plt.savefig('plausibilite.png')
