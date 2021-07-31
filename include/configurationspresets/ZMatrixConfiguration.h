//
// Created by dzing on 26/02/2021.
//

#ifndef VISORV3_PXMATRIXPINDEFINITIONS_H
#define VISORV3_PXMATRIXPINDEFINITIONS_H


//MODULES CONFIGURATION

//Capabilities:
//Here, you can define, what is supported. Defined number is an ID of a device. It should be max 255 per control, sensor and screen

//CONTROLS
#define FAN_QC_CONTROL 1
#define OLED_SUPPORT 2
#define PX_BRIGHTNESS_CONTROL 3
#define MAX_BRIGTHNESS_CONTROL 4
#define WS_BRIGTHNESS_CONTROL

//SENSORS
#define THERMOMETER_HYDROMETER_SENSOR 1
#define CURRENT_VOLTAGE_SENSOR 2
//SCREENS
#define PX_MATRIX_SCREEN 1
#define MAX_MATRIX_SCREEN 2
//#define WS_MATRIX_SCREEN 3

//HARDWARE
//SD SUPPORT
#define SD_SUPPORT
//WIFI CONNECTION
#define WIFI_SUPPORT
//BLUETOOTH CONNECTION
//#define BLUETOOTH_SUPPORT
//TIME
#define TIME_SUPPORT


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

//#define I2C_OLED_ADDRESS 0x0
//#define I2C_CURRENTMETER_ADDRESS 0x0

#define PIN_REG_FIRST 0
#define PIN_BUZZER PIN_REG_FIRST
#define PIN_REG_SECOND 1
#define PIN_REG_THIRD 2
#define PIN_REG_FOURTH 3

//SETTINGS
//PxMatrix section
#ifdef PX_MATRIX_SCREEN

// Defines how long we display things by default
#define PxMATRIX_SHOWTIME 40

// Defines the speed of the spiClass bus (reducing this may help if you experience noisy images)
#define PxMATRIX_SPI_FREQUENCY 15000000

#define PxMATRIX_INTERRUPT_TIMER 5000
//Defines max amount of buffered frames
#define PxMATRIX_MAX_FRAMES_BUFFER 40

//Defines SPIClass for PxMatrices
#define PxMATRIX_SPICLASS HSPI

// Defines the maxBuffer height / the height of the matrix
#define PxMATRIX_HEIGHT 32

// Defines the maxBuffer width / the maximum width of the matrix
#define PxMATRIX_WIDTH 128
//Number of matrices in the row
#define PxMatrix_WIDTH_NUMBER 2

// This is how many color levels the display shows - the more the slower the update
#define PxMATRIX_COLOR_DEPTH 1 //TODO: HERE MAY COLORS BE INCORRECTLY DISPLAYED

//Row pattern of matrix
#define PxMATRIX_ROW_PATTERN 16

//Default brightness of Matrix (0-255)
#define PxMATRIX_DEFAULT_BRIGHTNESS 128

//Mux delay of PX Matrix
#define PxMATRIX_MUX_DELAY 10

//FULLBRIGHTNESS TEST
//#define PxMATRIX_FULLBRIGHTNESS

#endif

#ifdef MAX_MATRIX_SCREEN
//MAX7219 Settings
//Frequency of SPI used for MAX7219
#define MAX_SPI_FREQ 2000000

/*
 * Used to calculate pixels:
 * 1 - connected as 1 long string of matrices
 * 2 - connected as squares 2x2
 * 3 - connected as simple visor configuration
 */
#define MAX_CONNECTION_TYPE 2

//If not defined - all matrices are chained
//MAX_BULK_MODE 1 - matrices are in bulks of 4 - like in Z-visor
#define MAX_BULK_MODE 1
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

//Unfortunately, MAX matrices need to be initialized.
// If you experience blocks of MAXes (for example screen getting fullbrigthness), you can define,
// how often the system will send startup commands to maxes (in milliseconds)
#define MAX_STARTUP_REFRESH 10000

//Performs test of all leds, turning them on at max power
//#define MAX_FULLPOWER_TEST
#endif

#ifdef WS_MATRIX_SCREEN
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

#ifdef FAN_QC_CONTROL


//Minimum and maximum QC voltage
#define QC_MIN_VOLTAGE 5000
#define QC_MAX_VOLTAGE 12000

#endif

#ifdef THERMOMETER_HYDROMETER_SENSOR
//Thermometer control

#endif


#ifdef CURRENT_VOLTAGE_SENSOR
//Current meter control
//Shut type (ohm)
#define CURRENT_VOLTAGE_SHUNT_OHM 0.01

//Max shunt voltage
#define CURRONT_VOLTAGE_SHUNT_VOLTAGE 32

#endif

#ifdef OLED_SUPPORT
//OLED control

//Number of lines
#define OLED_LINES 4
//Max line length
#define OLED_MAX_LENGTH 16

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

//Define if you want global Bluetooth instance
#define BLUETOOTH_GLOBAL_INSTANCE

#endif

#ifdef WIFI_SUPPORT
//WIFI
/**
 * SSID of WIFI network, configured
 */
extern String wifiSsid;

/**
 * Password of wifi network
 */
extern String wifiPassword;

/**
 * Wifi hostname - It will always start with 'VISS_'
 */

extern String wifiHostname;
/**
 * Port for the server to listen on wifi
 */
extern int wifiPort;
/**
 * Current IPAddress, used with wifi control
 */
extern String ipAddress;
#endif
//Define if you want debug output to be redirected to Serial
//#define SERIAL_DEBUG


#endif //VISORV3_PXMATRIXPINDEFINITIONS_H
