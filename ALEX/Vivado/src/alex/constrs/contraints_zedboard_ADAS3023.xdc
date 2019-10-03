# ----------------------------------------------------------------------------
# JA Pmod - Bank 13 - ADC 2
# ---------------------------------------------------------------------------- 
set_property PACKAGE_PIN Y11  [get_ports busy_1]; # "JA1"
set_property PACKAGE_PIN AA11 [get_ports sck_1];  # "JA2"
set_property PACKAGE_PIN Y10  [get_ports reset_1];# "JA3"
set_property PACKAGE_PIN AA9  [get_ports csn_1];  # "JA4"
set_property PACKAGE_PIN AB11 [get_ports cnv_1];  # "JA7"
set_property PACKAGE_PIN AB10 [get_ports sdo_1];  # "JA8"
set_property PACKAGE_PIN AB9  [get_ports pd_1];   # "JA9"
set_property PACKAGE_PIN AA8  [get_ports din_1];  # "JA10"

set_property IOSTANDARD LVCMOS33 [get_ports busy_1]
set_property IOSTANDARD LVCMOS33 [get_ports sck_1]
set_property IOSTANDARD LVCMOS33 [get_ports reset_1]
set_property IOSTANDARD LVCMOS33 [get_ports csn_1]
set_property IOSTANDARD LVCMOS33 [get_ports cnv_1]
set_property IOSTANDARD LVCMOS33 [get_ports sdo_1]
set_property IOSTANDARD LVCMOS33 [get_ports pd_1]
set_property IOSTANDARD LVCMOS33 [get_ports din_1]


# ----------------------------------------------------------------------------
# JB Pmod - Bank 13 - ADC 1
# ---------------------------------------------------------------------------- 
set_property PACKAGE_PIN W12 [get_ports busy]; # "JB1"
set_property PACKAGE_PIN W11 [get_ports sck];  # "JB2"
set_property PACKAGE_PIN V10 [get_ports reset];# "JB3"
set_property PACKAGE_PIN W8  [get_ports csn];  # "JB4"
set_property PACKAGE_PIN V12 [get_ports cnv];  # "JB7"
set_property PACKAGE_PIN W10 [get_ports sdo];  # "JB8"
set_property PACKAGE_PIN V9  [get_ports pd];   # "JB9"
set_property PACKAGE_PIN V8  [get_ports din];  # "JB10"

set_property IOSTANDARD LVCMOS33 [get_ports busy]
set_property IOSTANDARD LVCMOS33 [get_ports sck]
set_property IOSTANDARD LVCMOS33 [get_ports reset]
set_property IOSTANDARD LVCMOS33 [get_ports csn]
set_property IOSTANDARD LVCMOS33 [get_ports cnv]
set_property IOSTANDARD LVCMOS33 [get_ports sdo]
set_property IOSTANDARD LVCMOS33 [get_ports pd]
set_property IOSTANDARD LVCMOS33 [get_ports din]


# ----------------------------------------------------------------------------
# JC Pmod - Bank 13 - IED front panel LEDs
# ---------------------------------------------------------------------------- 
set_property PACKAGE_PIN AB7 [get_ports {trip_led_front[0]}];  # "JC1_P"
set_property PACKAGE_PIN AB6 [get_ports {trip_led_front[1]}];  # "JC1_N"
set_property PACKAGE_PIN Y4  [get_ports {trip_led_front[2]}];  # "JC2_P"
set_property PACKAGE_PIN AA4 [get_ports {trip_led_front[3]}];  # "JC2_N"
set_property PACKAGE_PIN R6  [get_ports {trip_led_front[4]}];  # "JC3_P"
set_property PACKAGE_PIN T6  [get_ports {trip_led_front[5]}];  # "JC3_N"
set_property PACKAGE_PIN T4  [get_ports {trip_led_front[6]}];  # "JC4_P"
set_property PACKAGE_PIN U4  [get_ports {trip_led_front[7]}];  # "JC4_N"


set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_front[0]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_front[1]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_front[2]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_front[3]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_front[4]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_front[5]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_front[6]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_front[7]}];


