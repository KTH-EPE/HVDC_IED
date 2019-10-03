REM bat-file for generating the "alex" SDK workspace from the version controlled "alex" files

ECHO "Set path variables"

set WS=.

set PROJECTS=%WS%

set HW=%PROJECTS%\design_1_wrapper_hw_platform_0

set BSP=%PROJECTS%\alex_bsp

set SW=%PROJECTS%\alex

set XILINX_SDK=c:\Xilinx\SDK\2018.3

set XillinxPath=%XILINX_SDK%\eclipse\win64.o

set JavaPath=%XILINX_SDK%\tps\win64\jre\bin

set ArmCc=%XILINX_SDK%\gnu\arm\nt\bin;%XILINX_SDK%\gnuwin\bin

ECHO "Set path"

set path=%path%;%ArmCc%;%XILINX_SDK%\bin;%JavaPath%

ECHO "Create the HW project and the BSP"

CMD /C xsdk -batch -source alex_sdk_ws.tcl

ECHO "Generate the workspace"

%XillinxPath%\eclipsec -nosplash -data %WS% -application org.eclipse.cdt.managedbuilder.core.headlessbuild -import %HW% -import %BSP% -import %SW% 
