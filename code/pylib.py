import sys
from pathlib import Path
import ctypes
from math import floor

## -----------------------------------------------------------------------------

# Define ctypes structure
class Capteur(ctypes.Structure):
    _fields_ = [
        ('x', ctypes.c_double),
        ('y', ctypes.c_double),
        ('observation', ctypes.c_double),
        ('paquets', ctypes.c_int)
    ]

def capteur2struct(df):

    # Create a ctypes array of MyStruct
    ctypes_array = (Capteur * len(df))()

    # Fill in the data
    for i in range(len(df)):
        ctypes_array[i].x = df.loc[df.index[i], 'X']
        ctypes_array[i].y = df.loc[df.index[i], 'Y']
        ctypes_array[i].observation = 0
        ctypes_array[i].paquets = 0

    return ctypes_array
    
# Define ctypes structure
class Grille(ctypes.Structure):
    _fields_ = [
        ('r0', ctypes.c_double * 2),
        ('dr', ctypes.c_double * 2),
        ('n', ctypes.c_int * 2),
    ]
    
def grille2struct(r0, rmax, dr):

    npoints = list(map(lambda r0, dr, rmax: \
                       floor((rmax - r0) / dr), r0, dr, rmax))
    
    grille = Grille()
    for i in range(len(r0)):
        grille.r0[i] = r0[i]
        grille.dr[i] = dr[i]        
        grille.n[i] = npoints[i]

    return grille
    
## -----------------------------------------------------------------------------

# Load the shared library into ctypes
ROOT = Path(sys.path[0]).parent
dll = ctypes.CDLL(ROOT.joinpath('bin', 'clib.so'))

## -----------------------------------------------------------------------------

# Provide the argument types and return type of the function
dll.print_sensor_pos.argtypes = (ctypes.c_int, ctypes.POINTER(Capteur))
dll.print_sensor_pos.restype = None

def sensor_test(df):
    # Call the C function
    ctypes_struct_array = capteur2struct(df)
    return dll.print_sensor_pos(len(ctypes_struct_array), ctypes_struct_array)

## -----------------------------------------------------------------------------

dll.simpoint.argtypes = (ctypes.c_int, ctypes.POINTER(Capteur),
                         ctypes.c_double, ctypes.c_double, ctypes.c_double,
                         ctypes.c_int, ctypes.c_int)
dll.simpoint.restype = ctypes.c_double

def simulate_point(df, x, y, r, iterations, seed):
    # Call the C function
    ctypes_struct_array = capteur2struct(df)
    p = dll.simpoint(len(ctypes_struct_array), ctypes_struct_array,
                     x, y, r, iterations, seed)
    paquets = [ctypes_struct_array[i].paquets for i in range(len(ctypes_struct_array))]
    return {'plausibilite': p, 'paquets': paquets}

## -----------------------------------------------------------------------------

dll.simgrid.argtypes = (ctypes.c_int, ctypes.POINTER(Capteur),
                        Grille, ctypes.c_double,
                        ctypes.c_int, ctypes.c_int,
                        ctypes.c_char_p)
dll.simgrid.restype = None

def simulate_grid(capteurs, grille, r, iterations, seed, fname):
    # Call the C function
    ctypes_fname = ctypes.c_char_p(str(fname).encode('utf-8'))
    ctypes_struct_array = capteur2struct(capteurs)
    return dll.simgrid(len(ctypes_struct_array), ctypes_struct_array,
                       grille, r, iterations, seed, ctypes_fname)
