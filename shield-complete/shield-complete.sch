EESchema Schematic File Version 4
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
L wemos_mini:WeMos_mini U?
U 1 1 5CD61191
P 8550 2000
F 0 "U?" H 8550 2743 60  0000 C CNN
F 1 "WeMos_mini" H 8550 2637 60  0000 C CNN
F 2 "wemos-d1-mini:wemos-d1-mini-with-pin-header-and-connector" H 9100 1300 60  0001 C CNN
F 3 "http://www.wemos.cc/Products/d1_mini.html" H 8550 2531 60  0000 C CNN
	1    8550 2000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Female J1
U 1 1 5CD61198
P 1750 1800
F 0 "J1" H 1777 1826 50  0000 L CNN
F 1 "Conn_01x03_Female" H 1777 1735 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 1750 1800 50  0001 C CNN
F 3 "~" H 1750 1800 50  0001 C CNN
	1    1750 1800
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 5CD6119F
P 3350 1850
F 0 "J2" H 3456 2028 50  0000 C CNN
F 1 "Conn_01x02_Male" H 3456 1937 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x02_Pitch2.54mm" H 3350 1850 50  0001 C CNN
F 3 "~" H 3350 1850 50  0001 C CNN
	1    3350 1850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 5CD611A6
P 4800 1850
F 0 "J3" H 4906 2128 50  0000 C CNN
F 1 "Conn_01x04_Male" H 4906 2037 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04_Pitch2.54mm" H 4800 1850 50  0001 C CNN
F 3 "~" H 4800 1850 50  0001 C CNN
	1    4800 1850
	1    0    0    -1  
$EndComp
$Comp
L device:R R1
U 1 1 5CD611AD
P 3400 2750
F 0 "R1" H 3470 2796 50  0000 L CNN
F 1 "R" H 3470 2705 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3330 2750 50  0001 C CNN
F 3 "~" H 3400 2750 50  0001 C CNN
	1    3400 2750
	1    0    0    -1  
$EndComp
$Comp
L device:R R2
U 1 1 5CD611B4
P 5700 2850
F 0 "R2" H 5770 2896 50  0000 L CNN
F 1 "R" H 5770 2805 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5630 2850 50  0001 C CNN
F 3 "~" H 5700 2850 50  0001 C CNN
	1    5700 2850
	1    0    0    -1  
$EndComp
$Comp
L device:R R4
U 1 1 5CD611BB
P 6150 2850
F 0 "R4" H 6220 2896 50  0000 L CNN
F 1 "R" H 6220 2805 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6080 2850 50  0001 C CNN
F 3 "~" H 6150 2850 50  0001 C CNN
	1    6150 2850
	1    0    0    -1  
$EndComp
$Comp
L device:R R3
U 1 1 5CD611C2
P 5700 3500
F 0 "R3" H 5770 3546 50  0000 L CNN
F 1 "R" H 5770 3455 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5630 3500 50  0001 C CNN
F 3 "~" H 5700 3500 50  0001 C CNN
	1    5700 3500
	1    0    0    -1  
$EndComp
$Comp
L device:R R5
U 1 1 5CD611C9
P 6150 3500
F 0 "R5" H 6220 3546 50  0000 L CNN
F 1 "R" H 6220 3455 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6080 3500 50  0001 C CNN
F 3 "~" H 6150 3500 50  0001 C CNN
	1    6150 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J4
U 1 1 5CD611D0
P 6000 1900
F 0 "J4" H 6080 1892 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 6080 1801 50  0000 L CNN
F 2 "TerminalBlocks_Phoenix:TerminalBlock_Phoenix_MPT-2.54mm_2pol" H 6000 1900 50  0001 C CNN
F 3 "~" H 6000 1900 50  0001 C CNN
	1    6000 1900
	1    0    0    -1  
$EndComp
$EndSCHEMATC
