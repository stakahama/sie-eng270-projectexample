import sys
from pathlib import Path
import pandas as pd
from pylib import simulate_grid

def stripkm(string):
    return string.replace(' km', '')

def capteurs_numeric(sensors):
    numeric = sensors.copy()
    for col in ['X', 'Y']:
        numeric[col] = sensors[col].apply(stripkm).astype(float) * 1e3
    return numeric

if __name__ == '__main__':

    ROOT = Path(sys.path[0]).parent

    sensors = pd.read_csv(ROOT.joinpath('data', 'capteur.csv'), sep='\t')
    outfile = ROOT.joinpath('results', 'plausibilite.csv')
    
    xycoord = capteurs_numeric(sensors)
    
    simulate_grid(xycoord, outfile)
