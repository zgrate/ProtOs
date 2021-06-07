EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Simple Max Board"
Date "2021-06-04"
Rev "1"
Comp "Z-Grate"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ESP32-DEVKITC-32D:ESP32-DEVKITC-32D U1
U 1 1 5F9B446B
P 3750 2850
F 0 "U1" H 3750 4017 50  0000 C CNN
F 1 "ESP32-DEVKITC-32D" H 3750 3926 50  0000 C CNN
F 2 "kicad-library-master:ESP32-DEVKITV1" H 3750 2850 50  0001 L BNN
F 3 "4" H 3750 2850 50  0001 L BNN
F 4 "Espressif Systems" H 3750 2850 50  0001 L BNN "Pole4"
	1    3750 2850
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC245 74HCT245
U 1 1 5F9B55E1
P 6350 3000
F 0 "74HCT245" H 6350 3981 50  0000 C CNN
F 1 "74HC245" H 6350 3890 50  0000 C CNN
F 2 "Package_SO:SO-20_12.8x7.5mm_P1.27mm" H 6350 3000 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC245" H 6350 3000 50  0001 C CNN
	1    6350 3000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female POWER1
U 1 1 5F9B827D
P 2000 900
F 0 "POWER1" V 1938 712 50  0000 R CNN
F 1 "Conn_01x02_Female" V 1847 712 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2000 900 50  0001 C CNN
F 3 "~" H 2000 900 50  0001 C CNN
	1    2000 900 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2950 3750 2700 3750
Wire Wire Line
	2700 3750 2700 1550
Wire Wire Line
	2700 1100 2100 1100
Text Label 2950 2550 2    50   ~ 0
WSLED
Text Label 2950 2650 2    50   ~ 0
IR_IN
Text Label 2950 2750 2    50   ~ 0
USB_D+
Text Label 2950 2850 2    50   ~ 0
USB_D-
Text Label 2950 2950 2    50   ~ 0
IR_OUT
Text Label 2950 3050 2    50   ~ 0
SD_CLK
Text Label 2950 3150 2    50   ~ 0
ADD_EXT
Text Label 2950 3350 2    50   ~ 0
CONN_EXT
Text Label 4550 2050 0    50   ~ 0
MAX_OUT_3V3
Text Label 4550 2150 0    50   ~ 0
SCL
Text Label 4550 2450 0    50   ~ 0
SDA
Text Label 4550 2750 0    50   ~ 0
MAX_CLK_3v3
Text Label 4550 2850 0    50   ~ 0
MAX_CS_1_3V3
Text Label 4550 3350 0    50   ~ 0
SD_D0
Text Label 4550 3450 0    50   ~ 0
CMD
Text Label 4550 2950 0    50   ~ 0
MAX_CS_2_3V3
Text Label 2950 1950 2    50   ~ 0
3v3
Text Label 4550 1950 0    50   ~ 0
GND
Text Label 2950 3750 2    50   ~ 0
ESP_5V
Text Label 2950 2150 2    50   ~ 0
SW_1
Text Label 2950 2250 2    50   ~ 0
SW_2
Text Label 2950 2350 2    50   ~ 0
SW_3
Wire Wire Line
	4550 2050 5550 2050
Wire Wire Line
	5550 2050 5550 2500
Wire Wire Line
	5550 2500 5650 2500
Text Label 6850 2500 0    50   ~ 0
MAX_OUT_5V_1
Text Label 6850 2700 0    50   ~ 0
MAX_CLK_5V_1
Text Label 6850 2900 0    50   ~ 0
MAX_CS_2_5V
Text Label 6850 3200 0    50   ~ 0
WS\LED
Wire Wire Line
	4550 2750 4850 2750
Wire Wire Line
	3300 1850 3300 2550
Wire Wire Line
	3300 2550 2950 2550
