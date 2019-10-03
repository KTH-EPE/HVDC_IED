sdk setws .

if {[file exists "./design_1_wrapper_hw_platform_0"] == 0} {
  sdk createhw -name design_1_wrapper_hw_platform_0 -hwspec design_1_wrapper.hdf
}

if {[file exists "./alex_bsp"] == 0} {
  sdk createbsp -name alex_bsp -hwproject design_1_wrapper_hw_platform_0 -proc ps7_cortexa9_0 -os standalone
}

