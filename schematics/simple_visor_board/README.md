# Simple matrix configuration

This is one of the simplest pre-built configuration. It uses cheap MAX7219 matrices


### Required and optional components
I sell some stuff, write me up if you want help :D
#### Required Minimum
- PCB from files in this folder. You can order on your own on site like JLCPCB or write me to ask for a board
- ESP32-WROOM-32 **VERY IMPORTANT** - You have to buy version with this exact pin configuration (Source: Wikipedia)!
Any other board will not fit into the PCB. You can order it on your local electronic dealer site or aliexpress
![ESP32_Espressif_ESP-WROOM-32_Dev_Board-1.jpg](images/ESP32_Espressif_ESP-WROOM-32_Dev_Board-1.jpg)
- CHIP 74AHCT245 or 74HCT245 in SOP20 format 3 state bus transceiver - you can buy it in your local electronics dealer site or aliexpress
- MAX7219 LED MATRIX - This is monochromatic LED Matrix based on MAX7219 chip. You can use any color! You can order it on your local electronic dealer site or aliexpress. In default "simple visor" configuration, you need 12 matrices: 2 for each eye and 4 for each mouth
- SD CARD HOLDER - Not really required, but strongly recommended. You have pinout for two options: One is to directly solder SD to MicroSD adapter to the board, second is to buy dedicated MicroSD memory module for Arduino, that looks like one on the photo. It's important that is **must** not have any 3v3 to 5v circuit, and it should directly connect to pins (eventually with some capacitors)
![micro_sd_connector.png](images/micro_sd_connector.png)
- Power source - Board is powered up by 5V power supply, you connect to pins POWER. There is also another set of pins called POWER_EXT, that connects power to MAX matrices. You can safely bridge them together to one power source. Recommended power for ESP32 is 5V0.5A, for MAX I recommend min 5V2A.
Altogether, the board should receive around 5V3A at minimum. You can use high-end powerbank,
or assemble your own power supply from some LiPO batteries and step-down converters.
- DIP Switch - if you use SDCARD, you need to be able to bridge and unbridge first pin on dip switch. I recommend buying minimum 3-pin dipswitch for switching, but you can also buy full 6-pin dip switch and use switches as options for software
- Resistors - at minimum, you need one 2K ohm resistor for SD card pullup. I recommend getting all 5 resistors (1x2kohm 4x1kohm) and solder them on the pins 2-6.
### Optional stuff you can connect to board
- WS leds - you can use WS2812 or similar in WS_LED pin. You need to bridge resistor 1 for WS to work
- Monochromatic LEDS - you can use standard leds in WS_LED pin. You need to add a resistor of around 100ohm to the resistor 1 place
- //TODO
