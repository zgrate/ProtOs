//
// Created by dzing on 03/02/2021.
//

#ifndef VISORV3_MAINSYSTEM_H
#define VISORV3_MAINSYSTEM_H

#include "Arduino.h"
#include "SD_MMC.h"
#include "control/Packets.h"
#include "control/Definitions.h"
#include "INA219.h"

#define DATA_VERSION 1
#define MAX_DATA 100
#define WS_DATA 101
#define PX_IN_FILE_DATA 102
#define PX_EXTERNAL_STREAMING 103
#define INVALID 0


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

    uint8_t pxMode;
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

        pxMode = INVALID;
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

    int getPXMatrixMode() const {
        return pxMode;
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
    void setup();

    void loop();

    void loadNewFile(const String &filename);

    void mainDisplayLoop();

};

namespace main_ns {
    void liveDrawUpdate(const uint8_t &screenId, const vector<Pixel> &vector);

    void clearDisplay(const uint8_t &screenId);

    String readSensor(const uint8_t &sensorId, uint8_t *type);

    void control(const uint8_t &controlId, const String &stringValue);

    void forwardPacket(const std::shared_ptr<ClientBoundPacket> &packet);

}

extern MainSystem MAIN;


#endif //VISORV3_MAINSYSTEM_H
