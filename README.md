# SIE ENG270 Project Repository Example

## Project Description

This program simulates the [pollution exercise](https://sieprog.ch/#c/pollution) ("Le bateau pollueur").

The program will:
1. Read in sensors locations found in "*data/capteur.csv*".
2. Generate a table of plausibilities ("*results/plausibilite.csv*").
3. Plot the table of plausibilities ("*results/plausibilite.csv*").

## Project structure

- "*data/*" contains input data
- "*results/*" contains program outputs
- "*code/*" contains program code
- "*code_reference/*" contains the original solution from sieprog.ch (in pure C) for comparison
- ("*bin/*") generated after compiling C code
- "*documentation/*" contains a notebook illustrating use and validation of the code

### Inputs and outputs

Inputs:
- "*data/sensors.csv*" is a tab-delimited file.

Outputs:
- "*results/plausibilite.csv*" is a comma-delimted file.
- "*results/plot.png*" is an image file

### Implementation details

Overview:
- The simulation is handled by C. The C program is compiled to a static library, which is called by Python via the `ctypes` module.
- Python handles most of the I/O, which includes reading sensor information and formatting the output of the single point simulation.
- For the grid simulation, the C program directly writes each simulation result to a CSV file designated by the calling Python script.

Structure. In the directory "*code/*":
- "*simulategrid.py*":
  - imports "*mylib.py*" as a module, which wraps the compiled C library file.
  - reads in "*data/sensors.csv*" and executes the C code.
- "*analysis.py*":
  - reads in the generated output ("*results/plausibilite.csv*") and makes the plot.

In each Python code, the project root directory is assigned using 

```{python}
import sys
from pathlib import Path
ROOT = Path(sys.path[0]).parent
```
`sys.path[0]` is the directory of the script or noteoobk file, and not the working directory of the shell from which the code is called. This allows the following commands to produce equivalent output. Starting in the project root directory:
```
$ python code/simulategrid.py
$ cd code && python simulategrid.py
```
This convention works for both Python scripts and Jupyter notebooks / Quarto documents, so the following two commands will also generate equivalent output.
```
$ quarto render documentation/analysis.qmd --to-pdf
$ cd code && quarto render analysis.qmd --to-pdf
```

## Instructions

To compile the C code, run the following line in the terminal from the project root directory (location of this README.md file):
```{sh}
make
```
This command will create a directory called bin/ and populated it with C object files, and the compiled .so file.

To run the Python and C code, run the following line in the terminal from the project root directory:
```{sh}
bash run.sh
```
This command will run the program and generate all of the output described above.

To generate documentation for the validation, run the following command from the root directory:
```{sh}
quarto render documentation/analysis.qmd --to pdf
```
This generates the file "*documentation/analysis.pdf*".

## Requirements

Versions of Python and C used are as follows. Optionally, the Quarto version is also included for rendering the "*documentation/analysis.qmd*" file. 
```
$ python --version
Python 3.9.18

$ gcc --version
gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0

$ quarto --version
1.3.450
```

The "*requirements.txt*" file for Python packages was generated with the command
```{sh}
conda list --export > requirements.txt
```
and deleting all but the relevant packages specifically used by this project.


## Credits

The code is adapted from the [solutions](https://sieprog.ch/#c/pollution/solutions) of sieprog.ch.