$Comp
L Connector:SD_Card SD_CARD1
U 1 1 5F9D6AD2
P 7000 4650
F 0 "SD_CARD1" H 7000 5315 50  0000 C CNN
F 1 "SD_Card" H 7000 5224 50  0000 C CNN
F 2 "kicad-library-master:PinHeader_1x09_P2.54mm_Vertical_SD" H 7000 4650 50  0001 C CNN
F 3 "http://portal.fciconnect.com/Comergent//fci/drawing/10067847.pdf" H 7000 4650 50  0001 C CNN
	1    7000 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 4450 5050 4450
Wire Wire Line
	5050 4450 5050 4000
Wire Wire Line
	5050 3450 4550 3450
Wire Wire Line
	6100 4950 4900 4950
Wire Wire Line
	4900 4950 4900 4200
Wire Wire Line
	4900 3350 4550 3350
Wire Wire Line
	2950 3050 3850 3050
Wire Wire Line
	3850 3050 3850 4750
Wire Wire Line
	3850 4750 5250 4750
Wire Wire Line
	6100 4550 5900 4550
Connection ~ 3350 3250
Wire Wire Line
	3350 3250 3350 4550
Wire Wire Line
	6100 4850 5900 4850
Wire Wire Line
	5900 4850 5900 4550
Connection ~ 5900 4550
Wire Wire Line
	5900 4550 5850 4550
Wire Wire Line
	6100 4650 4650 4650
Wire Wire Line
	3450 4650 3450 1950
Wire Wire Line
	6350 2200 6350 1550
Wire Wire Line
	6350 1550 2700 1550
Connection ~ 2700 1550
Wire Wire Line
	2450 3250 2950 3250
Connection ~ 2950 3250
$Comp
L Connector:Conn_01x02_Female POWER_MAX1
U 1 1 5F9EC785
P 7950 1250
F 0 "POWER_MAX1" V 7888 1062 50  0000 R CNN
F 1 "Conn_01x02_Female" V 7797 1062 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7950 1250 50  0001 C CNN
F 3 "~" H 7950 1250 50  0001 C CNN
	1    7950 1250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8050 1450 8050 2250
Wire Wire Line
	8050 2250 7900 2250
Wire Wire Line
	7900 2250 7900 2450
Wire Wire Line
	7850 2550 7850 2100
Wire Wire Line
	7850 2000 7950 2000
Wire Wire Line
	7950 2000 7950 1450
Wire Wire Line
	7850 2550 7900 2550
Wire Wire Line
	5850 3400 5700 3400
Wire Wire Line
	5700 3400 5700 2250
Wire Wire Line
	5700 2250 6350 2250
Wire Wire Line
	6350 2250 6350 2200
Connection ~ 6350 2200
Wire Wire Line
	5850 3500 5850 3800
Wire Wire Line
	5850 3800 6350 3800
Wire Wire Line
	5850 3800 5850 4550
Connection ~ 5850 3800
Connection ~ 5850 4550
Wire Wire Line
	5850 4550 5500 4550
$Comp
L Connector:Conn_01x03_Female WS_LED_CONN1
U 1 1 5F9FDD20
P 8100 3350
F 0 "WS_LED_CONN1" H 8128 3376 50  0000 L CNN
F 1 "Conn_01x03_Female" H 8128 3285 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 8100 3350 50  0001 C CNN
F 3 "~" H 8100 3350 50  0001 C CNN
	1    8100 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 3250 7900 3100
Wire Wire Line
	7900 3100 8150 3100
Wire Wire Line
	8150 3100 8150 2400
Wire Wire Line
	8150 2250 8050 2250
Connection ~ 8050 2250
$Comp
L Device:R RESISTOR_LED1
U 1 1 5FA0353D
P 7450 3350
F 0 "RESISTOR_LED1" V 7243 3350 50  0000 C CNN
F 1 "R" V 7334 3350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7380 3350 50  0001 C CNN
F 3 "~" H 7450 3350 50  0001 C CNN
	1    7450 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	7900 3350 7600 3350
Wire Wire Line
	7900 3450 7150 3450
