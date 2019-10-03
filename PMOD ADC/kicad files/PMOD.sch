EESchema Schematic File Version 4
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PMOD-rescue:ADAS-RESCUE-PMOD U1
U 1 1 5A67FF31
P 6050 2550
F 0 "U1" H 6000 2350 60  0000 C CNN
F 1 "ADAS" H 6050 2800 60  0000 C CNN
F 2 "Housings_DFN_QFN:QFN-40-1EP_6x6mm_Pitch0.5mm" V 5400 3400 60  0001 C CNN
F 3 "" V 5400 3400 60  0001 C CNN
	1    6050 2550
	1    0    0    -1  
$EndComp
Text GLabel 4800 2050 0    60   Input ~ 0
IN0
Text GLabel 4800 2150 0    60   Input ~ 0
IN1
Text GLabel 4800 2250 0    60   Input ~ 0
IN2
Text GLabel 4800 2350 0    60   Input ~ 0
IN3
Text GLabel 4800 2550 0    60   Input ~ 0
IN4
Text GLabel 4800 2650 0    60   Input ~ 0
IN5
Text GLabel 4800 2750 0    60   Input ~ 0
IN6
Text GLabel 4800 2850 0    60   Input ~ 0
IN7
Text GLabel 7500 2050 2    60   Input ~ 0
AGND
Text GLabel 7500 2250 2    60   Input ~ 0
AVDD
Text GLabel 7500 2350 2    60   Input ~ 0
DVDD
Text GLabel 7500 2450 2    60   Input ~ 0
ACAP
Text GLabel 7500 2550 2    60   Input ~ 0
DCAP
Text GLabel 7500 2650 2    60   Input ~ 0
AGND
Text GLabel 7500 2850 2    60   Input ~ 0
DGND
Text GLabel 5550 1500 1    60   Input ~ 0
AGND
Text GLabel 5650 850  1    60   Input ~ 0
VDDH
Text GLabel 5750 1100 1    60   Input ~ 0
VSSH
Text GLabel 5850 1500 1    60   Input ~ 0
REFN
Text GLabel 6050 1700 1    60   Input ~ 0
RGND
Text GLabel 6150 1700 1    60   Input ~ 0
REF2
Text GLabel 6250 1700 1    60   Input ~ 0
REF1
Text GLabel 6350 1700 1    60   Input ~ 0
REFIN
Text GLabel 6450 1700 1    60   Input ~ 0
RCAP
Text GLabel 5550 3350 3    60   Input ~ 0
CSn
Text GLabel 5650 3350 3    60   Input ~ 0
DIN
Text GLabel 5750 3350 3    60   Input ~ 0
RESET
Text GLabel 5850 3350 3    60   Input ~ 0
AGND
Text GLabel 5950 3350 3    60   Input ~ 0
PD
Text GLabel 6050 3350 3    60   Input ~ 0
SCK
Text GLabel 6150 3350 3    60   Input ~ 0
VIO
Text GLabel 6250 3350 3    60   Input ~ 0
SDO
Text GLabel 6350 3350 3    60   Input ~ 0
BUSY2
Text GLabel 6450 3350 3    60   Input ~ 0
CNV
Text GLabel 4800 2450 0    60   Input ~ 0
AGND
Text GLabel 850  600  0    60   Input ~ 0
IN0_i
$Comp
L Device:C C17
U 1 1 5A680DDE
P 1250 850
F 0 "C17" H 1275 950 50  0000 L CNN
F 1 "2700p" H 1275 750 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1288 700 50  0001 C CNN
F 3 "" H 1250 850 50  0001 C CNN
	1    1250 850 
	1    0    0    -1  
$EndComp
Text GLabel 1350 600  2    60   Input ~ 0
IN0
Text GLabel 1600 1000 2    60   Input ~ 0
AGND
Text GLabel 850  1150 0    60   Input ~ 0
IN1_i
$Comp
L Device:C C18
U 1 1 5A6810EC
P 1250 1400
F 0 "C18" H 1275 1500 50  0000 L CNN
F 1 "2700p" H 1275 1300 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1288 1250 50  0001 C CNN
F 3 "" H 1250 1400 50  0001 C CNN
	1    1250 1400
	1    0    0    -1  
$EndComp
Text GLabel 1350 1150 2    60   Input ~ 0
IN1
Text GLabel 1600 1550 2    60   Input ~ 0
AGND
Text GLabel 850  1650 0    60   Input ~ 0
IN2_i
$Comp
L Device:C C19
U 1 1 5A681293
P 1250 1900
F 0 "C19" H 1275 2000 50  0000 L CNN
F 1 "2700p" H 1275 1800 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1288 1750 50  0001 C CNN
F 3 "" H 1250 1900 50  0001 C CNN
	1    1250 1900
	1    0    0    -1  
$EndComp
Text GLabel 1350 1650 2    60   Input ~ 0
IN2
Text GLabel 1600 2050 2    60   Input ~ 0
AGND
Text GLabel 850  2200 0    60   Input ~ 0
IN3_i
$Comp
L Device:C C20
U 1 1 5A6812B9
P 1250 2450
F 0 "C20" H 1275 2550 50  0000 L CNN
F 1 "2700p" H 1275 2350 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1288 2300 50  0001 C CNN
F 3 "" H 1250 2450 50  0001 C CNN
	1    1250 2450
	1    0    0    -1  
$EndComp
Text GLabel 1350 2200 2    60   Input ~ 0
IN3
Text GLabel 1600 2600 2    60   Input ~ 0
AGND
Text GLabel 850  2750 0    60   Input ~ 0
IN4_i
$Comp
L Device:C C21
U 1 1 5A6816D1
P 1250 3000
F 0 "C21" H 1275 3100 50  0000 L CNN
F 1 "2700p" H 1275 2900 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1288 2850 50  0001 C CNN
F 3 "" H 1250 3000 50  0001 C CNN
	1    1250 3000
	1    0    0    -1  
$EndComp
Text GLabel 1350 2750 2    60   Input ~ 0
IN4
Text GLabel 1600 3150 2    60   Input ~ 0
AGND
Text GLabel 850  3300 0    60   Input ~ 0
IN5_i
$Comp
L Device:C C22
U 1 1 5A6816F7
P 1250 3550
F 0 "C22" H 1275 3650 50  0000 L CNN
F 1 "2700p" H 1275 3450 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1288 3400 50  0001 C CNN
F 3 "" H 1250 3550 50  0001 C CNN
	1    1250 3550
	1    0    0    -1  
$EndComp
Text GLabel 1350 3300 2    60   Input ~ 0
IN5
Text GLabel 1600 3700 2    60   Input ~ 0
AGND
Text GLabel 850  3800 0    60   Input ~ 0
IN6_i
$Comp
L Device:C C23
U 1 1 5A68171D
P 1250 4050
F 0 "C23" H 1275 4150 50  0000 L CNN
F 1 "2700p" H 1275 3950 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1288 3900 50  0001 C CNN
F 3 "" H 1250 4050 50  0001 C CNN
	1    1250 4050
	1    0    0    -1  
