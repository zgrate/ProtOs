//
// Created by dzing on 03/02/2021.
//

#ifndef VISORV3_MAINSYSTEM_H
#define VISORV3_MAINSYSTEM_H

#include "Arduino.h"
#include "SD_MMC.h"
#include "ConstantsAndSettings.h"
#include "control/Packets.h"
#include "control/Definitions.h"
#include "ConstantsAndSettings.h"
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


class LoadedData {
private:
    String fileName;
    uint32_t packetLength;
    String animationName;
    uint8_t version;
    uint8_t flags;
    uint8_t fps;
    uint8_t unused[2]{};

    unsigned long maxFrames;
    uint8_t maxSizeX, maxSizeY;
    uint8_t *maxData;
    int maxFrameSize{};

    unsigned long wsFrames;
    uint8_t wsSizeX, wsSizeY;
    uint8_t *wsData;
    int wsFrameSize = 0;

    bool pxStreaming;
    unsigned long pxFrames;
    uint8_t pxSizeX, pxSizeY;
    uint8_t *pxData;
    String pxFileName;
    File pxFile;
    uint8_t pxFileStart = 0;
    int pxFrameSize = 0;


public:
    int getMaxFrameSize() const {
        return maxFrameSize;
    }

    int getWsFrameSize() const {
        return wsFrameSize;
    }

    int getPxFrameSize() const {
        return pxFrameSize;
    }

    ~LoadedData() {
        if (maxData != nullptr) {
            free(maxData);
        }
        if (wsData != nullptr) {
            free(wsData);
        }
        if (pxData != nullptr) {
            free(pxData);
        }
        if (pxFile) {
            pxFile.close();
        }
    }

    LoadedData() {
        packetLength = 0;
        fileName = "";
        animationName = "";
        version = 0;
        flags = 0;
        fps = 0;
        unused[0] = 0;
        unused[1] = 0;

        maxFrames = 0;
        maxSizeX = 0;
        maxSizeY = 0;
        maxData = nullptr;

        wsFrames = 0;
        wsSizeX = 0;
        wsSizeY = 0;
        wsData = nullptr;

        pxStreaming = false; //todo
        pxFrames = 0;
        pxSizeX = 0;
        pxSizeY = 0;

        pxData = nullptr;
        pxFileName = "";
        pxFile = File();


    }

    void printContents();

    void loadFile(String filename);

    int getFPS() const {
        return fps;
    }

    bool isStreaming() const {
        return this->pxStreaming;
    }


    uint8_t *getPXFrameStartAddress(unsigned long currentFramePx) {
        return pxData + currentFramePx * pxFrameSize;
    }

    unsigned long getPxFrames() const {
        return pxFrames;
    }

    unsigned long getMaxFrames() const {
        return maxFrames;
    }

    uint8_t *getMaxFrameStartAddress(unsigned long frame) {
        return maxData + frame * maxFrameSize;
    }

    unsigned long getWsFrames() const {
        return wsFrames;
    }

    uint8_t *getWSFrameStartAddress(unsigned long frame) {
        return wsData + frame * wsFrameSize;
    }

    void seekPXMatrixData(int deltaPosition);

    void readPxToBuffer(uint8_t *buffer);
};


class MainSystem {

private:
    void beginAll();

    LoadedData *data = nullptr;
    unsigned long currentFramePx = 0, currentFrameMax = 0, currentFrameWs = 0;
    unsigned long lastFrame = millis();
    int frameTime = 0;


public:
    static MainSystem &getMainSystem();

    void setup();

    void loop();

    void loadNewFile(const String &filename);

    void mainDisplayLoop();

#ifdef MAX_MATRIX_SCREEN
private:
    Max7219Screen max7219Screen;
public:
    Max7219Screen &getMaxScreen() {
        return max7219Screen;
    }

#endif
#ifdef PX_MATRIX_SCREEN
private:
    PxMatrixScreen pxMatrixScreen = PxMatrixScreen(PxMATRIX_WIDTH, PxMATRIX_HEIGHT, PIN_OUTPUT_PX_MOSI,
                                                   PIN_OUTPUT_PX_CLK,
                                                   PIN_OUTPUT_PX_STROBO, PIN_OUTPUT_PX_OE, PIN_OUTPUT_PX_REGLATCH);
public:
    PxMatrixScreen &getPxMatrixScreen() {
        return pxMatrixScreen;
    }

#endif
#ifdef WS_MATRIX_SCREEN
    //TODO
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

    void testSD() {
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
    }

#endif

    void executeTests() {
        Serial.println("STARTING TESTS....");
#ifdef THERMOMETER_HYDROMETER_SENSOR
        thermometerSensor.test();
#endif
#ifdef WS_MATRIX_SCREEN
        WsControlInstance.test();
#endif
#ifdef MAX_MATRIX_SCREEN
        max7219Screen.test();
#endif
#ifdef PX_MATRIX_SCREEN
        pxMatrixScreen.test();
#endif
#ifdef CURRENT_VOLTAGE_SENSOR
        currentVoltageSensor.test(); //TODO:TEST?
#endif
#ifdef FAN_GLOBAL_INSTANCE
        QC3FanControlInstance.test();
#endif
#ifdef SD_SUPPORT
        testSD();
#endif
#ifdef OLED_SUPPORT //TODO TEST MUST ME MADE USING I2C SCANNER
        oledControl.test();
#endif
        Serial.println("Tests COMPLETED!");
    };

};

namespace main_ns {


    void liveDrawUpdate(const uint8_t &screenId, const vector<Pixel> &vector);

    void clearDisplay(const uint8_t &screenId);

    String readSensor(const uint8_t &sensorId, uint8_t *type);

    void control(const uint8_t &controlId, const String &stringValue);

    void forwardPacket(const std::shared_ptr<ClientBoundPacket> &packet);
}


#endif //VISORV3_MAINSYSTEM_H
