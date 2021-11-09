//
// Created by dzing on 03/02/2021.
//

#ifndef VISORV3_MAINSYSTEM_H
#define VISORV3_MAINSYSTEM_H

#include <control/BluetoothManager.h>
#include "Arduino.h"
#include "SD_MMC.h"
#include "ConstantsAndSettings.h"
#include "control/Packets.h"
#include "control/Definitions.h"
#include "libraries/PxMatrix.h"
#include "libraries/Max7219Screen.h"
#include "libraries/WSControl.h"
#include "libraries/ThermometerSensor.h"
#include "libraries/Max7219Screen.h"
#include "libraries/FanControls.h"
#include "libraries/OLEDControl.h"
#include "control/WifiManager.h"
#include "control/Packets.h"
#include "libraries/CurrentVoltageSensor.h"


#define DATA_VERSION 1


#ifdef PX_MATRIX_SCREEN

//void mainPxScreenThread();
#endif
namespace main_ns {

/**
 * Draws on the screen, while the screen is in LiveAnimation mode
 * @param screenId ID of the screen, based on configuration file
 * @param vector List of pixels to draw
 */
    void liveDrawUpdate(const uint8_t &screenId, const vector<Pixel> &vector);

    /**
     * Clears the screen (sets all pixels to BLACK)
     * @param screenId ID of the screen, based on configuration file
     */
    void clearDisplay(const uint8_t &screenId);

    /**
     * Reads the data from the given sensor
     * @param sensorId ID of the sensor, based on configuration file
     * @param type pointer to the variable holding the type of the sensor
     * @return value read from the sensor, in a format defined by the type
     */
    String readSensor(const uint8_t &sensorId, uint8_t *type);

    /**
     * Sends the control signal to the device, controlling something
     * @param controlId Id of the control, defined in configuration
     * @param stringValue string value of a control value, for example target speed
     */
    void control(const uint8_t &controlId, const String &stringValue);

    /**
     * Forwards the packet to the Network Thread and sends it
     * @param packet Packet to send
     */
    void forwardPacket(const std::shared_ptr<ClientBoundPacket> &packet);

    /**
     * Read input state from a pin - designed for a dip switches and Analog input
     * @param pinNumber Pin number
     */
    bool readInputState(const int &pinNumber);

    /**
     * Prints message on internal screen
     * @param lineNumber Line number, defaults to 0
     */
    void printOnExternalScreen(const String &line, const int &lineNumber, const bool &update = true);

    void load_animation(const char *string);

    void load_animation_from_number(int number);
}

class LoadedData {
private:
    String fileName;
    uint32_t packetLength;
    String animationName;
    uint8_t version;
    uint8_t flags;
    uint8_t fps;


    uint16_t maxFrames;
    //uint8_t maxSizeX, maxSizeY;
    uint8_t *maxData;
    uint16_t maxFrameSize;

//    unsigned long wsFrames;
//    uint8_t wsSizeX, wsSizeY;
//    uint8_t *wsData;
//    int wsFrameSize = 0;

    bool pxStreaming = true;
    unsigned short pxFrames;
    //uint8_t pxSizeX, pxSizeY;
//    uint8_t *pxData = new uint8_t[0];
    String pxFileName;
    File pxFile;
    uint8_t pxFileStart = 1;
    uint16_t pxFrameSize = 0;
    bool initialized = false;


public:


    int getMaxFrameSize() const {
        return maxFrameSize;
    }

/*    int getWsFrameSize() const {
        return wsFrameSize;
    }*/

    int getPxFrameSize() const {
        return pxFrameSize;
    }

    ~LoadedData() {
//        if (maxData != nullptr) {
//            free(maxData);
//        }
//        if (wsData != nullptr) {
//            free(wsData);
//        }
        if (pxFile) {
            pxFile.close();
        }
    }

    bool isInitialized() const;

    LoadedData() {
        initialized = false;
        packetLength = 0;
        fileName = "";
        version = 0;
        animationName = "";
        flags = 0;
        fps = 0;

        maxFrames = 0;

        //maxSizeX = 0;
        //   maxSizeY = 0;

        /**
         * Max frame size
         */
        maxFrameSize = 0;
        maxData = nullptr;

//        wsFrames = 0;
//        wsSizeX = 0;
//        wsSizeY = 0;
//        wsData = nullptr;

        pxStreaming = false; //todo
        pxFrames = 0;
//        pxSizeX = 0;
//        pxSizeY = 0;

        //pxData = nullptr;
        pxFileName = "";
        pxFile = File();


    }

    void printContents();

#define MAX_HEADER 1
#define PX_HEADER 2

#define PX_INFO_FILE 100
#define PX_STREAMING_FILE 101

    const String &getAnimationName() const;

    /**
     * Loads the file from SD card to the memory
     * @param filename filename
     * @return True if success False if not success
     */
    bool loadFile(const String &filename);

    /**
     * Returns speed in Frames Per Second for this animation
     * @return int as FPS
     */
    int getFPS() const {
        return fps;
    }

    bool isStreaming() const {
        return this->pxStreaming;
    }


//    uint8_t *getPXFrameStartAddress(unsigned long currentFramePx) {
//        return pxData + currentFramePx * pxFrameSize;
//    }

    unsigned long getPxFrames() const {
        return pxFrames;
    }

    unsigned long getMaxFrames() const {
        return maxFrames;
    }

    uint8_t *getMaxFrameStartAddress(unsigned long frame) {
        return maxData + frame * maxFrameSize;
    }

//    unsigned long getWsFrames() const {
//        return wsFrames;
//    }
//
//    uint8_t *getWSFrameStartAddress(unsigned long frame) {
//        return wsData + frame * wsFrameSize;
//    }