Wire Wire Line
	6350 3450 6350 3800
Connection ~ 6350 3800
Wire Wire Line
	2000 1100 2000 1300
Wire Wire Line
	2000 1300 2450 1300
$Comp
L Connector_Generic:Conn_02x04_Counter_Clockwise EXTERNAL_BOARD_HEADER1
U 1 1 60130108
P 1750 2150
F 0 "EXTERNAL_BOARD_HEADER1" H 1800 2467 50  0000 C CNN
F 1 "Conn_02x04_Counter_Clockwise" H 1800 2376 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x04_P2.54mm_Vertical" H 1750 2150 50  0001 C CNN
F 3 "~" H 1750 2150 50  0001 C CNN
	1    1750 2150
	1    0    0    -1  
$EndComp
Text Label 1550 2050 2    50   ~ 0
GND
Text Label 1550 2150 2    50   ~ 0
3v3
Text Label 1550 2250 2    50   ~ 0
SCL
Text Label 1550 2350 2    50   ~ 0
SDA
Text Label 2050 2050 0    50   ~ 0
IR_REC
Text Label 2050 2150 0    50   ~ 0
IR_TRANS
Text Label 2050 2250 0    50   ~ 0
CONTACT
Text Label 2050 2350 0    50   ~ 0
LASER
Wire Wire Line
	2950 2650 2550 2650
Wire Wire Line
	2550 2650 2550 2050
Wire Wire Line
	2550 2050 2050 2050
Wire Wire Line
	2950 2950 2400 2950
Wire Wire Line
	2400 2950 2400 2150
Wire Wire Line
	2400 2150 2050 2150
Wire Wire Line
	3650 2150 3650 1900
Wire Wire Line
	3650 1900 1700 1900
Wire Wire Line
	1700 1900 1700 2250
Wire Wire Line
	1700 2250 1550 2250
Wire Wire Line
	5150 1850 3300 1850
Wire Wire Line
	2450 1300 2450 1350
Wire Wire Line
	3600 2450 3600 2000
Wire Wire Line
	3600 2000 1750 2000
Wire Wire Line
	1750 2000 1750 2350
Wire Wire Line
	1750 2350 1550 2350
Wire Wire Line
	2950 3350 2300 3350
Wire Wire Line
	2300 3350 2300 2250
Wire Wire Line
	2300 2250 2050 2250
Wire Wire Line
	2950 3150 2200 3150
Wire Wire Line
	2200 3150 2200 2350
Wire Wire Line
	2200 2350 2050 2350
Wire Wire Line
	4550 2550 4100 2550
Wire Wire Line
	3650 2550 3650 3250
Wire Wire Line
	3650 3250 3350 3250
Text Label 4550 2550 0    50   ~ 0
GND
Wire Wire Line
	2950 3250 3350 3250
Wire Wire Line
	4550 3350 3500 3350
Wire Wire Line
	3500 3350 3500 1300
Wire Wire Line
	3500 1300 3650 1300
Connection ~ 4550 3350
$Comp
L Switch:SW_DIP_x06 SW1
U 1 1 601A7D8B
P 3950 1000
F 0 "SW1" V 3996 670 50  0000 R CNN
F 1 "SW_DIP_x06" V 3950 1250 50  0000 R CNN
F 2 "Button_Switch_THT:SW_DIP_SPSTx06_Slide_9.78x17.42mm_W7.62mm_P2.54mm" H 3950 1000 50  0001 C CNN
F 3 "~" H 3950 1000 50  0001 C CNN
	1    3950 1000
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2K1
U 1 1 601B90DB
P 3400 1000
F 0 "R2K1" V 3400 900 50  0000 L CNN
F 1 "R" H 3300 1050 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3330 1000 50  0001 C CNN
F 3 "~" H 3400 1000 50  0001 C CNN
	1    3400 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 700  3650 700 
Wire Wire Line
	3400 1700 2950 1700