$EndComp
Text GLabel 1350 3800 2    60   Input ~ 0
IN6
Text GLabel 1600 4200 2    60   Input ~ 0
AGND
Text GLabel 850  4350 0    60   Input ~ 0
IN7_i
$Comp
L Device:C C24
U 1 1 5A681743
P 1250 4600
F 0 "C24" H 1275 4700 50  0000 L CNN
F 1 "2700p" H 1275 4500 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1288 4450 50  0001 C CNN
F 3 "" H 1250 4600 50  0001 C CNN
	1    1250 4600
	1    0    0    -1  
$EndComp
Text GLabel 1350 4350 2    60   Input ~ 0
IN7
Text GLabel 1600 4750 2    60   Input ~ 0
AGND
$Comp
L Device:C C27
U 1 1 5A68502B
P 5350 900
F 0 "C27" H 5375 1000 50  0000 L CNN
F 1 "0.1u" H 5375 800 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5388 750 50  0001 C CNN
F 3 "" H 5350 900 50  0001 C CNN
	1    5350 900 
	0    1    1    0   
$EndComp
$Comp
L Device:C C28
U 1 1 5A685126
P 5350 1150
F 0 "C28" H 5375 1250 50  0000 L CNN
F 1 "0.1u" H 5375 1050 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5388 1000 50  0001 C CNN
F 3 "" H 5350 1150 50  0001 C CNN
	1    5350 1150
	0    1    1    0   
$EndComp
Text GLabel 8350 1650 0    60   Input ~ 0
REF1
Text GLabel 8700 1650 2    60   Input ~ 0
REF2
Text GLabel 8350 650  0    60   Input ~ 0
REFIN
Text GLabel 8350 850  0    60   Input ~ 0
REF1
$Comp
L Device:C C29
U 1 1 5A68690D
P 8800 1100
F 0 "C29" H 8825 1200 50  0000 L CNN
F 1 "0.1u" H 8825 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 8838 950 50  0001 C CNN
F 3 "" H 8800 1100 50  0001 C CNN
	1    8800 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C36
U 1 1 5A686982
P 9100 1100
F 0 "C36" H 9125 1200 50  0000 L CNN
F 1 "10u" H 9125 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9138 950 50  0001 C CNN
F 3 "" H 9100 1100 50  0001 C CNN
	1    9100 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C37
U 1 1 5A6869E3
P 9400 1100
F 0 "C37" H 9425 1200 50  0000 L CNN
F 1 "0.1u" H 9425 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9438 950 50  0001 C CNN
F 3 "" H 9400 1100 50  0001 C CNN
	1    9400 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C41
U 1 1 5A686A60
P 9700 1100
F 0 "C41" H 9725 1200 50  0000 L CNN
F 1 "10u" H 9725 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9738 950 50  0001 C CNN
F 3 "" H 9700 1100 50  0001 C CNN
	1    9700 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C45
U 1 1 5A686ADD
P 10000 1100
F 0 "C45" H 10025 1200 50  0000 L CNN
F 1 "0.1u" H 10025 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 10038 950 50  0001 C CNN
F 3 "" H 10000 1100 50  0001 C CNN
	1    10000 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C46
U 1 1 5A686B44
P 10300 1100
F 0 "C46" H 10325 1200 50  0000 L CNN
F 1 "10u" H 10325 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 10338 950 50  0001 C CNN
F 3 "" H 10300 1100 50  0001 C CNN
	1    10300 1100
	1    0    0    -1  
$EndComp
Text GLabel 8350 1250 0    60   Input ~ 0
AGND
$Comp
L Device:C C33
U 1 1 5A687870
P 9050 2100
F 0 "C33" H 9075 2200 50  0000 L CNN
F 1 "0.1u" H 9075 2000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9088 1950 50  0001 C CNN
F 3 "" H 9050 2100 50  0001 C CNN
	1    9050 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C38
U 1 1 5A687907
P 9650 2100
F 0 "C38" H 9675 2200 50  0000 L CNN
F 1 "0.1u" H 9675 2000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9688 1950 50  0001 C CNN
F 3 "" H 9650 2100 50  0001 C CNN
	1    9650 2100
	1    0    0    -1  
$EndComp
Text GLabel 8500 1950 0    60   Input ~ 0
AVDD
Text GLabel 10200 1950 2    60   Input ~ 0
DVDD
$Comp
L Device:C C34
U 1 1 5A688761
P 9050 2500
F 0 "C34" H 9075 2600 50  0000 L CNN
F 1 "0.1u" H 9075 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9088 2350 50  0001 C CNN
F 3 "" H 9050 2500 50  0001 C CNN
	1    9050 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C39
U 1 1 5A688767
P 9650 2500
F 0 "C39" H 9675 2600 50  0000 L CNN
F 1 "0.1u" H 9675 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9688 2350 50  0001 C CNN
F 3 "" H 9650 2500 50  0001 C CNN
	1    9650 2500
	1    0    0    -1  
$EndComp
Text GLabel 8500 2350 0    60   Input ~ 0
ACAP
Text GLabel 10200 2350 2    60   Input ~ 0
DCAP
$Comp
L Device:C C35
U 1 1 5A688A22
P 9050 2900
F 0 "C35" H 9075 3000 50  0000 L CNN
F 1 "0.1u" H 9075 2800 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9088 2750 50  0001 C CNN
F 3 "" H 9050 2900 50  0001 C CNN
	1    9050 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C40
U 1 1 5A688A28
P 9650 2900
F 0 "C40" H 9675 3000 50  0000 L CNN
F 1 "0.1u" H 9675 2800 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9688 2750 50  0001 C CNN
F 3 "" H 9650 2900 50  0001 C CNN
	1    9650 2900
	1    0    0    -1  
$EndComp
Text GLabel 8500 2750 0    60   Input ~ 0
RCAP
Text GLabel 10200 2750 2    60   Input ~ 0
VIO
Text GLabel 8800 3600 0    60   Input ~ 0
SCK
$Comp
L Device:R R25
U 1 1 5A68DCDC
P 9200 3750
F 0 "R25" V 9280 3750 50  0000 C CNN
F 1 "10K" V 9200 3750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 9130 3750 50  0001 C CNN
F 3 "" H 9200 3750 50  0001 C CNN
	1    9200 3750
	1    0    0    -1  
$EndComp
Text GLabel 9200 4050 3    60   Input ~ 0
DGND
Text GLabel 9400 3600 2    60   Input ~ 0
SCK_O
Text GLabel 8900 4450 0    60   Input ~ 0
SDO
Text GLabel 8900 4750 0    60   Input ~ 0
VIO
$Comp
L Device:R R24
U 1 1 5A68E78F
P 9050 4600
F 0 "R24" V 9130 4600 50  0000 C CNN
F 1 "15k" V 8950 4600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 8980 4600 50  0001 C CNN
F 3 "" H 9050 4600 50  0001 C CNN
	1    9050 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R27