    void seekPXMatrixData(int deltaPosition);

    void readPxToBuffer(uint8_t *buffer);
};

/**
 * Main Class, this is the entry point of the Visor App
 */
class MainSystem {

private:
    /**
     * Initializes all the hardware of the system
     */
    void beginAll();

    /**
     * Stores current loaded data from the SD Card
     */
    LoadedData currentData = LoadedData();

    /**
     * Helper variables for main loop
     */
    unsigned long currentFramePx = 0, currentFrameMax = 0, currentFrameWs = 0;
    unsigned long lastFrame = millis();
    int frameTime = 0;


public:
    /**
     * Static Singleton of the main system instance
     * @return Main instance of MainSystem
     */
    static MainSystem &getMainSystem();

    /**
     * Setup method, that should be executed from global setup() method
     */
    void setup();

    /**
     * Main loop of application, that should be executed from global loop() method
     */
    void loop();

    /**
     * TODO
     * @param filename
     */
    void loadNewFile(const String &filename);

    /**
     * Main loop of display thread, that plays animations
     */
    void mainDisplayLoop();

    /**
     * This section is dependant on configuration in files
     */
#ifdef MAX_MATRIX_SCREEN
private:
    Max7219Screen max7219Screen = Max7219Screen();
public:
    Max7219Screen &getMaxScreen() {
        return max7219Screen;
    }

#endif
#ifdef PX_MATRIX_SCREEN
private:
//        PxMatrixScreen pxMatrixScreen = PxMatrixScreen(PxMATRIX_WIDTH, PxMATRIX_HEIGHT, PIN_OUTPUT_PX_MOSI,
//                                                       PIN_OUTPUT_PX_CLK,
//                                                       PIN_OUTPUT_PX_STROBO, PIN_OUTPUT_PX_OE, PIN_OUTPUT_PX_REGLATCH);
public:
    PxMatrixScreen &getPxMatrixScreen() {
        return PxMatrixControlInstance;
    }

    void startDisplayThread() {
        //getPxMatrixScreen().startDisplayThread();
        //   pxMatrixScreen.timer = timerBegin(0, 80, true);
        // timerAttachInterrupt(pxMatrixScreen.timer, &mainPxScreenThread, true);
        //timerAlarmWrite(pxMatrixScreen.timer, PxMATRIX_INTERRUPT_TIMER, true);
        //timerAlarmEnable(pxMatrixScreen.timer);
    }

#endif
#ifdef WS_MATRIX_SCREEN
private:
    WSControl wsControl;
public:
    WSControl &getWsControl() {
        return wsControl;
    }

#endif
#ifdef THERMOMETER_HYDROMETER_SENSOR
    private:
        ThermometerSensor thermometerSensor;
    public:
        ThermometerSensor &getThermometerSensor() {
            return thermometerSensor;
        }

#endif
#ifdef CURRENT_VOLTAGE_SENSOR
private:
    CurrentVoltageSensor currentVoltageSensor;
public:
    CurrentVoltageSensor &getCurrentVoltageSensor() {
        return currentVoltageSensor;
    }

#endif
#ifdef FAN_QC_CONTROL
private:
    QC3FanControl qc3FanControl;
public:
    QC3FanControl &getQC3FanControl() {
        return qc3FanControl;
    }

#endif
#ifdef OLED_SUPPORT
private:
    OLEDControl oledControl;
public:
    OLEDControl &getOledControl() {
        return oledControl;
    }

#endif
#ifdef WIFI_SUPPORT
private:
    WifiManager wifiManager;
public:
    WifiManager &getWifiManager() {
        return wifiManager;
    }

#endif

#ifdef SD_SUPPORT

    /**
     * Executes SD card setup
     */
    static void beginAndTestSD() {
        if (!SD_MMC.begin("/sdcard", true)) {
            Serial.println("Card Mount Failed");
            return;
        }
        uint8_t cardType = SD_MMC.cardType();

        if (cardType == CARD_NONE) {
            Serial.println("No SD_MMC card attached");
            return;
        }

            Serial.print("SD_MMC Card Type: ");
            if (cardType == CARD_MMC) {
                Serial.println("MMC");
            } else if (cardType == CARD_SD) {
                Serial.println("SDSC");
            } else if (cardType == CARD_SDHC) {
                Serial.println("SDHC");
            } else {
                Serial.println("UNKNOWN");
            }

        uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
        Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
        Serial.printf("Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
        Serial.printf("Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));

        readConfiguration();
    }

#endif

    void executeTests() {
        Serial.println("STARTING TESTS....");
        Serial.println(main_ns::readInputState(PIN_INPUT_DIPSWITCH_1));
#ifdef THERMOMETER_HYDROMETER_SENSOR
        thermometerSensor.test();
#endif
#ifdef WS_MATRIX_SCREEN
        wsControl.test();
#endif
#ifdef MAX_MATRIX_SCREEN
        max7219Screen.test();
#endif
#ifdef PX_MATRIX_SCREEN
        getPxMatrixScreen().test();
#endif
#ifdef CURRENT_VOLTAGE_SENSOR
        currentVoltageSensor.test(); //TODO:TEST?
#endif
#ifdef FAN_QC_CONTROL
        qc3FanControl.test();
#endif

#ifdef OLED_SUPPORT //TODO TEST MUST ME MADE USING I2C SCANNER
        oledControl.test();
#endif

        Serial.println("Tests COMPLETED!");
    }


    void startSystem() {
        oledControl.clear();
        loadNewFile(START_FILE);
    }

};



#endif //VISORV3_MAINSYSTEM_H
