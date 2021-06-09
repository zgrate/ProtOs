//
// Created by dzing on 25/01/2021.
//

#ifndef VISORV3_MAX7219SCREEN_H
#define VISORV3_MAX7219SCREEN_H

#include <random>
#include "ConstantsAndSettings.h"

#define MAX7219_TEST 0x0f // in real code put into a .h file
#define MAX7219_BRIGHTNESS 0x0a // in real code put into a .h file
#define MAX7219_SCAN_LIMIT 0x0b // in real code put into a .h file
#define MAX7219_DECODE_MODE 0x09 // in real code put into a .h file
#define MAX7219_SHUTDOWN 0x0C // in real code put into a .h file

#if MAX_CONNECTION_TYPE == 3

uint8_t SIMPLE_VISOR_OFFSETS[32][24] = {
        {7,6,5,4,3,2,1,0,23,22,21,20,19,18,17,16,71,70,69,68,67,66,65,64},
        {7,6,5,4,3,2,1,0,23,22,21,20,19,18,17,16,71,70,69,68,67,66,65,64},
        {7,6,5,4,3,2,1,0,23,22,21,20,19,18,17,16,71,70,69,68,67,66,65,64},
        {7,6,5,4,3,2,1,0,23,22,21,20,19,18,17,16,71,70,69,68,67,66,65,64},
        {7,6,5,4,3,2,1,0,23,22,21,20,19,18,17,16,71,70,69,68,67,66,65,64},
        {7,6,5,4,3,2,1,0,23,22,21,20,19,18,17,16,71,70,69,68,67,66,65,64},
        {7,6,5,4,3,2,1,0,23,22,21,20,19,18,17,16,71,70,69,68,67,66,65,64},
        {7,6,5,4,3,2,1,0,23,22,21,20,19,18,17,16,71,70,69,68,67,66,65,64},

        {15,14,13,12,11,10,9,8,31,30,29,28,27,26,25,24,79,78,77,76,75,74,73,72},
        {15,14,13,12,11,10,9,8,31,30,29,28,27,26,25,24,79,78,77,76,75,74,73,72},
        {15,14,13,12,11,10,9,8,31,30,29,28,27,26,25,24,79,78,77,76,75,74,73,72},
        {15,14,13,12,11,10,9,8,31,30,29,28,27,26,25,24,79,78,77,76,75,74,73,72},
        {15,14,13,12,11,10,9,8,31,30,29,28,27,26,25,24,79,78,77,76,75,74,73,72},
        {15,14,13,12,11,10,9,8,31,30,29,28,27,26,25,24,79,78,77,76,75,74,73,72},
        {15,14,13,12,11,10,9,8,31,30,29,28,27,26,25,24,79,78,77,76,75,74,73,72},
        {15,14,13,12,11,10,9,8,31,30,29,28,27,26,25,24,79,78,77,76,75,74,73,72},

        {55,54,53,52,51,50,49,48,39,38,37,36,35,34,33,32,87,86,85,84,83,82,81,80},
        {55,54,53,52,51,50,49,48,39,38,37,36,35,34,33,32,87,86,85,84,83,82,81,80},
        {55,54,53,52,51,50,49,48,39,38,37,36,35,34,33,32,87,86,85,84,83,82,81,80},
        {55,54,53,52,51,50,49,48,39,38,37,36,35,34,33,32,87,86,85,84,83,82,81,80},
        {55,54,53,52,51,50,49,48,39,38,37,36,35,34,33,32,87,86,85,84,83,82,81,80},
        {55,54,53,52,51,50,49,48,39,38,37,36,35,34,33,32,87,86,85,84,83,82,81,80},
        {55,54,53,52,51,50,49,48,39,38,37,36,35,34,33,32,87,86,85,84,83,82,81,80},
        {55,54,53,52,51,50,49,48,39,38,37,36,35,34,33,32,87,86,85,84,83,82,81,80},

        {63,62,61,60,59,58,57,56,47,46,45,44,43,42,41,40,95,94,93,92,91,90,89,88},
        {63,62,61,60,59,58,57,56,47,46,45,44,43,42,41,40,95,94,93,92,91,90,89,88},
        {63,62,61,60,59,58,57,56,47,46,45,44,43,42,41,40,95,94,93,92,91,90,89,88},
        {63,62,61,60,59,58,57,56,47,46,45,44,43,42,41,40,95,94,93,92,91,90,89,88},
        {63,62,61,60,59,58,57,56,47,46,45,44,43,42,41,40,95,94,93,92,91,90,89,88},
        {63,62,61,60,59,58,57,56,47,46,45,44,43,42,41,40,95,94,93,92,91,90,89,88},
        {63,62,61,60,59,58,57,56,47,46,45,44,43,42,41,40,95,94,93,92,91,90,89,88},
        {63,62,61,60,59,58,57,56,47,46,45,44,43,42,41,40,95,94,93,92,91,90,89,88}
};
//{0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,8,48,48,48,48,48,48,48,48,56,56,56,56,56,56,56,56 }

//[0][1-8] - 0
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

    inline void begin() override;

    void drawTestPattern();

    void drawPixel(int16_t x, int16_t y, uint16_t color) override;

    void display();

    void clear() override;

    void test();

    hw_timer_s *timer = NULL;

    bool isDisplayTaskRunning() const;

    int getFPS() const;

    void writeFrameFromBuffer(const uint8_t *frameStartAddress, const int &frameLength) override;

    void drawPixels(const std::vector<Pixel> &vector) override;

    void setBrightness(const uint8_t &target) override;

    void init(uint8_t numberOfMatrices, uint8_t width, uint8_t height, uint8_t mosi, uint8_t clk, uint8_t ss,
              ConnectionType connectionType) {
        _matricesNumber = numberOfMatrices;
        //_buffer = new uint8_t[numberOfMatrices * 8];
        //Serial.println("BUFFER WRITING COMPLETED! RESULT: " + String(_buffer == nullptr));

        _spiMosi = mosi;
        _spiClk = clk;
        _spiSS = ss;
        _connectionType = connectionType;
        _height = height;
        _width = width;
        WIDTH = _width * 8;
        HEIGHT = _height * 8;

        if (connectionType == SIMPLE_VISOR) {
#if MAX_CONNECTION_TYPE == 3
            for(int x = 0; x < WIDTH; x++){
                for(int y = 0; y < HEIGHT; y++){
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
    uint8_t _buffer[MAX_MATRICES_NUMBER * 8];
    uint8_t _matricesNumber;
    uint8_t _spiMosi;
    uint8_t _spiClk;
    uint8_t _spiSS;
    uint8_t _height;
    uint8_t _width;
    unsigned long lastStartupCommandsSend = 0;

    bool _initialized = false;
    ConnectionType _connectionType;
    TaskHandle_t maxTaskHandle;
    bool _displayTask;
    uint8_t _fps = MAX_MAXFPS;


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


    void prefillOffsets(const int &squareLength = 2) {
        //TODO: Change numbers to being calculated
        int squarePixelsLength = squareLength * 8;
        int totalPixelsPerBulk = squarePixelsLength * squareLength;
        int numberOfBulks = this->_matricesNumber / 4;
        for (int i = 0; i < numberOfBulks; i++) {
            for (int y = 0; y < squarePixelsLength; y++) {
                for (int x = 0; x < squarePixelsLength; x++) {
                    offsets[x][i * squarePixelsLength + y] =
                            i * totalPixelsPerBulk + ((y / 8) * 8) + x + ((x / 8) * (8));
                }
            }
        }
    }


};


#endif //VISORV3_MAX7219SCREEN_H
