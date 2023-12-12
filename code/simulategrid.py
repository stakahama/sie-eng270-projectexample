import sys
from pathlib import Path
import pandas as pd
import json
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

    outfile = ROOT.joinpath('results', 'plausibilite.csv')

    ## sensors    
    sensors = pd.read_csv(ROOT.joinpath('data', 'capteurs.csv'), sep='\t')
    xycoord = capteurs_numeric(sensors)

    ## seed value
    with open(ROOT.joinpath('data', 'seedvalues.json')) as f:
        seeds = json.load(f)

    ## grid
    ## r0, rmax, and dr are tuples for (x, y) dimensions
    grille = grille2struct(r0 = (100, 4900),
                           rmax = (8000, 0),
                           dr = (200, -200))
    r = 10
    iterations = 100

    ## simulate
    simulate_grid(xycoord, grille, r, iterations, seeds['grid'], outfile)
