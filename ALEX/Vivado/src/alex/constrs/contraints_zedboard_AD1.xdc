#sliders:
set_property PACKAGE_PIN F22  [get_ports {manual_trip}];
set_property IOSTANDARD LVCMOS25 [get_ports {manual_trip}];
set_property PACKAGE_PIN G22  [get_ports {enable_trip}];
set_property IOSTANDARD LVCMOS25 [get_ports {enable_trip}];

# ----------------------------------------------------------------------------
# JA Pmod - Bank 13 
# ---------------------------------------------------------------------------- 
set_property PACKAGE_PIN Y11  [get_ports {cs}];  # "JA1"
set_property PACKAGE_PIN AA11 [get_ports {d0}];  # "JA2"
set_property PACKAGE_PIN Y10  [get_ports {d1}];  # "JA3"
set_property PACKAGE_PIN AA9  [get_ports {clk_ad1}];  # "JA4"

set_property IOSTANDARD LVCMOS33 [get_ports {cs}];
set_property IOSTANDARD LVCMOS33 [get_ports {d0}];
set_property IOSTANDARD LVCMOS33 [get_ports {d1}];
set_property IOSTANDARD LVCMOS33 [get_ports {clk_ad1}];

#set_property PACKAGE_PIN AB11 [get_ports {JA7}];  # "JA7"
#set_property PACKAGE_PIN AB10 [get_ports {JA8}];  # "JA8"
#set_property PACKAGE_PIN AB9 [get_ports {JA9}]; # "JA9"
#set_property PACKAGE_PIN AA8  [get_ports {JA10}];  # "JA10"
# ----------------------------------------------------------------------------
# User LEDs - Bank 33
# ---------------------------------------------------------------------------- 
set_property PACKAGE_PIN T22 [get_ports trip_led];  # "LD0"
set_property IOSTANDARD LVCMOS33 [get_ports trip_led];

set_property PACKAGE_PIN T21 [get_ports {dch_00[13]}];  # "LD1"
set_property PACKAGE_PIN U22 [get_ports {dch_00[12]}];  # "LD2"
set_property PACKAGE_PIN U21 [get_ports {dch_00[11]}];  # "LD3"
set_property PACKAGE_PIN V22 [get_ports {dch_00[10]}];  # "LD4"
set_property PACKAGE_PIN W22 [get_ports {dch_00[9]}];  # "LD5"
set_property PACKAGE_PIN U19 [get_ports {dch_00[8]}];  # "LD6"
set_property PACKAGE_PIN U14 [get_ports {dch_00[7]}];  # "LD7"



# ----------------------------------------------------------------------------
# JB Pmod - Bank 13
# ---------------------------------------------------------------------------- 
set_property PACKAGE_PIN W12 [get_ports {dch_00[15]}];  # "JB1"
set_property PACKAGE_PIN V8 [get_ports {dch_00[0]}];  # "JB10"
set_property PACKAGE_PIN W11 [get_ports {dch_00[6]}];  # "JB2"
set_property PACKAGE_PIN V10 [get_ports {dch_00[5]}];  # "JB3"
set_property PACKAGE_PIN W8 [get_ports {dch_00[4]}];  # "JB4"
set_property PACKAGE_PIN V12 [get_ports {dch_00[3]}];  # "JB7"
set_property PACKAGE_PIN W10 [get_ports {dch_00[2]}];  # "JB8"
set_property PACKAGE_PIN V9 [get_ports {dch_00[1]}];  # "JB9"
# ----------------------------------------------------------------------------
# JC Pmod - Bank 13
# ---------------------------------------------------------------------------- 
set_property PACKAGE_PIN AB6 [get_ports {dch_00[14]}];  # "LD0"



set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[0]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[1]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[2]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[3]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[4]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[5]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[6]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[7]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[8]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[9]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[10]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[11]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[12]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[13]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[14]}];
set_property IOSTANDARD LVCMOS33 [get_ports {dch_00[15]}];





#TRIP_LED[7:0] : out STD_LOGIC; LEDs
#set_property PACKAGE_PIN T22 [get_ports {TRIP_LED[0]}]
#set_property PACKAGE_PIN T21 [get_ports {TRIP_LED[1]}]
#set_property PACKAGE_PIN U22 [get_ports {TRIP_LED[2]}]
#set_property PACKAGE_PIN U21 [get_ports {TRIP_LED[3]}]
#set_property PACKAGE_PIN V22 [get_ports {TRIP_LED[4]}]
#set_property PACKAGE_PIN W22 [get_ports {TRIP_LED[5]}]
#set_property PACKAGE_PIN U19 [get_ports {TRIP_LED[6]}]
#set_property PACKAGE_PIN U14 [get_ports {TRIP_LED[7]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {TRIP_LED[0]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {TRIP_LED[1]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {TRIP_LED[2]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {TRIP_LED[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {TRIP_LED[4]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {TRIP_LED[5]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {TRIP_LED[6]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {TRIP_LED[7]}]
#set_property PULLDOWN true [get_ports {TRIP_LED[0]}]
#set_property PULLDOWN true [get_ports {TRIP_LED[1]}]
#set_property PULLDOWN true [get_ports {TRIP_LED[2]}]
#set_property PULLDOWN true [get_ports {TRIP_LED[3]}]
#set_property PULLDOWN true [get_ports {TRIP_LED[4]}]
#set_property PULLDOWN true [get_ports {TRIP_LED[5]}]
#set_property PULLDOWN true [get_ports {TRIP_LED[6]}]
# set_property PULLDOWN true [get_ports {TRIP_LED[7]}]