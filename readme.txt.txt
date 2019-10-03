The open-source hardware and software for the IED will be made available from github.com/KTH/ using the MIT open-source license, once the paper is accepted for publication.

ALEX:
for Vivado and Vivado-SDK 2018.3
The project is called alex after Alexander the Great who cut the gordic knot.   
These are the minimum files for the alex-project (source code + relevant project files). 
They are version-controlled with git (not in the attachement of this paper, but in general).
It is not possible to have a space in a path if you want to do the below.
Use batch files in alex/Vivado to run tcl files to re-generate the IP-projects and the "main" 
alex-projects in Vivado.
Export the Vivado bitstream+hardware to alex/SDK/alex (design_1_wrapper.hdf).
Use the batch file in alex/SDK/alex to re-generate the SDK project. 
Follow the instructions in alex_main.c to set the right IP adresses manually. 
Compile and download to Zedboard.

LogicAnalyzer:
Example workspace files for Analog Discovery 2. Waveforms previously exported from PSCAD.

MATLAB:
Run m-file to get IED GUI. Tested in MATLAB R2019a. Should work in other MATLAB versions. 

PMOD ADC: kicad 5-1-2 project and gerber files for PMOD ADC PCB
PMOD DIGIN: kicad 5-1-2 project and gerber files for PMOD DIGIN PCB
PMOD DIGOUT: kicad 5-1-2 project and gerber files for PMOD DIGOUT PCB

Components: excel file containing all required parts for building an IED, links refer to June 2018