# ----------------------------------------------------------------------------
# JD Pmod - Bank 13 - digital input and output card
# ---------------------------------------------------------------------------- 
# digital in, upper row
set_property PACKAGE_PIN V7 [get_ports {MAX1_CS}];    # "JD1_P" -- clock for MAX
set_property PACKAGE_PIN W7 [get_ports {MAX1_SCK}];   # "JD1_N" -- chip select for MAX
set_property PACKAGE_PIN V5 [get_ports {MAX1_DIN}];   # "JD2_P" -- data coming from MAX
set_property PACKAGE_PIN V4 [get_ports {MAX1_DOUT}];  # "JD2_N" -- command bytes for MAX

# digital out, lower row
set_property PACKAGE_PIN W6 [get_ports {MAX2_CS}];   # "JD3_P"
set_property PACKAGE_PIN W5 [get_ports {MAX2_SCK}];  # "JD3_N"
set_property PACKAGE_PIN U6 [get_ports {MAX2_DIN}];  # "JD4_P"
set_property PACKAGE_PIN U5 [get_ports {MAX2_DOUT}]; # "JD4_N"

set_property IOSTANDARD LVCMOS33 [get_ports {MAX1_CS}];
set_property IOSTANDARD LVCMOS33 [get_ports {MAX1_SCK}];
set_property IOSTANDARD LVCMOS33 [get_ports {MAX1_DIN}];
set_property IOSTANDARD LVCMOS33 [get_ports {MAX1_DOUT}];
set_property IOSTANDARD LVCMOS33 [get_ports {MAX2_CS}];
set_property IOSTANDARD LVCMOS33 [get_ports {MAX2_SCK}];
set_property IOSTANDARD LVCMOS33 [get_ports {MAX2_DIN}];
set_property IOSTANDARD LVCMOS33 [get_ports {MAX2_DOUT}];

# ----------------------------------------------------------------------------
# User LEDs - Bank 33 - indicate trip signals
# ---------------------------------------------------------------------------- 
set_property PACKAGE_PIN T22 [get_ports {trip_led_board[0]}];  # "LD0"
set_property PACKAGE_PIN T21 [get_ports {trip_led_board[1]}];  # "LD1"
set_property PACKAGE_PIN U22 [get_ports {trip_led_board[2]}];  # "LD2"
set_property PACKAGE_PIN U21 [get_ports {trip_led_board[3]}];  # "LD3"
set_property PACKAGE_PIN V22 [get_ports {trip_led_board[4]}];  # "LD4"
set_property PACKAGE_PIN W22 [get_ports {trip_led_board[5]}];  # "LD5"
set_property PACKAGE_PIN U19 [get_ports {trip_led_board[6]}];  # "LD6"
set_property PACKAGE_PIN U14 [get_ports {trip_led_board[7]}];  # "LD7"

set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_board[0]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_board[1]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_board[2]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_board[3]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_board[4]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_board[5]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_board[6]}];
set_property IOSTANDARD LVCMOS33 [get_ports {trip_led_board[7]}];

# ----------------------------------------------------------------------------
# FMC Header
# ----------------------------------------------------------------------------
set_property PACKAGE_PIN G15 [get_ports TX]
set_property IOSTANDARD LVCMOS33 [get_ports TX]
set_property PULLUP true [get_ports TX]

set_property PACKAGE_PIN R19 [get_ports RX] 
#R19 on J8-1 works, receiver IP works
set_property IOSTANDARD LVCMOS33 [get_ports RX]
#set_property PULLUP true [get_ports RX]
# ----------------------------------------------------------------------------
# Enabling RS485 Transceiver (Always turned ON)
# ----------------------------------------------------------------------------
set_property PACKAGE_PIN C20 [get_ports TX_en]
set_property IOSTANDARD LVCMOS33 [get_ports TX_en]

set_property PACKAGE_PIN G16 [get_ports RX_en]
set_property IOSTANDARD LVCMOS33 [get_ports RX_en]


