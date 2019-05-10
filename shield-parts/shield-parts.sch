EESchema Schematic File Version 4
LIBS:shield-parts-cache
EELAYER 26 0
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
L wemos_mini:WeMos_mini U1
U 1 1 5CD60680
P 8400 2000
F 0 "U1" H 8400 2743 60  0000 C CNN
F 1 "WeMos_mini" H 8400 2637 60  0000 C CNN
F 2 "wemos-d1-mini:wemos-d1-mini-with-pin-header-and-connector" H 8950 1300 60  0001 C CNN
F 3 "http://www.wemos.cc/Products/d1_mini.html" H 8400 2531 60  0000 C CNN
	1    8400 2000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Female J1
U 1 1 5CD608CB
P 1600 1800
F 0 "J1" H 1627 1826 50  0000 L CNN
F 1 "Conn_01x03_Female" H 1627 1735 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 1600 1800 50  0001 C CNN
F 3 "~" H 1600 1800 50  0001 C CNN
	1    1600 1800
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 5CD60B2A
P 3200 1850
F 0 "J2" H 3306 2028 50  0000 C CNN
F 1 "Conn_01x02_Male" H 3306 1937 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x02_Pitch2.54mm" H 3200 1850 50  0001 C CNN
F 3 "~" H 3200 1850 50  0001 C CNN
	1    3200 1850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 5CD60D13
P 4650 1850
F 0 "J3" H 4756 2128 50  0000 C CNN
F 1 "Conn_01x04_Male" H 4756 2037 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04_Pitch2.54mm" H 4650 1850 50  0001 C CNN
F 3 "~" H 4650 1850 50  0001 C CNN
	1    4650 1850
	1    0    0    -1  
$EndComp
$Comp
L device:R R1
U 1 1 5CD60E87
P 3250 2750
F 0 "R1" H 3320 2796 50  0000 L CNN
F 1 "R" H 3320 2705 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3180 2750 50  0001 C CNN
F 3 "~" H 3250 2750 50  0001 C CNN
	1    3250 2750
	1    0    0    -1  
$EndComp
$Comp
L device:R R2
U 1 1 5CD60FB4
P 5550 2850
F 0 "R2" H 5620 2896 50  0000 L CNN
F 1 "R" H 5620 2805 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5480 2850 50  0001 C CNN
F 3 "~" H 5550 2850 50  0001 C CNN
	1    5550 2850
	1    0    0    -1  
$EndComp
$Comp
L device:R R4
U 1 1 5CD610A4
P 6000 2850
F 0 "R4" H 6070 2896 50  0000 L CNN
F 1 "R" H 6070 2805 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5930 2850 50  0001 C CNN
F 3 "~" H 6000 2850 50  0001 C CNN
	1    6000 2850
	1    0    0    -1  
$EndComp
$Comp
L device:R R3
U 1 1 5CD610FF
P 5550 3500
F 0 "R3" H 5620 3546 50  0000 L CNN
F 1 "R" H 5620 3455 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5480 3500 50  0001 C CNN
F 3 "~" H 5550 3500 50  0001 C CNN
	1    5550 3500
	1    0    0    -1  
$EndComp
$Comp
L device:R R5
U 1 1 5CD6122D
P 6000 3500
F 0 "R5" H 6070 3546 50  0000 L CNN
F 1 "R" H 6070 3455 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5930 3500 50  0001 C CNN
F 3 "~" H 6000 3500 50  0001 C CNN
	1    6000 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J4
U 1 1 5CD613D0
P 5850 1900
F 0 "J4" H 5930 1892 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 5930 1801 50  0000 L CNN
F 2 "TerminalBlocks_Phoenix:TerminalBlock_Phoenix_MPT-2.54mm_2pol" H 5850 1900 50  0001 C CNN
F 3 "~" H 5850 1900 50  0001 C CNN
	1    5850 1900
	1    0    0    -1  
$EndComp
$EndSCHEMATC