U 1 1 5A68E795
P 9300 4600
F 0 "R27" V 9380 4600 50  0000 C CNN
F 1 "15K" V 9300 4600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 9230 4600 50  0001 C CNN
F 3 "" H 9300 4600 50  0001 C CNN
	1    9300 4600
	1    0    0    -1  
$EndComp
Text GLabel 9300 4900 3    60   Input ~ 0
DGND
Text GLabel 9500 4450 2    60   Input ~ 0
SDO_O
Text GLabel 8850 5300 0    60   Input ~ 0
DIN
$Comp
L Device:R R26
U 1 1 5A68F1ED
P 9250 5450
F 0 "R26" V 9330 5450 50  0000 C CNN
F 1 "10K" V 9250 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 9180 5450 50  0001 C CNN
F 3 "" H 9250 5450 50  0001 C CNN
	1    9250 5450
	1    0    0    -1  
$EndComp
Text GLabel 9250 5750 3    60   Input ~ 0
DGND
Text GLabel 9450 5300 2    60   Input ~ 0
DIN_O
Text GLabel 10150 3600 0    60   Input ~ 0
RESET
Text GLabel 10150 3900 0    60   Input ~ 0
VIO
$Comp
L Device:R R29
U 1 1 5A68FE35
P 10300 3750
F 0 "R29" V 10380 3750 50  0000 C CNN
F 1 "10K" V 10200 3750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10230 3750 50  0001 C CNN
F 3 "" H 10300 3750 50  0001 C CNN
	1    10300 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R31
U 1 1 5A68FE3B
P 10550 3750
F 0 "R31" V 10630 3750 50  0000 C CNN
F 1 "10K" V 10550 3750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10480 3750 50  0001 C CNN
F 3 "" H 10550 3750 50  0001 C CNN
	1    10550 3750
	1    0    0    -1  
$EndComp
Text GLabel 10550 4050 3    60   Input ~ 0
DGND
Text GLabel 10750 3600 2    60   Input ~ 0
RESET_O
Text GLabel 10150 4600 0    60   Input ~ 0
CNV
$Comp
L Device:R R32
U 1 1 5A69053A
P 10550 4750
F 0 "R32" V 10630 4750 50  0000 C CNN
F 1 "10K" V 10550 4750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10480 4750 50  0001 C CNN
F 3 "" H 10550 4750 50  0001 C CNN
	1    10550 4750
	1    0    0    -1  
$EndComp
Text GLabel 10550 5050 3    60   Input ~ 0
DGND
Text GLabel 10750 4600 2    60   Input ~ 0
CNV_O
$Comp
L Device:C C47
U 1 1 5A690703
P 10300 4750
F 0 "C47" H 10325 4850 50  0000 L CNN
F 1 "100p" H 10325 4650 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 10338 4600 50  0001 C CNN
F 3 "" H 10300 4750 50  0001 C CNN
	1    10300 4750
	1    0    0    -1  
$EndComp
Text GLabel 10150 5450 0    60   Input ~ 0
PD
$Comp
L Device:R R30
U 1 1 5A690A32
P 10300 5600
F 0 "R30" V 10380 5600 50  0000 C CNN
F 1 "10K" V 10200 5600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10230 5600 50  0001 C CNN
F 3 "" H 10300 5600 50  0001 C CNN
	1    10300 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R33
U 1 1 5A690A38
P 10550 5600
F 0 "R33" V 10630 5600 50  0000 C CNN
F 1 "10K" V 10550 5600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10480 5600 50  0001 C CNN
F 3 "" H 10550 5600 50  0001 C CNN
	1    10550 5600
	1    0    0    -1  
$EndComp
Text GLabel 10550 5900 3    60   Input ~ 0
DGND
Text GLabel 10750 5450 2    60   Input ~ 0
PD_O
Text GLabel 7550 5300 0    60   Input ~ 0
CSn
$Comp
L Device:R R21
U 1 1 5A695F41
P 7950 5450
F 0 "R21" V 8030 5450 50  0000 C CNN
F 1 "10K" V 7950 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7880 5450 50  0001 C CNN
F 3 "" H 7950 5450 50  0001 C CNN
	1    7950 5450
	1    0    0    -1  
$EndComp
Text GLabel 7950 5750 3    60   Input ~ 0
DGND
Text GLabel 8150 5300 2    60   Input ~ 0
CSn_O
$Comp
L Device:R R20
U 1 1 5A697D8F
P 7950 4600
F 0 "R20" V 8030 4600 50  0000 C CNN
F 1 "10K" V 7950 4600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7880 4600 50  0001 C CNN
F 3 "" H 7950 4600 50  0001 C CNN
	1    7950 4600
	1    0    0    -1  
$EndComp
Text GLabel 7950 4900 3    60   Input ~ 0
DGND
Text GLabel 8150 4450 2    60   Input ~ 0
BUSY2_O
Text GLabel 8300 1050 0    60   Input ~ 0
REFN
Text GLabel 9100 3200 0    60   Input ~ 0
AGND
Text GLabel 9100 3400 0    60   Input ~ 0
RGND
Text GLabel 900  5000 0    60   Input ~ 0
+15V
Text GLabel 900  5600 0    60   Input ~ 0
-15V
$Comp
L Device:C C2
U 1 1 5A6B09B5
P 1100 5150
F 0 "C2" H 1125 5250 50  0000 L CNN
F 1 "10u" H 1125 5050 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1138 5000 50  0001 C CNN
F 3 "" H 1100 5150 50  0001 C CNN
	1    1100 5150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5A6B0A72
P 1100 5400
F 0 "C3" H 1125 5500 50  0000 L CNN
F 1 "10u" H 1125 5300 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1138 5250 50  0001 C CNN
F 3 "" H 1100 5400 50  0001 C CNN
	1    1100 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C15
U 1 1 5A6B0B17
P 1600 5150
F 0 "C15" H 1625 5250 50  0000 L CNN
F 1 "100n" H 1625 5050 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1638 5000 50  0001 C CNN
F 3 "" H 1600 5150 50  0001 C CNN
	1    1600 5150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C16
U 1 1 5A6B0BCC
P 1600 5400
F 0 "C16" H 1625 5500 50  0000 L CNN
F 1 "100n" H 1625 5300 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1638 5250 50  0001 C CNN
F 3 "" H 1600 5400 50  0001 C CNN
	1    1600 5400
	1    0    0    -1  
$EndComp
Text GLabel 1900 5000 2    60   Input ~ 0
VDDH
Text GLabel 1900 5600 2    60   Input ~ 0
VSSH
Text GLabel 1900 5250 2    60   Input ~ 0
AGND
Text GLabel 900  5900 0    60   Input ~ 0
3.3V
$Comp
L Device:C C1
U 1 1 5A6B3A42
P 1050 6050
F 0 "C1" H 1075 6150 50  0000 L CNN
F 1 "100n" H 1075 5950 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1088 5900 50  0001 C CNN
F 3 "" H 1050 6050 50  0001 C CNN
	1    1050 6050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5A6B3B03