Wire Wire Line
	2950 1700 2950 1950
Wire Wire Line
	3400 1150 3400 1400
Text Label 2950 2450 2    50   ~ 0
SW_4
Wire Wire Line
	4550 2150 3650 2150
Wire Wire Line
	3550 2150 3550 2100
Wire Wire Line
	3550 2100 3950 2100
Wire Wire Line
	3950 2100 3950 1300
Wire Wire Line
	4050 1300 4050 2250
Wire Wire Line
	4050 2250 3200 2250
Wire Wire Line
	4150 2350 4150 1300
$Comp
L Device:R R1K3
U 1 1 6019BAC7
P 4750 850
F 0 "R1K3" V 4750 750 50  0000 L CNN
F 1 "R" H 4400 900 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4680 850 50  0001 C CNN
F 3 "~" H 4750 850 50  0001 C CNN
	1    4750 850 
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1K2
U 1 1 60196E9A
P 4750 950
F 0 "R1K2" V 4750 850 50  0000 L CNN
F 1 "R" H 4500 1000 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4680 950 50  0001 C CNN
F 3 "~" H 4750 950 50  0001 C CNN
	1    4750 950 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4150 1300 4450 1300
Wire Wire Line
	4450 1300 4450 1050
Wire Wire Line
	4450 1050 4600 1050
Connection ~ 4150 1300
Wire Wire Line
	4600 950  4400 950 
Wire Wire Line
	4400 950  4400 1200
Wire Wire Line
	4400 1200 4050 1200
Wire Wire Line
	4050 1200 4050 1300
Connection ~ 4050 1300
Wire Wire Line
	3950 1300 3950 1100
Wire Wire Line
	3950 1100 4350 1100
Wire Wire Line
	4350 1100 4350 850 
Wire Wire Line
	4350 850  4600 850 
Connection ~ 3950 1300
$Comp
L Device:R R1K4
U 1 1 60227160
P 4750 750
F 0 "R1K4" V 4750 650 50  0000 L CNN
F 1 "R" H 4300 800 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4680 750 50  0001 C CNN
F 3 "~" H 4750 750 50  0001 C CNN
	1    4750 750 
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1K1
U 1 1 60194A2B
P 4750 1050
F 0 "R1K1" V 4750 950 50  0000 L CNN
F 1 "R" H 4600 1100 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4680 1050 50  0001 C CNN
F 3 "~" H 4750 1050 50  0001 C CNN
	1    4750 1050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3850 950  4300 950 
Wire Wire Line
	4300 950  4300 750 
Wire Wire Line
	4300 750  4600 750 
Wire Wire Line
	3850 950  3850 1300
Connection ~ 3850 1300
Wire Wire Line
	3850 1300 3850 2050
$Comp
L Connector:Conn_01x02_Female BUZZER1
U 1 1 6023D864
P 2750 750
F 0 "BUZZER1" V 2688 562 50  0000 R CNN
F 1 "Conn_01x02_Female" V 2597 562 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2750 750 50  0001 C CNN
F 3 "~" H 2750 750 50  0001 C CNN
	1    2750 750 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2850 950  2850 650 
Wire Wire Line
	2850 650  3750 650 
Wire Wire Line
	3750 650  3750 700 
Wire Wire Line
	3400 700  3400 850 
Wire Wire Line
	2950 2450 3250 2450
Wire Wire Line
	3250 2450 3250 2350
Wire Wire Line
	3250 2350 4150 2350
Wire Wire Line
	3200 2350 3200 2250
Wire Wire Line
	2950 2350 3200 2350
Wire Wire Line
	3150 2250 3150 2150
Wire Wire Line
	3150 2150 3550 2150
Wire Wire Line
	2950 2250 3150 2250
Wire Wire Line
	2950 2150 3100 2150
Wire Wire Line
	3100 2150 3100 2050
Wire Wire Line
	3100 2050 3850 2050
