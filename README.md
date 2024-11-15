# SIE ENG270 Project Repository Example

## Project Description

This program simulates the [pollution exercise](https://sieprog.ch/#c/pollution) ("Le bateau pollueur").

The program will:
1. Read in sensors locations found in "*data/capteurs.csv*".
2. Generate a table of plausibilities ("*outputs/plausibilite.csv*").
3. Plot the table of plausibilities ("*outputs/plausibilite.png*").

## Project structure

- "*data/*" contains input data
- "*outputs/*" contains program outputs
- "*src/*" contains program code
- "*soln_sieprog/*" contains the original solution from sieprog.ch (in pure C) for comparison
- ("*bin/*") generated after compiling C code
- "*docs/*" contains a notebook illustrating use and validation of the code

### Inputs and outputs

Inputs:
- "*data/capteurs.csv*" is a tab-delimited file.

Outputs:
- "*outputs/plausibilite.csv*" is a comma-delimted file.
- "*outputs/plausibilite.png*" is an image file

### Implementation details

Overview:
- The simulation is handled by C. The C program is compiled to a static library, which is called by Python via the `ctypes` module.
- Python handles most of the I/O, which includes reading sensor information and formatting the output of the single point simulation.
- For the grid simulation, the C program directly writes each simulation result to a CSV file designated by the calling Python script.

Structure. In the directory "*src/*":
- "*simulategrid.py*":
  - imports "*mylib.py*" as a module, which wraps the compiled C library file.
  - reads in "*data/capteurs.csv*" and executes the C code.
- "*analysis.py*":
  - reads in the generated output ("*outputs/plausibilite.csv*") and makes the plot.

In each Python code, the project root directory is assigned using 

```{python}
import sys
from pathlib import Path
ROOT = Path(sys.path[0]).parent
```
`sys.path[0]` is the directory of the script or noteoobk file, and not the working directory of the shell from which the code is called. This allows the following commands to produce equivalent output. Starting in the project root directory:
```
$ python src/simulategrid.py
$ cd src && python simulategrid.py
```
This convention works for both Python scripts and Jupyter notebooks / Quarto documents, so the following two commands will also generate equivalent output.
```
$ quarto render docs/analysis.qmd --to-pdf
$ cd src && quarto render analysis.qmd --to-pdf
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
quarto render docs/analysis.qmd --to pdf
```
This generates the file "*docs/analysis.pdf*".

## Requirements

Versions of Python and C used are as follows. Optionally, the Quarto version is also included for rendering the "*docs/analysis.qmd*" file. 
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