P 1350 6050
F 0 "C5" H 1375 6150 50  0000 L CNN
F 1 "10u" H 1375 5950 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1388 5900 50  0001 C CNN
F 3 "" H 1350 6050 50  0001 C CNN
	1    1350 6050
	1    0    0    -1  
$EndComp
Text GLabel 900  6200 0    60   Input ~ 0
DGND
Text GLabel 1700 5900 2    60   Input ~ 0
VIO
Text GLabel 900  6550 0    60   Input ~ 0
+5V
$Comp
L Device:C C4
U 1 1 5A6B5217
P 1150 6700
F 0 "C4" H 1175 6800 50  0000 L CNN
F 1 "10u" H 1175 6600 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1188 6550 50  0001 C CNN
F 3 "" H 1150 6700 50  0001 C CNN
	1    1150 6700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C14
U 1 1 5A6B52C6
P 1400 6700
F 0 "C14" H 1425 6800 50  0000 L CNN
F 1 "100n" H 1425 6600 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1438 6550 50  0001 C CNN
F 3 "" H 1400 6700 50  0001 C CNN
	1    1400 6700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C25
U 1 1 5A6B5381
P 2250 6700
F 0 "C25" H 2275 6800 50  0000 L CNN
F 1 "10u" H 2275 6600 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 2288 6550 50  0001 C CNN
F 3 "" H 2250 6700 50  0001 C CNN
	1    2250 6700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C26
U 1 1 5A6B544C
P 2500 6700
F 0 "C26" H 2525 6800 50  0000 L CNN
F 1 "100n" H 2525 6600 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 2538 6550 50  0001 C CNN
F 3 "" H 2500 6700 50  0001 C CNN
	1    2500 6700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R17
U 1 1 5A6B5ADD
P 1850 6550
F 0 "R17" V 1930 6550 50  0000 C CNN
F 1 "20" V 1850 6550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 1780 6550 50  0001 C CNN
F 3 "" H 1850 6550 50  0001 C CNN
	1    1850 6550
	0    1    1    0   
$EndComp
Text GLabel 900  6850 0    60   Input ~ 0
AGND
Text GLabel 1600 7000 3    60   Input ~ 0
AVDD
Text GLabel 1850 7000 3    60   Input ~ 0
AGND
Text GLabel 2100 7000 3    60   Input ~ 0
DVDD
Text GLabel 2300 7000 3    60   Input ~ 0
DGND
$Comp
L PMOD-rescue:CONN_01X16 J1
U 1 1 5A6B87BF
P 2250 1400
F 0 "J1" H 2250 2250 50  0000 C CNN
F 1 "CONN_01X16" V 2350 1400 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x16_Pitch2.54mm" H 2250 1400 50  0001 C CNN
F 3 "" H 2250 1400 50  0001 C CNN
	1    2250 1400
	-1   0    0    1   
$EndComp
Text GLabel 2400 2400 0    60   Input ~ 0
AGND
Text GLabel 2650 750  2    60   Input ~ 0
IN0_i
Text GLabel 2650 950  2    60   Input ~ 0
IN1_i
Text GLabel 2650 1350 2    60   Input ~ 0
IN3_i
Text GLabel 2650 1550 2    60   Input ~ 0
IN4_i
Text GLabel 2650 1750 2    60   Input ~ 0
IN5_i
Text GLabel 2650 1950 2    60   Input ~ 0
IN6_i
Text GLabel 2650 2150 2    60   Input ~ 0
IN7_i
Text GLabel 2650 1150 2    60   Input ~ 0
IN2_i
$Comp
L PMOD-rescue:CONN_02X06 J2
U 1 1 5A6BB90A
P 3850 950
F 0 "J2" H 3850 1300 50  0000 C CNN
F 1 "CONN_02X06" H 3850 600 50  0000 C CNN
F 2 ".pretty:Multicomp_MC9A_2x06x2.54mm_Straight" H 3850 -250 50  0001 C CNN
F 3 "" H 3850 -250 50  0001 C CNN
	1    3850 950 
	1    0    0    -1  
$EndComp
Text GLabel 4100 800  2    60   Input ~ 0
SDO_O
Text GLabel 3600 700  0    60   Input ~ 0
BUSY2_O
Text GLabel 3600 1000 0    60   Input ~ 0
CSn_O
Text GLabel 4100 900  2    60   Input ~ 0
PD_O
Text GLabel 4100 1000 2    60   Input ~ 0
DIN_O
Text GLabel 4100 700  2    60   Input ~ 0
CNV_O
Text GLabel 3600 800  0    60   Input ~ 0
SCK_O
$Comp
L PMOD-rescue:CONN_01X02 J4
U 1 1 5A6B0008
P 2900 3300
F 0 "J4" H 2900 3450 50  0000 C CNN
F 1 "CONN_01X02" V 3000 3300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x02_Pitch2.54mm" H 2900 3300 50  0001 C CNN
F 3 "" H 2900 3300 50  0001 C CNN
	1    2900 3300
	-1   0    0    1   
$EndComp
Text GLabel 6700 5800 2    60   Input ~ 0
+15V
Text GLabel 6700 4350 2    60   Input ~ 0
-15V
Text GLabel 5250 7000 2    60   Input ~ 0
+5V
Text GLabel 3100 3350 2    60   Input ~ 0
AGND
Text GLabel 4100 1100 2    60   Input ~ 0
DGND
$Comp
L Device:R R39
U 1 1 5A7908D5
P 1000 600
F 0 "R39" V 1080 600 50  0000 C CNN
F 1 "90" V 1000 600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 930 600 50  0001 C CNN
F 3 "" H 1000 600 50  0001 C CNN
	1    1000 600 
	0    1    1    0   
$EndComp
$Comp
L Device:R R40
U 1 1 5A7A0600
P 1000 1150
F 0 "R40" V 1080 1150 50  0000 C CNN
F 1 "90" V 1000 1150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 930 1150 50  0001 C CNN
F 3 "" H 1000 1150 50  0001 C CNN
	1    1000 1150
	0    1    1    0   
$EndComp
$Comp
L Device:R R41
U 1 1 5A7A0735
P 1000 1650
F 0 "R41" V 1080 1650 50  0000 C CNN
F 1 "90" V 1000 1650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 930 1650 50  0001 C CNN
F 3 "" H 1000 1650 50  0001 C CNN
	1    1000 1650
	0    1    1    0   
$EndComp
$Comp
L Device:R R42
U 1 1 5A7A0887
P 1000 2200
F 0 "R42" V 1080 2200 50  0000 C CNN
F 1 "90" V 1000 2200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 930 2200 50  0001 C CNN
F 3 "" H 1000 2200 50  0001 C CNN
	1    1000 2200
	0    1    1    0   