Text Label 4550 3250 0    50   ~ 0
UNAVAILABLE
Text Label 4550 2650 0    50   ~ 0
BUZZER
Text Label 4550 3050 0    50   ~ 0
EMPTY1
Text Label 4550 3150 0    50   ~ 0
EMPTY2
Wire Wire Line
	3600 2450 4550 2450
Text Label 4550 2250 0    50   ~ 0
EMPTY3
Text Label 4550 2350 0    50   ~ 0
BUTTON
Wire Wire Line
	4900 750  4900 850 
Connection ~ 4900 850 
Wire Wire Line
	4900 850  4900 950 
Connection ~ 4900 950 
Wire Wire Line
	4900 950  4900 1050
Wire Wire Line
	3400 1400 4900 1400
Wire Wire Line
	4900 1400 4900 1050
Connection ~ 3400 1400
Wire Wire Line
	3400 1400 3400 1450
Connection ~ 4900 1050
Wire Wire Line
	3750 1300 3750 2650
Wire Wire Line
	3750 2650 4550 2650
$Comp
L Connector:Conn_01x02_Female BUTTON1
U 1 1 602C5A67
P 5350 950
F 0 "BUTTON1" V 5288 762 50  0000 R CNN
F 1 "Conn_01x02_Female" V 5197 762 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5350 950 50  0001 C CNN
F 3 "~" H 5350 950 50  0001 C CNN
	1    5350 950 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4550 2350 5350 2350
Wire Wire Line
	5350 2350 5350 1150
Wire Wire Line
	5450 1150 5450 1450
Wire Wire Line
	5450 1450 3400 1450
Connection ~ 3400 1450
Wire Wire Line
	3400 1450 3400 1700
Wire Wire Line
	2700 1100 2700 1550
Wire Wire Line
	2750 1350 2450 1350
Connection ~ 2450 1350
Wire Wire Line
	2450 1350 2450 3000
Wire Wire Line
	2750 950  2750 1000
Text Label 2950 3250 2    50   ~ 0
GND
Wire Wire Line
	4150 700  4050 700 
Connection ~ 3950 700 
Wire Wire Line
	3950 700  3850 700 
Connection ~ 4050 700 
Wire Wire Line
	4050 700  3950 700 
Wire Wire Line
	3850 700  3850 550 
Wire Wire Line
	3850 550  2650 550 
Wire Wire Line
	2650 550  2650 1000
Wire Wire Line
	2650 1000 2750 1000
Connection ~ 3850 700 
Connection ~ 2750 1000
Wire Wire Line
	2750 1000 2750 1350
Wire Wire Line
	4550 1950 5400 1950
Wire Wire Line
	5400 1950 5400 1900
Wire Wire Line
	5500 1900 5450 1900
Wire Wire Line
	5450 1900 5450 3050
Wire Wire Line
	5450 3050 4550 3050
Wire Wire Line
	5600 1900 5600 3100
Wire Wire Line
	5600 3150 4550 3150
Wire Wire Line
	4550 2250 5650 2250
Wire Wire Line
	5650 2250 5650 1900
Wire Wire Line
	5650 1900 5700 1900
Wire Wire Line
	5950 1900 5950 1400
Wire Wire Line
	5950 1400 4900 1400
Connection ~ 4900 1400
Connection ~ 2950 1950
Wire Wire Line
	2950 1950 3450 1950
Wire Wire Line
	4550 1950 4100 1950
Wire Wire Line
	4100 1950 4100 2550
Connection ~ 4550 1950
Connection ~ 4100 2550
Wire Wire Line
	4100 2550 3650 2550
Wire Wire Line
	1550 2050 1550 1300
Wire Wire Line
	1550 1300 2000 1300
Connection ~ 2000 1300
Wire Wire Line
	1550 2150 1650 2150
Wire Wire Line
	1650 2150 1650 1700
Wire Wire Line
	1650 1700 2950 1700
