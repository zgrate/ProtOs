//
// Created by dzing on 29/01/2021.
//

#ifndef VISORV3_CONSTANTSANDSETTINGS_H
#define VISORV3_CONSTANTSANDSETTINGS_H

//MODULES CONFIGURATION
#define THERMOMETER_SUPPORT
#define FAN_SUPPORT
#define WS_SUPPORT
#define OLED_SUPPORT

//TEST MODE - CONNECT ESP THROUGH SERIAL AND CHECK IF EVERYTHING WORKS
#define TEST_MODE


//PINS CONFIGURATION
#define PIN_INPUT_DIPSWITCH_1 35
#define PIN_INPUT_DIPSWITCH_2 34
#define PIN_INPUT_DIPSWITCH_3 39
#define PIN_INPUT_DIPSWITCH_4 36
#define PIN_INPUT_BUTTON 24

#define PIN_OUTPUT_WS 32
#define PIN_OUTPUT_USB_DPLUS 25
#define PIN_OUTPUT_USB_DMINUS 26
#define PIN_DAC_USB_DPLUS DAC1
#define PIN_DAC_USB_DMINUS DAC2

#define PIN_OUTPUT_MAX_MOSI 27
#define PIN_OUTPUT_MAX_CLK 12
#define PIN_OUTPUT_MAX_CS 13

#define PIN_SD_CLK 14
#define PIN_SD_D0 2
#define PIN_SD_CMD 15

#define PIN_OUTPUT_PX_MOSI 23
#define PIN_OUTPUT_PX_CLK 18
#define PIN_OUTPUT_PX_STROBO 5
#define PIN_OUTPUT_PX_REGLATCH PIN_OUTPUT_PX_STROBO
#define PIN_OUTPUT_PX_OE 19

#define PIN_EMPTY_1 17
#define PIN_EMPTY_2 16
#define PIN_EMPTY_3 4

#define PIN_SDA 21
#define PIN_SCL 22
//Set to 255 if no reset pin
#define PIN_OLED_RESET 255

#define PIN_DHT22TEMP 33

#define I2C_OLED_ADDRESS 0x0
#define I2C_CURRENTMETER_ADDRESS 0x0

#define PIN_REG_FIRST 0
#define PIN_BUZZER PIN_REG_FIRST
#define PIN_REG_SECOND 1
#define PIN_REG_THIRD 2
#define PIN_REG_FOURTH 3


//SETTINGS
//PxMatrix section
//Comment if you want custom defined PXMatrix instance
#define PxMATRIX_GLOBAL_INSTANCE

// Defines how long we display things by default
#define PxMATRIX_SHOWTIME 40

// Defines the speed of the spiClass bus (reducing this may help if you experience noisy images)
#define PxMATRIX_SPI_FREQUENCY 15000000

#define PxMATRIX_INTERRUPT_TIMER 5000
//Defines max amount of buffered frames
#define PxMATRIX_MAX_FRAMES_BUFFER 40

//Defines SPIClass for PxMatrices
#define PxMATRIX_SPICLASS HSPI

// Defines the buffer height / the height of the matrix
#define PxMATRIX_HEIGHT 32

// Defines the buffer width / the maximum width of the matrix
#define PxMATRIX_WIDTH 128
//Number of matrices in the row
#define PxMatrix_WIDTH_NUMBER 2

// This is how many color levels the display shows - the more the slower the update
#define PxMATRIX_COLOR_DEPTH 1 //TODO: HERE MAY COLORS BE INCORRECTLY DISPLAYED

//Row pattern of matrix
#define PxMATRIX_ROW_PATTERN 16

//Default brightness of Matrix (0-255)
#define PxMATRIX_DEFAULT_BRIGHTNESS 20

//Mux delay of PX Matrix
#define PxMATRIX_MUX_DELAY 10

//FULLBRIGHTNESS TEST
//#define PxMATRIX_FULLBRIGHTNESS

//MAX7219 Settings
//Frequency of SPI used for MAX7219
#define MAX_SPI_FREQ 2000000

//Number of matrics
#define MAX_MATRICES_NUMBER 8
//Height of matrix (in number of matrices)
#define MAX_HEIGHT 4
//Local width of matrices (in number of matrics)
#define MAX_WIDTH 2

//SPI CLASS used for SPI
#define MAX_SPI_CLASS VSPI

//MAX FPS OF MAX7219
#define MAX_MAXFPS 30

//Default MAX brightness
#define MAX_DEFAULT_BRIGHTNESS 3

//Global MAX7219 Instance
#define MAX_GLOBAL_INSTANCE

//Unfortunately, MAX matrices need to be initialized.
// If you experience blocks of MAXes (for example screen getting fullbrigthness), you can define,
// how often the system will send startup commands to maxes (in milliseconds)
#define MAX_STARTUP_REFRESH 10000

//Performs test of all leds, turning them on at max power
//#define MAX_FULLPOWER_TEST

//WS CONTROL
//Number of leds in a row
#define WS_NUMBEROFLEDS 64
//Width of matrix in leds count
#define WS_WIDTH 8

//Height of matrix in leds count
#define WS_HEIGHT 8

//Default brightness of WS
#define WS_DEFAULT_BRIGHTNESS 10

//If create global instance
#define WS_GLOBAL_INSTANCE

//If defined, performs test by turning all WS leds at max power
//#define WS_FULLPOWER_TEST
//QuickCharge fan control control setup
//Define if you want global instance of qc fan control
#define QC_GLOBAL_INSTANCE

//Minimum and maximum QC voltage
#define QC_MIN_VOLTAGE 5000
#define QC_MAX_VOLTAGE 12000

//Thermometer control

//Define if you want global instance of Thermometer control
#define THERMOMETER_GLOBAL_INSTANCE

//OLED control
//Define if you want global instance of Oled Control
#define OLED_GLOBAL_INSTANCE

//BLUETOOTH OPTION

// See the following for generating UUIDs if you want your custom UUID:
// https://www.uuidgenerator.net/

//Service UUID for BTLE
#define BLUETOOTH_SERVICE_UUID        "c70ab98a-5fd8-49ac-b370-76b49d821cad"
//Characteristics for BTLE
#define BLUETOOTH_CHARACTERISTIC_UUID "85eae7bc-2ae3-4bd3-8763-a43e4f99508e"


//PIN used to protect the connection. This is default pin, that can be overriden
const int BLUETOOTH_CONNECTION_PIN = 11991;

//Define if you want global Bluetooth instance
#define BLUETOOTH_GLOBAL_INSTANCE

//Define if you want debug output to be redirected to Serial
//#define SERIAL_DEBUG


//Define in you want to disable debug output
//#define DISABLE_DEBUG

//Useful, global methods

#include <Arduino.h>
void writeToRegisterPin(uint8_t pin, uint8_t state);

void debugPrint(const String& text);

#ifdef SERIAL_DEBUG

void debugPrint(const String& text){
    Serial.println(text);
}
#elif DISABLE_DEBUG
void debugPrint(String text)
{

}
#endif


#endif //VISORV3_CONSTANTSANDSETTINGS_H
