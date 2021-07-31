//
// Created by dzing on 25/01/2021.
//

#ifndef VISORV3_MAX7219SCREEN_H
#define VISORV3_MAX7219SCREEN_H

#include <random>
#include "ConstantsAndSettings.h"
#ifdef MAX_MATRIX_SCREEN
#define MAX7219_TEST 0x0f // in real code put into a .h file
#define MAX7219_BRIGHTNESS 0x0a // in real code put into a .h file
#define MAX7219_SCAN_LIMIT 0x0b // in real code put into a .h file
#define MAX7219_DECODE_MODE 0x09 // in real code put into a .h file
#define MAX7219_SHUTDOWN 0x0C // in real code put into a .h file

#if MAX_CONNECTION_TYPE == 3

extern uint8_t SIMPLE_VISOR_OFFSETS[32][24];

#endif

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include "control/Definitions.h"


class Max7219Screen : public Adafruit_GFX, public Screen {

public:
    const uint16_t MAX_OFF_PIXEL = 0;
    const uint16_t MAX_ON_PIXEL = 255;

    SPIClass maxSpiClass = SPIClass(MAX_SPI_CLASS);

    Max7219Screen();

    void begin() override;

    void drawPixel(int16_t x, int16_t y, uint16_t color) override;

    void display();

    void clear() override;

    void test();

    int getFPS() const;

    void writeFrameFromBuffer(const uint8_t *frameStartAddress, const int &frameLength) override;

    void drawPixels(const std::vector<Pixel> &vector) override;

    void setBrightness(const uint8_t &target) override;

    void init(const uint8_t &numberOfMatrices, const uint8_t &width, const uint8_t &height, const uint8_t &mosi,
              const uint8_t &clk, const uint8_t &ss,
              const ConnectionType &connectionType) {
        matricesNumber = numberOfMatrices;
        //maxBuffer = new uint8_t[numberOfMatrices * 8];
        //Serial.println("BUFFER WRITING COMPLETED! RESULT: " + String(maxBuffer == nullptr));

        spiMosi = mosi;
        spiClk = clk;
        spiSS = ss;
        _height = height;
        _width = width;
        WIDTH = _width * 8;
        HEIGHT = _height * 8;
        prefillOffsets();
        Serial.println("INITIALIZED");
        return;
        if (connectionType == SIMPLE_VISOR) {
#if MAX_CONNECTION_TYPE == 3
            for (int x = 0; x < WIDTH; x++) {
                for (int y = 0; y < HEIGHT; y++) {
                    offsets[x][y] = SIMPLE_VISOR_OFFSETS[x][y];
                }
            }
#endif
        } else if (connectionType == BULK2x2) {
            prefillOffsets();
        } else {
            prefillOffsetsDirect();
        }
    }

private:

    int offsets[MAX_WIDTH * 8][MAX_HEIGHT * 8];
    uint8_t maxBuffer[MAX_MATRICES_NUMBER * 8];
    uint8_t matricesNumber = MAX_MATRICES_NUMBER;
    int8_t spiMosi = PIN_OUTPUT_MAX_MOSI;
    int8_t spiClk = PIN_OUTPUT_MAX_CLK;
    int8_t spiSS = PIN_OUTPUT_MAX_CS;
    int _width;
    int _height;
    unsigned long lastStartupCommandsSend = 0;

    bool initialized = false;
    uint8_t fps = MAX_MAXFPS;


    void sendCommand(const uint8_t &address, const uint8_t &value);

    void sendStartupCommands();

    void prefillOffsetsSimpleVisor() {
        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 32; x++) {
                offsets[x][y] = x + (y / 8) * 32;
                if (y > 7 && y < 23) {
                    offsets[x][y] -= 16;
                } else if (y > 24) {
                    offsets[x][y] -= 32;
                }
            }
        }
    }

    void prefillOffsetsDirect() {
        //TODO
    }


    void prefillOffsets() {
        Serial.println("PREFILLING");
        //TODO: Change numbers to being calculated
        for (int i = 0; i < 2; i++) {
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    offsets[x][i * 16 + y] = i * 32 + (y / 8) * 8 + x + (x / 8) * (8);
                    offsets[x][i * 16 + y] = i * 32 + (y / 8) * 8 + x + (x / 8) * (8);
                }
            }
        }
    }


};

#endif
#endif //VISORV3_MAX7219SCREEN_H