$EndComp
$Comp
L Device:R R43
U 1 1 5A7A09DA
P 1000 2750
F 0 "R43" V 1080 2750 50  0000 C CNN
F 1 "90" V 1000 2750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 930 2750 50  0001 C CNN
F 3 "" H 1000 2750 50  0001 C CNN
	1    1000 2750
	0    1    1    0   
$EndComp
$Comp
L Device:R R44
U 1 1 5A7A0B46
P 1000 3300
F 0 "R44" V 1080 3300 50  0000 C CNN
F 1 "90" V 1000 3300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 930 3300 50  0001 C CNN
F 3 "" H 1000 3300 50  0001 C CNN
	1    1000 3300
	0    1    1    0   
$EndComp
$Comp
L Device:R R45
U 1 1 5A7A0CA3
P 1000 3800
F 0 "R45" V 1080 3800 50  0000 C CNN
F 1 "90" V 1000 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 930 3800 50  0001 C CNN
F 3 "" H 1000 3800 50  0001 C CNN
	1    1000 3800
	0    1    1    0   
$EndComp
$Comp
L Device:R R46
U 1 1 5A7A0DFF
P 1000 4350
F 0 "R46" V 1080 4350 50  0000 C CNN
F 1 "90" V 1000 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 930 4350 50  0001 C CNN
F 3 "" H 1000 4350 50  0001 C CNN
	1    1000 4350
	0    1    1    0   
$EndComp
Text GLabel 4100 1200 2    60   Input ~ 0
3.3V
Text GLabel 3600 1100 0    60   Input ~ 0
DGND
Text GLabel 3600 1200 0    60   Input ~ 0
3.3V
Text GLabel 4800 2950 0    60   Input ~ 0
AGND
Text GLabel 3600 900  0    60   Input ~ 0
RESET_O
Text GLabel 10150 5750 0    60   Input ~ 0
VIO
$Comp
L PMOD-rescue:LTC6994 U7
U 1 1 5AE92A18
P 4150 4750
F 0 "U7" H 3700 5150 50  0000 L CNN
F 1 "LTC6994" H 4300 5150 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TSOT-23-6_HandSoldering" H 4150 4750 50  0001 C CNN
F 3 "" H 4150 4750 50  0001 C CNN
	1    4150 4750
	1    0    0    -1  
$EndComp
Text GLabel 7800 4450 0    60   Input ~ 0
BUSY2
Wire Wire Line
	5550 1500 5550 1600
Connection ~ 5550 1600
Wire Wire Line
	5650 850  5650 900 
Wire Wire Line
	5750 1100 5750 1150
Wire Wire Line
	5850 1700 5850 1550
Wire Wire Line
	7300 2850 7400 2850
Wire Wire Line
	7300 2650 7400 2650
Wire Wire Line
	7300 2450 7500 2450
Wire Wire Line
	7300 2250 7500 2250
Wire Wire Line
	1250 4750 1600 4750
Wire Wire Line
	1250 4200 1600 4200
Wire Wire Line
	1250 3700 1600 3700
Wire Wire Line
	1250 3150 1600 3150
Wire Wire Line
	1250 2600 1600 2600
Wire Wire Line
	1250 2050 1600 2050
Wire Wire Line
	1250 1550 1600 1550
Wire Wire Line
	1250 1000 1600 1000
Wire Wire Line
	2500 6550 2250 6550
Wire Wire Line
	1150 4350 1250 4350
Wire Wire Line
	1150 3800 1250 3800
Wire Wire Line
	1150 3300 1250 3300
Wire Wire Line
	1150 2750 1250 2750
Wire Wire Line
	1150 2200 1250 2200
Wire Wire Line
	1150 1650 1250 1650
Wire Wire Line
	1150 1150 1250 1150
Wire Wire Line
	1150 600  1250 600 
Connection ~ 8800 1250
Connection ~ 9100 1250
Connection ~ 9400 1250
Connection ~ 10000 1250
Connection ~ 9700 1250
Wire Wire Line
	8800 1250 9100 1250
Wire Wire Line
	2650 750  2450 750 
Wire Wire Line
	2650 950  2450 950 
Wire Wire Line
	2650 1150 2450 1150
Wire Wire Line
	2650 1350 2450 1350
Wire Wire Line
	2650 1550 2450 1550
Wire Wire Line
	2650 1750 2450 1750
Wire Wire Line
	2650 1950 2450 1950
Wire Wire Line
	2650 2150 2450 2150
Connection ~ 2550 850 
Wire Wire Line
	2450 650  2550 650 
Connection ~ 2550 1050
Wire Wire Line
	2450 850  2550 850 
Connection ~ 2550 1250
Wire Wire Line
	2450 1050 2550 1050
Connection ~ 2550 1450
Wire Wire Line
	2450 1250 2550 1250
Connection ~ 2550 1650
Wire Wire Line
	2450 1450 2550 1450
Connection ~ 2550 1850
Wire Wire Line
	2450 1650 2550 1650
Connection ~ 2550 2050
Wire Wire Line
	2450 1850 2550 1850
Wire Wire Line
	2550 2400 2400 2400
Wire Wire Line
	2550 650  2550 850 
Wire Wire Line
	2550 2050 2450 2050
Connection ~ 2300 6850
Wire Wire Line
	2300 7000 2300 6850
Connection ~ 1850 6850
Wire Wire Line
	1850 7000 1850 6850
Connection ~ 2100 6550
Wire Wire Line
	2100 7000 2100 6550
Connection ~ 1600 6550
Wire Wire Line
	1600 7000 1600 6550
Connection ~ 2250 6850
Connection ~ 1400 6850
Connection ~ 1150 6850
Wire Wire Line
	900  6850 1150 6850
Connection ~ 2250 6550
Connection ~ 1400 6550
Connection ~ 1150 6550
Wire Wire Line
	900  6550 1150 6550
Connection ~ 1050 6200
Wire Wire Line
	900  6200 1050 6200
Connection ~ 1350 5900
Connection ~ 1050 5900
Wire Wire Line
	900  5900 1050 5900
Connection ~ 1600 5250
Wire Wire Line
	1100 5250 1600 5250
Wire Wire Line
	1600 5250 1600 5300
Wire Wire Line
	1100 5250 1100 5300
Connection ~ 1600 5600
Wire Wire Line
	1600 5550 1600 5600
Connection ~ 1100 5600
Wire Wire Line
	1100 5550 1100 5600
Wire Wire Line
	900  5600 1100 5600
Connection ~ 1600 5000
Connection ~ 1100 5000
Wire Wire Line
	900  5000 1100 5000
Connection ~ 9350 3200
Wire Wire Line
	9350 3400 9100 3400
Wire Wire Line
	9350 3200 9100 3200
Connection ~ 9350 3050
Connection ~ 9350 2250
Connection ~ 9350 2650
Wire Wire Line
	9350 2250 9350 2650
Connection ~ 8650 1250
Wire Wire Line
	8650 1050 8650 1250
