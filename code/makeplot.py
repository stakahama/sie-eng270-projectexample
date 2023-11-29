import sys
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt

ROOT = Path(sys.path[0]).parent
outfile = ROOT.joinpath('results', 'plausibilite.csv')

plausibilite = np.genfromtxt(outfile, delimiter = ',')
dx = 0.2
dy = 0.2
x = np.arange(0, plausibilite.shape[1] + 1) * dx
y = 5 - np.arange(0, plausibilite.shape[0] + 1) * dy

plt.axes().set_aspect('equal')
plt.title('Plausibilité')
plt.pcolormesh(x, y, plausibilite)
plt.xlabel('X coordinate [km]')
plt.ylabel('Y coordinate [km]')
# plt.show()
plt.savefig(ROOT.joinpath('results', 'plausibilite.png'))
