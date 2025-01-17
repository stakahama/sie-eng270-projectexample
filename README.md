# SIE ENG270 Project Repository Example

## Project Description

This program simulates the [pollution exercise](https://sieprog.ch/#c/pollution) ("Le bateau pollueur").

The program will:
1. Read in inputs
   - sensor locations found in "*data/capteurs.csv*",
   - seed values for random number generation ("*data/seedvalues.json*"), and
   - parameters of the simulations ("*data/paramsimul.json*").
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
- "*data/capteur.csv*" is a tab-delimited file (not comma-separated).
- "*data/seedvalues.json*" is a JSON file.
- "*data/paramsimul.json*" is a JSON file.

Outputs:
- "*outputs/plausibilite.csv*" is a comma-delimted file.
- "*outputs/plausibilite.png*" is an image file

### Implementation details

Overview:
- The simulation is handled by C. The C program is compiled to a shared library, which is called by Python via the `ctypes` module.
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
This convention works for both Python scripts and Jupyter notebooks / Quarto documents, so the following two commands will also generate the same output.
```
$ quarto render docs/analysis.qmd
$ cd docs && quarto render analysis.qmd
```

## Instructions

To reproduce results in the report, two steps should be followed:

1. Build (compile) the shared library.
2. Run the program.

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

## (***Extra notes for students***)

### Regarding relative paths

When running python scripts from the command line:
```{bash}
python src/simulategrid.py
```
the working directory of the Python program is the project root (the location of this README.md file) and not "*src/*". 

When running the Python program after changing into the "*code/*" directory,
```{bash}
cd src
python simulategrid.py
```
the working directory is "*src/*". (A word of caution - changing directories multiple times in shell scripts can be tricky since the program may end up in a different working directory than intended if any of the programs that are called exit with error.)

The important point is that relative paths to input files and other files/directories should be relative to the working directory. 

- If the Python program is run from the root directory, the relative path to "*capteurs.csv*" is "*data/capteurs.csv*". 
- If the Python program is run from the "*code/*" directory, the relative path to "*capteurs.csv*" is "*../data/capteurs.csv*". 

Using the `sys.path[0]` and `ROOT` convention as shown in this project example circumvents this ambiguity by anchoring all paths to `ROOT`.

### Regarding the build process

Note that to build on Windows, the "*Makefile*" line 
```{lang-makefile}
CFLAGS=-Wall -fPIC -O2
```
should be replaced with
```{lang-makefile}
CFLAGS=-Wall -fPIC -O2 -Dsrandom=srand -Drandom=rand
```
to account for the fact that `srand` and `rand` are to be used in place of `srandom` and `random`, respectively. It is possible to further automate this substitution by writing conditional statements in the "*Makefile*" based on the operating system - e.g.,
```{lang-makefile}
ifeq ($(OS), Windows_NT)
    CCFLAGS += -Dsrandom=srand -Drandom=rand
endif
```
Alteratively create separate makefiles for each operating system - e.g., "*Makefile.win*" and "*Makefile.linux*" and so on, and the user must rename the appropriate file on their machine to "*Makefile*" before calling `make`.

The "*Makefile*" is a general build tool and is useful for projects with many files that need to be compiled. For simple cases, you can create a shell script called, for instance, "*build.bash*" in the root directory with the following contents:
```{bash}
#!/bin/bash
mkdir -p bin
gcc -Wall -fPIC -O2 -o bin/cmain.o -c code/cmain.c
gcc -Wall -fPIC -O2 -o bin/cfunctions.o -c code/cfunctions.c -lm
gcc -shared -o bin/clib.so bin/cmain.o bin/cfunctions.o
```
Then, the library files can be built with 
```{bash}
bash build.bash
```
before running `bash run.bash`.