Wire Wire Line
	8300 1050 8650 1050
Wire Wire Line
	7950 4750 7950 4900
Connection ~ 7950 4450
Wire Wire Line
	7950 5600 7950 5750
Connection ~ 7950 5300
Wire Wire Line
	10550 5750 10550 5900
Wire Wire Line
	10150 5750 10300 5750
Connection ~ 10550 5450
Connection ~ 10300 5450
Wire Wire Line
	10150 5450 10300 5450
Connection ~ 10550 5000
Wire Wire Line
	10300 5000 10550 5000
Wire Wire Line
	10300 4900 10300 5000
Wire Wire Line
	10550 4900 10550 5000
Connection ~ 10550 4600
Connection ~ 10300 4600
Wire Wire Line
	10150 4600 10300 4600
Wire Wire Line
	10550 3900 10550 4050
Wire Wire Line
	10150 3900 10300 3900
Connection ~ 10550 3600
Connection ~ 10300 3600
Wire Wire Line
	10150 3600 10300 3600
Wire Wire Line
	9250 5600 9250 5750
Connection ~ 9250 5300
Wire Wire Line
	9300 4750 9300 4900
Wire Wire Line
	8900 4750 9050 4750
Connection ~ 9300 4450
Connection ~ 9050 4450
Wire Wire Line
	8900 4450 9050 4450
Wire Wire Line
	9200 3900 9200 4050
Connection ~ 9200 3600
Connection ~ 9650 3050
Connection ~ 9050 3050
Wire Wire Line
	8800 3050 9050 3050
Connection ~ 9650 2650
Connection ~ 9050 2650
Wire Wire Line
	8800 2650 9050 2650
Connection ~ 9650 2250
Connection ~ 9050 2250
Wire Wire Line
	8800 2250 9050 2250
Connection ~ 8800 850 
Wire Wire Line
	8800 950  8800 850 
Connection ~ 9100 850 
Wire Wire Line
	9100 950  9100 850 
Connection ~ 9400 850 
Wire Wire Line
	9400 950  9400 850 
Wire Wire Line
	9700 850  9700 950 
Wire Wire Line
	8350 850  8800 850 
Connection ~ 10000 650 
Wire Wire Line
	10000 950  10000 650 
Wire Wire Line
	10300 650  10300 950 
Wire Wire Line
	8350 650  10000 650 
Wire Wire Line
	8350 1250 8650 1250
Wire Wire Line
	8350 1650 8700 1650
Connection ~ 5000 1050
Wire Wire Line
	4850 1050 5000 1050
Wire Wire Line
	5000 1150 5200 1150
Wire Wire Line
	5000 900  5000 1050
Wire Wire Line
	5200 900  5000 900 
Connection ~ 5750 1150
Wire Wire Line
	5500 1150 5750 1150
Connection ~ 5650 900 
Wire Wire Line
	5500 900  5650 900 
Connection ~ 1250 4350
Wire Wire Line
	1250 4450 1250 4350
Connection ~ 1250 3800
Wire Wire Line
	1250 3900 1250 3800
Connection ~ 1250 3300
Wire Wire Line
	1250 3400 1250 3300
Connection ~ 1250 2750
Wire Wire Line
	1250 2850 1250 2750
Connection ~ 1250 2200
Wire Wire Line
	1250 2300 1250 2200
Connection ~ 1250 1650
Wire Wire Line
	1250 1750 1250 1650
Connection ~ 1250 1150
Wire Wire Line
	1250 1250 1250 1150
Connection ~ 1250 600 
Wire Wire Line
	1250 700  1250 600 
Connection ~ 5850 1550
Wire Wire Line
	5950 1550 5850 1550
Wire Wire Line
	5950 1700 5950 1550
Connection ~ 7400 2850
Wire Wire Line
	7400 2950 7400 2850
Wire Wire Line
	7300 2950 7400 2950
Connection ~ 7400 2650
Wire Wire Line
	7400 2750 7400 2650
Wire Wire Line
	7300 2750 7400 2750
Wire Wire Line
	7500 2550 7300 2550
Wire Wire Line
	7500 2350 7300 2350
Connection ~ 7400 2050
Wire Wire Line
	7400 2150 7400 2050
Wire Wire Line
	7300 2150 7400 2150
Wire Wire Line
	7300 2050 7400 2050
Wire Wire Line
	4850 1600 5550 1600
Wire Wire Line
	4850 1600 4850 1050
Wire Wire Line
	7800 4450 7950 4450
$Comp
L Device:R R70
U 1 1 5AE980A5
P 3550 4400
F 0 "R70" V 3630 4400 50  0000 C CNN
F 1 "100k" V 3550 4400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3480 4400 50  0001 C CNN
F 3 "" H 3550 4400 50  0001 C CNN
	1    3550 4400
	-1   0    0    1   
$EndComp
Text GLabel 3550 4250 1    60   Input ~ 0
DGND
Text GLabel 3350 4550 0    60   Input ~ 0
+5V_in
Wire Wire Line
	3350 4550 3550 4550
Connection ~ 3550 4550
Text GLabel 3550 4750 0    60   Input ~ 0
DGND
$Comp
L Device:R R71
U 1 1 5AE98BBC
P 3400 4950
F 0 "R71" V 3480 4950 50  0000 C CNN
F 1 "190k" V 3400 4950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3330 4950 50  0001 C CNN
F 3 "" H 3400 4950 50  0001 C CNN
	1    3400 4950
	0    -1   -1   0   
$EndComp
Text GLabel 3250 4950 0    60   Input ~ 0
DGND
$Comp
L Device:R R73
U 1 1 5AE99EAF
P 4900 4900
F 0 "R73" V 4980 4900 50  0000 C CNN
F 1 "1M" V 4900 4900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4830 4900 50  0001 C CNN
F 3 "" H 4900 4900 50  0001 C CNN
	1    4900 4900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R74
U 1 1 5AE99FB3
P 4900 5200
F 0 "R74" V 4980 5200 50  0000 C CNN
F 1 "681k" V 4900 5200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4830 5200 50  0001 C CNN
F 3 "" H 4900 5200 50  0001 C CNN
	1    4900 5200
	1    0    0    -1  
$EndComp
Text GLabel 4800 5350 0    60   Input ~ 0
DGND
Wire Wire Line
	4750 4950 4750 5050
Wire Wire Line
	4750 5050 4900 5050
Wire Wire Line
	4800 5350 4900 5350
Wire Wire Line
	4750 4750 4900 4750
$Comp
L Device:C C70
U 1 1 5AE9A6AC
P 5100 4900
F 0 "C70" H 5125 5000 50  0000 L CNN
F 1 "100n" H 5125 4800 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5138 4750 50  0001 C CNN
F 3 "" H 5100 4900 50  0001 C CNN
	1    5100 4900
	1    0    0    -1  