Connection ~ 2950 1700
Text Label 6850 2800 0    50   ~ 0
MAX2_OUT_5V_2
Text Label 6850 3000 0    50   ~ 0
MAX2_CLK_5V_2
$Comp
L Connector:Conn_01x05_Female MAX1
U 1 1 603D0F17
P 8100 2650
F 0 "MAX1" H 8128 2676 50  0000 L CNN
F 1 "Conn_01x05_Female" H 8128 2585 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 8100 2650 50  0001 C CNN
F 3 "~" H 8100 2650 50  0001 C CNN
	1    8100 2650
	1    0    0    -1  
$EndComp
Text Label 7900 2450 2    50   ~ 0
VCC
Text Label 7900 2550 2    50   ~ 0
GND
Text Label 7900 2650 2    50   ~ 0
DIN1
Text Label 7900 2750 2    50   ~ 0
CS1
Text Label 7900 2850 2    50   ~ 0
CLK1
$Comp
L Connector:Conn_01x05_Female MAX2
U 1 1 603DCDEE
P 8650 3000
F 0 "MAX2" H 8678 3026 50  0000 L CNN
F 1 "Conn_01x05_Female" H 8678 2935 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 8650 3000 50  0001 C CNN
F 3 "~" H 8650 3000 50  0001 C CNN
	1    8650 3000
	1    0    0    -1  
$EndComp
Text Label 8450 2800 2    50   ~ 0
VCC
Text Label 8450 2900 2    50   ~ 0
GND
Text Label 8450 3000 2    50   ~ 0
DIN2
Text Label 8450 3100 2    50   ~ 0
CS2
Text Label 8450 3200 2    50   ~ 0
CLK2
Text Label 6850 2600 0    50   ~ 0
MAX_CS_1_5V
Wire Wire Line
	6850 3200 6850 3350
Wire Wire Line
	6850 3350 7300 3350
Wire Wire Line
	5150 3200 5850 3200
Wire Wire Line
	5150 1850 5150 3200
Wire Wire Line
	4550 2950 5750 2950
Wire Wire Line
	5850 2900 5750 2900
Wire Wire Line
	5850 2800 5650 2800
Wire Wire Line
	5650 2800 5650 2500
Connection ~ 5650 2500
Wire Wire Line
	5650 2500 5850 2500
Wire Wire Line
	6850 2500 7750 2500
Wire Wire Line
	7750 2500 7750 2650
Wire Wire Line
	7750 2650 7900 2650
Wire Wire Line
	6850 2600 7650 2600
Wire Wire Line
	7650 2600 7650 2750
Wire Wire Line
	7650 2750 7900 2750
Wire Wire Line
	6850 2700 7600 2700
Wire Wire Line
	7600 2700 7600 2850
Wire Wire Line
	7600 2850 7900 2850
Wire Wire Line
	4850 2750 4850 2700
Wire Wire Line
	4850 2700 5800 2700
Wire Wire Line
	5350 2850 5350 2600
Wire Wire Line
	5350 2600 5850 2600
Wire Wire Line
	4550 2850 5350 2850
Wire Wire Line
	6850 2800 7550 2800
Wire Wire Line
	7550 2800 7550 3000
Wire Wire Line
	7550 3000 8450 3000
Wire Wire Line
	6850 2900 7450 2900
Wire Wire Line
	7450 2900 7450 3050
Wire Wire Line
	7450 3050 8450 3050
Wire Wire Line
	8450 3050 8450 3100
Wire Wire Line
	8450 3200 7350 3200
Wire Wire Line
	7350 3200 7350 3000
Wire Wire Line
	7350 3000 6850 3000
Wire Wire Line
	5750 2950 5750 2900
Wire Wire Line
	5850 3000 5800 3000
Wire Wire Line
	5800 3000 5800 2700
Connection ~ 5800 2700
Wire Wire Line
	5800 2700 5850 2700
Wire Wire Line
	7850 2100 7150 2100
Wire Wire Line
	7150 2100 7150 3450
Connection ~ 7850 2100
Wire Wire Line
	7850 2100 7850 2000
