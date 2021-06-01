//
// Created by dzing on 29/01/2021.
//

#ifndef VISORV3_CONSTANTSANDSETTINGS_H
#define VISORV3_CONSTANTSANDSETTINGS_H

#include <Arduino.h>
#include <cstring>
//#define MAIN_PX_MATRIX
#define MAIN_MAX_MATRIX


//MODULES CONFIGURATION
#define THERMOMETER_SUPPORT
#define FAN_SUPPORT
#define WS_SUPPORT
#define OLED_SUPPORT

//TEST MODE - CONNECT ESP THROUGH SERIAL AND CHECK IF EVERYTHING WORKS
#define TEST_MODE



//Import pin definitions
#ifdef MAIN_PX_MATRIX
#include "pindefinitions/PXMatrixPinDefinitions.h"
#endif

#ifdef MAIN_MAX_MATRIX

#include "pindefinitions/MAXPinDefinitions.h"

#endif

#define CONFIGURATION_FILE "/config.json"

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
#define PxMATRIX_DEFAULT_BRIGHTNESS 255

//Mux delay of PX Matrix
#define PxMATRIX_MUX_DELAY 10

//FULLBRIGHTNESS TEST
//#define PxMATRIX_FULLBRIGHTNESS

//MAX7219 Settings
//Frequency of SPI used for MAX7219
#define MAX_SPI_FREQ 2000000

//Number of matrics
#define MAX_MATRICES_NUMBER 12
//Height of matrix (in number of matrices)
#define MAX_HEIGHT 4
//Local width of matrices (in number of matrics)
#define MAX_WIDTH 4

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
//]#define MAX_FULLPOWER_TEST

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

//Current meter control

//Shut type (ohm)
#define SHUNT_OHM 0.01

//Max shunt voltage
#define SHUNT_VOLTAGE 32

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

//WIFI
//Default SSID of WIFI network
extern String WIFI_SSID;

//Default Password of WIFI network
extern String WIFI_PASSWORD;

//Wifi hostname - It will always start with 'VISS_'
extern String WIFI_HOSTNAME;
//WIFI PORT
extern int WIFI_PORT;

//Define if you want debug output to be redirected to Serial
//#define SERIAL_DEBUG



//Define in you want to disable debug output
//#define DISABLE_DEBUG

//Useful, global methods

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