$EndComp
Text GLabel 5100 5050 3    60   Input ~ 0
DGND
Connection ~ 4900 5050
Connection ~ 4900 4750
Connection ~ 5100 4750
$Comp
L PMOD-rescue:TracoTES1-0513 U71
U 1 1 5AE9F923
P 6100 4550
F 0 "U71" H 5650 4950 50  0000 L CNN
F 1 "TracoTES1-0513" H 5850 4950 50  0000 L CNN
F 2 ".pretty:DCDC-Conv_TRACO_TES-1SM" H 6100 4550 50  0001 C CNN
F 3 "" H 6100 4550 50  0001 C CNN
	1    6100 4550
	1    0    0    -1  
$EndComp
Text GLabel 6700 4550 2    60   Input ~ 0
DGND
Text GLabel 5100 4250 0    60   Input ~ 0
DGND
$Comp
L Device:C C71
U 1 1 5AE9FD9A
P 5350 4400
F 0 "C71" H 5375 4500 50  0000 L CNN
F 1 "4.7u" H 5375 4300 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5388 4250 50  0001 C CNN
F 3 "" H 5350 4400 50  0001 C CNN
	1    5350 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 4550 5350 4550
Wire Wire Line
	5500 4350 5500 4250
Wire Wire Line
	5500 4250 5350 4250
Connection ~ 5350 4250
Connection ~ 5350 4550
$Comp
L PMOD-rescue:LTC6994 U8
U 1 1 5AEA2D4C
P 4150 6000
F 0 "U8" H 3700 6400 50  0000 L CNN
F 1 "LTC6994" H 4300 6400 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TSOT-23-6_HandSoldering" H 4150 6000 50  0001 C CNN
F 3 "" H 4150 6000 50  0001 C CNN
	1    4150 6000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R80
U 1 1 5AEA2D52
P 3550 5650
F 0 "R80" V 3630 5650 50  0000 C CNN
F 1 "100k" V 3550 5650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3480 5650 50  0001 C CNN
F 3 "" H 3550 5650 50  0001 C CNN
	1    3550 5650
	-1   0    0    1   
$EndComp
Text GLabel 3550 5500 1    60   Input ~ 0
DGND
Text GLabel 3350 5800 0    60   Input ~ 0
+5V_in
Wire Wire Line
	3350 5800 3550 5800
Connection ~ 3550 5800
Text GLabel 3550 6000 0    60   Input ~ 0
DGND
$Comp
L Device:R R81
U 1 1 5AEA2D5D
P 3400 6200
F 0 "R81" V 3480 6200 50  0000 C CNN
F 1 "380k" V 3400 6200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3330 6200 50  0001 C CNN
F 3 "" H 3400 6200 50  0001 C CNN
	1    3400 6200
	0    -1   -1   0   
$EndComp
Text GLabel 3250 6200 0    60   Input ~ 0
DGND
$Comp
L Device:R R83
U 1 1 5AEA2D6A
P 4900 6150
F 0 "R83" V 4980 6150 50  0000 C CNN
F 1 "1M" V 4900 6150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4830 6150 50  0001 C CNN
F 3 "" H 4900 6150 50  0001 C CNN
	1    4900 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 6200 4750 6300
Wire Wire Line
	4750 6300 4900 6300
Wire Wire Line
	4800 6600 4900 6600
Wire Wire Line
	4750 6000 4900 6000
$Comp
L Device:C C80
U 1 1 5AEA2D7B
P 5100 6150
F 0 "C80" H 5125 6250 50  0000 L CNN
F 1 "100n" H 5125 6050 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5138 6000 50  0001 C CNN
F 3 "" H 5100 6150 50  0001 C CNN
	1    5100 6150
	1    0    0    -1  
$EndComp
Connection ~ 4900 6300
Connection ~ 4900 6000
Connection ~ 5100 6000
Text GLabel 6700 5600 2    60   Input ~ 0
DGND
Text GLabel 5100 5500 0    60   Input ~ 0
DGND
$Comp
L Device:C C81
U 1 1 5AEA2D8E
P 5350 5650
F 0 "C81" H 5375 5750 50  0000 L CNN
F 1 "4.7u" H 5375 5550 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5388 5500 50  0001 C CNN
F 3 "" H 5350 5650 50  0001 C CNN
	1    5350 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 5800 5350 5800
Wire Wire Line
	5500 5600 5500 5500
Wire Wire Line
	5500 5500 5350 5500
Connection ~ 5350 5500
Connection ~ 5350 5800
$Comp
L Device:R R84
U 1 1 5AEA41BC
P 4900 6450
F 0 "R84" V 4980 6450 50  0000 C CNN
F 1 "681k" V 4900 6450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4830 6450 50  0001 C CNN
F 3 "" H 4900 6450 50  0001 C CNN
	1    4900 6450
	1    0    0    -1  
$EndComp
Text GLabel 4800 6600 0    60   Input ~ 0
DGND
Text GLabel 5100 6300 3    60   Input ~ 0
DGND
$Comp
L PMOD-rescue:LTC6994 U9
U 1 1 5AEA41C6
P 4150 7200
F 0 "U9" H 3700 7600 50  0000 L CNN
F 1 "LTC6994" H 4300 7600 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TSOT-23-6_HandSoldering" H 4150 7200 50  0001 C CNN
F 3 "" H 4150 7200 50  0001 C CNN
	1    4150 7200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R90
U 1 1 5AEA41CC
P 3550 6850
F 0 "R90" V 3630 6850 50  0000 C CNN
F 1 "100k" V 3550 6850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3480 6850 50  0001 C CNN
F 3 "" H 3550 6850 50  0001 C CNN
	1    3550 6850
	-1   0    0    1   
$EndComp
Text GLabel 3550 6700 1    60   Input ~ 0
DGND
Text GLabel 3350 7000 0    60   Input ~ 0
+5V_in
Wire Wire Line
	3350 7000 3550 7000
Connection ~ 3550 7000
Text GLabel 3550 7200 0    60   Input ~ 0
DGND
$Comp
L Device:R R91
U 1 1 5AEA41D7
P 3400 7400
F 0 "R91" V 3480 7400 50  0000 C CNN
F 1 "570k" V 3400 7400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3330 7400 50  0001 C CNN
F 3 "" H 3400 7400 50  0001 C CNN
	1    3400 7400
	0    -1   -1   0   
$EndComp
Text GLabel 3250 7400 0    60   Input ~ 0
DGND
$Comp
L Device:R R93
U 1 1 5AEA41E4
P 4900 7350
F 0 "R93" V 4980 7350 50  0000 C CNN
F 1 "1M" V 4900 7350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4830 7350 50  0001 C CNN
F 3 "" H 4900 7350 50  0001 C CNN
	1    4900 7350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R94
U 1 1 5AEA41EA
P 4900 7650
F 0 "R94" V 4980 7650 50  0000 C CNN
F 1 "681k" V 4900 7650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4830 7650 50  0001 C CNN
F 3 "" H 4900 7650 50  0001 C CNN
	1    4900 7650
	1    0    0    -1  