Connection ~ 7150 3450
Wire Wire Line
	7150 3450 6350 3450
Wire Wire Line
	8150 2400 8450 2400
Wire Wire Line
	8450 2400 8450 2800
Connection ~ 8150 2400
Wire Wire Line
	8150 2400 8150 2250
Wire Wire Line
	8450 2900 8300 2900
Wire Wire Line
	8300 2900 8300 2550
Wire Wire Line
	8300 2550 7900 2550
Connection ~ 7900 2550
$Comp
L Connector:Conn_01x06_Female EMPTY_SLOTS1
U 1 1 60505E76
P 5600 1700
F 0 "EMPTY_SLOTS1" V 5538 1312 50  0000 R CNN
F 1 "Conn_01x06_Female" V 5447 1312 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 5600 1700 50  0001 C CNN
F 3 "~" H 5600 1700 50  0001 C CNN
	1    5600 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5900 1900 5950 1900
Wire Wire Line
	5800 1900 5800 2100
Wire Wire Line
	5800 2100 6500 2100
Wire Wire Line
	6500 2100 6500 3100
Wire Wire Line
	6500 3100 6850 3100
Wire Wire Line
	5850 3100 5600 3100
Connection ~ 5600 3100
Wire Wire Line
	5600 3100 5600 3150
Text Label 6850 3100 0    50   ~ 0
EMPTY2_5v
$Comp
L Connector:Conn_01x03_Female USB_FAN1
U 1 1 6052775A
P 1550 2900
F 0 "USB_FAN1" H 1442 2575 50  0000 C CNN
F 1 "Conn_01x03_Female" H 1442 2666 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical" H 1550 2900 50  0001 C CNN
F 3 "~" H 1550 2900 50  0001 C CNN
	1    1550 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	1750 3000 2450 3000
Connection ~ 2450 3000
Wire Wire Line
	2450 3000 2450 3250
Wire Wire Line
	2000 2900 1750 2900
Wire Wire Line
	2950 2850 2000 2850
Wire Wire Line
	2000 2850 2000 2900
Wire Wire Line
	1750 2750 1750 2800
Wire Wire Line
	1750 2750 2950 2750
$Comp
L Connector:Conn_01x06_Female MICRO_SD1
U 1 1 60113E83
P 5700 4000
F 0 "MICRO_SD1" H 5728 3976 50  0000 L CNN
F 1 "Conn_01x06_Female" H 5728 3885 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x06_P2.54mm_Vertical" H 5700 4000 50  0001 C CNN
F 3 "~" H 5700 4000 50  0001 C CNN
	1    5700 4000
	1    0    0    -1  
$EndComp
Text Label 5500 3800 2    50   ~ 0
SD_3v3
Text Label 5500 3900 2    50   ~ 0
SD_CS
Text Label 5500 4000 2    50   ~ 0
SD_MOSI
Text Label 5500 4100 2    50   ~ 0
SD_CLK
Text Label 5500 4200 2    50   ~ 0
SD_MISO
Text Label 5500 4300 2    50   ~ 0
SD_GND
Wire Wire Line
	5500 3800 4650 3800
Wire Wire Line
	4650 3800 4650 4650
Connection ~ 4650 4650
Wire Wire Line
	4650 4650 3450 4650
Wire Wire Line
	5500 4300 5500 4550
Connection ~ 5500 4550
Wire Wire Line
	5500 4550 3350 4550
Wire Wire Line
	5500 4000 5050 4000
Connection ~ 5050 4000
Wire Wire Line
	5050 4000 5050 3450
Wire Wire Line
	5500 4100 5250 4100
Wire Wire Line
	5250 4100 5250 4750
Connection ~ 5250 4750
Wire Wire Line
	5250 4750 6100 4750
Wire Wire Line
	5500 4200 4900 4200
Connection ~ 4900 4200
Wire Wire Line
	4900 4200 4900 3350
$EndSCHEMATC
