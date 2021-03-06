//
// Created by dzing on 26/02/2021.
//

#ifndef VISORV3_MAXPINDEFINITIONS_H
#define VISORV3_MAXPINDEFINITIONS_H

//CONTROLS
//#define FAN_QC_CONTROL 1
//#define OLED_CONTROL 2
//#define PX_BRIGHTNESS_CONTROL 3
#define MAX_BRIGTHNESS_CONTROL 4

//SENSORS
//#define THERMOMETER_HYDROMETER_SENSOR 1
//#define CURRENT_VOLTAGE_SENSOR 2

//SCREENS
//#define PX_MARIX_SCREEN 1
#define MAX_MATRIX_SCREEN 2

//HARDWARE
//SD SUPPORT
//#define SD_SUPPORT
//WIFI CONNECTION
//#define WIFI_SUPPORT
//BLUETOOTH CONNECTION
//#define BLUETOOTH_SUPPORT
//TIME
//#define TIME_SUPPORT

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

#define PIN_OUTPUT_MAX_MOSI 23
#define PIN_OUTPUT_MAX_CLK 18
#define PIN_OUTPUT_MAX_CS 5
#define PIN_OUTPUT_MAX_ALTERNATIVE 17

#define PIN_SD_CLK 14
#define PIN_SD_D0 2
#define PIN_SD_CMD 15

//#define PIN_OUTPUT_PX_MOSI 23
//#define PIN_OUTPUT_PX_CLK 18
//#define PIN_OUTPUT_PX_STROBO 5
//#define PIN_OUTPUT_PX_REGLATCH PIN_OUTPUT_PX_STROBO
//#define PIN_OUTPUT_PX_OE 19

//#define PIN_EMPTY_1 17
#define PIN_EMPTY_2 16
#define PIN_EMPTY_3 4
#define PIN_EMPTY_4 23
#define PIN_EMPTY_5_5V 4

#define PIN_SDA 21
#define PIN_SCL 22
//Set to 255 if no reset pin
#define PINOLED_RESET 255

//#define PIN_DHT22TEMP 33


//External board header - can be used by other devices
#define PIN_8_IR_REC 33
#define PIN_7_IR_TRANS 27
#define PIN_6_EXTERNAL 13
#define PIN_5_EXTERNAL 12

//#define I2C_OLED_ADDRESS 0x0
//#define I2C_CURRENTMETER_ADDRESS 0x0

//SETTINGS
#ifdef MAX_MATRIX_SCREEN
//MAX7219 Settings
//Frequency of SPI used for MAX7219
#define MAX_SPI_FREQ 2000000

//If not defined - all matrices are chained
//MAX_BULK_MODE 1 - matrices are in bulks of 4 - like in Z-visor

//Number of matrics
#define MAX_MATRICES_NUMBER 12
//Height of matrix (in number of matrices)
#define MAX_HEIGHT 3
//Local width of matrices (in number of matrics)
#define MAX_WIDTH 4

//SPI CLASS used for SPI
#define MAX_SPI_CLASS VSPI

//MAX FPS OF MAX7219
#define MAX_MAXFPS 30

//Default MAX brightness
#define MAX_DEFAULT_BRIGHTNESS 3

/**
 *Defines connection type (see ConnectionType for help)
 * 1 - DIRECT LINES
 * 2 - BULK2x2
 * 3 - SIMPLE VISOR
 */

#define MAX_CONNECTION_TYPE 3


//Unfortunately, MAX matrices need to be initialized.
// If you experience blocks of MAXes (for example screen getting fullbrigthness), you can define,
// how often the system will send startup commands to maxes (in milliseconds)
#define MAX_STARTUP_REFRESH 10000

//Performs test of all leds, turning them on at max power
//]#define MAX_FULLPOWER_TEST
#endif

#ifdef WS_MATRIX_SUPPORT
//WS CONTROL
//Number of leds in a row
#define WS_NUMBEROFLEDS 64
//Width of matrix in leds count
#define WS_WIDTH 8

//Height of matrix in leds count
#define WS_HEIGHT 8

//Default brightness of WS
#define WS_DEFAULT_BRIGHTNESS 10


//If defined, performs test by turning all WS leds at max power
//#define WS_FULLPOWER_TEST

#endif

#ifdef FAN_QC_SUPPORT

//QuickCharge fan control control setup
//Define if you want global instance of qc fan control
#define QC_GLOBAL_INSTANCE

//Minimum and maximum QC voltage
#define QC_MIN_VOLTAGE 5000
#define QC_MAX_VOLTAGE 12000

#endif

#ifdef THERMOMETER_HYDROMETER_SUPPORT

#endif

#ifdef BLUETOOTH_SUPPORT
//BLUETOOTH OPTION

// See the following for generating UUIDs if you want your custom UUID:
// https://www.uuidgenerator.net/

//Service UUID for BTLE
#define BLUETOOTH_SERVICE_UUID        "c70ab98a-5fd8-49ac-b370-76b49d821cad"
//Characteristics for BTLE
#define BLUETOOTH_CHARACTERISTIC_UUID "85eae7bc-2ae3-4bd3-8763-a43e4f99508e"


//PIN used to protect the connection. This is default pin, that can be overridden
const int BLUETOOTH_CONNECTION_PIN = 11991;

#endif

#ifdef WIFI_SUPPORT
//WIFI
//Default SSID of WIFI network
extern String wifiSsid;

//Default Password of WIFI network
extern String wifiPassword;

//Wifi hostname - It will always start with 'VISS_'
extern String wifiHostname;
//WIFI PORT
extern int WIFI_PORT;

#endif
//Define if you want debug output to be redirected to Serial
//#define SERIAL_DEBUG


#endif //VISORV3_MAXPINDEFINITIONS_H