$EndComp
Text GLabel 4800 7800 0    60   Input ~ 0
DGND
Wire Wire Line
	4750 7400 4750 7500
Wire Wire Line
	4750 7500 4900 7500
Wire Wire Line
	4800 7800 4900 7800
Wire Wire Line
	4750 7200 4900 7200
$Comp
L Device:C C90
U 1 1 5AEA41F5
P 5100 7350
F 0 "C90" H 5125 7450 50  0000 L CNN
F 1 "100n" H 5125 7250 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5138 7200 50  0001 C CNN
F 3 "" H 5100 7350 50  0001 C CNN
	1    5100 7350
	1    0    0    -1  
$EndComp
Text GLabel 5100 7500 3    60   Input ~ 0
DGND
Connection ~ 4900 7500
Connection ~ 4900 7200
Connection ~ 5100 7200
Text GLabel 3100 3250 2    60   Input ~ 0
+5V_in
$Comp
L PMOD-rescue:TracoTES1-0513 U81
U 1 1 5AEAB8C9
P 6100 5800
F 0 "U81" H 5650 6200 50  0000 L CNN
F 1 "TracoTES1-0513" H 5850 6200 50  0000 L CNN
F 2 ".pretty:DCDC-Conv_TRACO_TES-1SM" H 6100 5800 50  0001 C CNN
F 3 "" H 6100 5800 50  0001 C CNN
	1    6100 5800
	1    0    0    -1  
$EndComp
Text GLabel 6650 1700 1    60   Input ~ 0
VSSH
Text GLabel 5250 4750 2    60   Input ~ 0
+5V_in
Text GLabel 5250 6000 2    60   Input ~ 0
+5V_in
Text GLabel 5250 7200 2    60   Input ~ 0
+5V_in
Wire Wire Line
	5550 1600 5550 1700
Wire Wire Line
	9100 1250 9400 1250
Wire Wire Line
	9400 1250 9700 1250
Wire Wire Line
	10000 1250 10300 1250
Wire Wire Line
	9700 1250 10000 1250
Wire Wire Line
	2550 850  2550 1050
Wire Wire Line
	2550 1050 2550 1250
Wire Wire Line
	2550 1250 2550 1450
Wire Wire Line
	2550 1450 2550 1650
Wire Wire Line
	2550 1650 2550 1850
Wire Wire Line
	2550 1850 2550 2050
Wire Wire Line
	2550 2050 2550 2400
Wire Wire Line
	2300 6850 2500 6850
Wire Wire Line
	1850 6850 2250 6850
Wire Wire Line
	2100 6550 2000 6550
Wire Wire Line
	1600 6550 1700 6550
Wire Wire Line
	2250 6850 2300 6850
Wire Wire Line
	1400 6850 1850 6850
Wire Wire Line
	1150 6850 1400 6850
Wire Wire Line
	2250 6550 2100 6550
Wire Wire Line
	1400 6550 1600 6550
Wire Wire Line
	1150 6550 1400 6550
Wire Wire Line
	1050 6200 1350 6200
Wire Wire Line
	1350 5900 1700 5900
Wire Wire Line
	1050 5900 1350 5900
Wire Wire Line
	1600 5250 1900 5250
Wire Wire Line
	1600 5600 1900 5600
Wire Wire Line
	1100 5600 1600 5600
Wire Wire Line
	1600 5000 1900 5000
Wire Wire Line
	1100 5000 1600 5000
Wire Wire Line
	9350 3200 9350 3400
Wire Wire Line
	9350 3050 9350 3200
Wire Wire Line
	9350 3050 9650 3050
Wire Wire Line
	9350 2250 9650 2250
Wire Wire Line
	9350 2650 9350 3050
Wire Wire Line
	9350 2650 9650 2650
Wire Wire Line
	8650 1250 8800 1250
Wire Wire Line
	7950 4450 8150 4450
Wire Wire Line
	7950 5300 8150 5300
Wire Wire Line
	10550 5450 10750 5450
Wire Wire Line
	10300 5450 10550 5450
Wire Wire Line
	10550 5000 10550 5050
Wire Wire Line
	10550 4600 10750 4600
Wire Wire Line
	10300 4600 10550 4600
Wire Wire Line
	10550 3600 10750 3600
Wire Wire Line
	10300 3600 10550 3600
Wire Wire Line
	9250 5300 9450 5300
Wire Wire Line
	9300 4450 9500 4450
Wire Wire Line
	9050 4450 9300 4450
Wire Wire Line
	9200 3600 9400 3600
Wire Wire Line
	9650 3050 9900 3050
Wire Wire Line
	9050 3050 9350 3050
Wire Wire Line
	9650 2650 9900 2650
Wire Wire Line
	9050 2650 9350 2650
Wire Wire Line
	9650 2250 9900 2250
Wire Wire Line
	9050 2250 9350 2250
Wire Wire Line
	8800 850  9100 850 
Wire Wire Line
	9100 850  9400 850 
Wire Wire Line
	9400 850  9700 850 
Wire Wire Line
	10000 650  10300 650 
Wire Wire Line
	5000 1050 5000 1150
Wire Wire Line
	5750 1150 5750 1700
Wire Wire Line
	5650 900  5650 1700
Wire Wire Line
	1250 4350 1350 4350
Wire Wire Line
	1250 3800 1350 3800
Wire Wire Line
	1250 3300 1350 3300
Wire Wire Line
	1250 2750 1350 2750
Wire Wire Line
	1250 2200 1350 2200
Wire Wire Line
	1250 1650 1350 1650
Wire Wire Line
	1250 1150 1350 1150
Wire Wire Line
	1250 600  1350 600 
Wire Wire Line
	5850 1550 5850 1500
Wire Wire Line
	7400 2850 7500 2850
Wire Wire Line
	7400 2650 7500 2650
Wire Wire Line
	7400 2050 7500 2050
Wire Wire Line
	4900 4750 5100 4750
Wire Wire Line
	5100 4750 5250 4750
Wire Wire Line
	5350 4250 5100 4250
Wire Wire Line
	5350 4550 5500 4550
Wire Wire Line
	4900 6000 5100 6000
Wire Wire Line
	5100 6000 5250 6000
Wire Wire Line
	5350 5500 5100 5500
Wire Wire Line
	5350 5800 5500 5800
Wire Wire Line
	4900 7200 5100 7200
Wire Wire Line
	5100 7200 5250 7200
Wire Wire Line
	7550 5300 7950 5300
Wire Wire Line
	8850 5300 9250 5300
Wire Wire Line
	8800 3600 9200 3600
Wire Wire Line
	9650 2750 10200 2750
Wire Wire Line
	8500 2750 9050 2750
Wire Wire Line
	9650 2350 10200 2350
Wire Wire Line
	8500 2350 9050 2350
Wire Wire Line
	9650 1950 10200 1950
Wire Wire Line
	8500 1950 9050 1950
Wire Wire Line
	4750 7000 5250 7000
$EndSCHEMATC
