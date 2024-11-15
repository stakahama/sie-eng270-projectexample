import sys
from pathlib import Path
import pandas as pd
from pylib import simulate_grid, grille2struct

def stripkm(string):
    return string.replace(' km', '')

def capteurs_numeric(sensors):
    numeric = sensors.copy()
    for col in ['X', 'Y']:
        numeric[col] = sensors[col].apply(stripkm).astype(float) * 1e3
    return numeric

if __name__ == '__main__':

    ROOT = Path(sys.path[0]).parent

    inpfile = ROOT.joinpath('data', 'capteurs.csv')
    outfile = ROOT.joinpath('outputs', 'plausibilite.csv')

    ## sensors    
    sensors = pd.read_csv(inpfile, sep='\t')
    xycoord = capteurs_numeric(sensors)

    ## grid
    grille = grille2struct(r0 = (100, 4900),
                           rmax = (8000, 0),
                           dr = (200, -200))
    r = 10
    iterations = 100

    ## simulate
    simulate_grid(xycoord, grille, r, iterations, outfile)
