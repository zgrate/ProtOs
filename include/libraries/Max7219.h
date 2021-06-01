//
// Created by dzing on 25/01/2021.
//

#ifndef VISORV3_MAX7219_H
#define VISORV3_MAX7219_H

#include <random>

#define MAX7219_TEST 0x0f // in real code put into a .h file
#define MAX7219_BRIGHTNESS 0x0a // in real code put into a .h file
#define MAX7219_SCAN_LIMIT 0x0b // in real code put into a .h file
#define MAX7219_DECODE_MODE 0x09 // in real code put into a .h file
#define MAX7219_SHUTDOWN 0x0C // in real code put into a .h file

uint8_t testData[] = {85,0,85,0,85,0,85,0,85,0,85,0,85,0,85,0,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,1,2,4,8,16,32,64,128,0,0,0,0,0,0,0,255,128,64,32,16,8,4,2,1,255,0,0,0,0,0,0,0};
/*
 * Used to calculate pixels:
 * DIRECT - connected as 1 long string of matrices
 * BULK2x2 - connected as squares 2x2
 * BULK4x4 - connected as squares 4x4
 */
enum connType {DIRECT, BULK2x2, BULK4x4, BULK6x6, BULK8x8};

#define MAX_DEFAULT_BULK BULK2x2

const uint16_t MAX_OFF_PIXEL = 0;
const uint16_t MAX_ON_PIXEL = 255;
#include <Arduino.h>
#include <Adafruit_GFX.h>

class Max7219 : public Adafruit_GFX
{

public:
    Max7219();
    Max7219(uint8_t width, uint8_t height, uint8_t numberOfMatrices, connType connType);
    //Max7219(uint8_t numberOfMatrices, SPIClass spi, uint8_t mosi, uint8_t clk, uint8_t ss,  connType connectionType);
    ~Max7219(){
        delete _buffer;
    }

    inline void begin();

    void drawTestPattern();

    void drawPixel(int16_t x, int16_t y, uint16_t color) override;

    void display();

    void clearDisplay();

    void test();

    hw_timer_s* timer = NULL;

    bool isDisplayTaskRunning();

    int getFPS();

    void writeFrameFromBuffer(uint8_t *frameStartAddress, int frameLength);

private:

    int offsets[MAX_WIDTH*8][MAX_HEIGHT*8];
    uint8_t* _buffer;
    uint8_t _matricesNumber;
    uint8_t _spiMosi;
    uint8_t _spiClk;
    uint8_t _spiSS;
    uint8_t _height;
    uint8_t _width;
    unsigned long lastStartupCommandsSend = 0;

    bool _initialized = false;
    SPIClass _spiClass;
    connType _connectionType;
    TaskHandle_t maxTaskHandle;
    bool _displayTask;
    uint8_t _fps = MAX_MAXFPS;
    void init(uint8_t numberOfMatrices, uint8_t width, uint8_t height, SPIClass spi, uint8_t mosi, uint8_t clk, uint8_t ss, connType connectionType)
    {
        _matricesNumber = numberOfMatrices;
        _buffer = new uint8_t[numberOfMatrices * 8];
        _spiClass = spi;
        _spiMosi = mosi;
        _spiClk = clk;
        _spiSS = ss;
        _connectionType = connectionType;
        _height = height;
        _width = width;
        WIDTH = _width * 8;
        HEIGHT = _height * 8;

#ifdef MAIN_MAX_MATRIX
        prefillOffsetsDirect();
#else
        prefillOffsets2x2();
#endif

        _initialized = true;
        return;
        switch (connectionType) {
            case DIRECT:
                //TODO
                break;
            case BULK2x2:
                WIDTH = (numberOfMatrices / (height)) * 8;
                HEIGHT = height*8;
                //precalculateBulk2x2Offsets();
                break;
                //TODO: OTHERS
            case BULK4x4:
                break;
            case BULK6x6:
                break;
            case BULK8x8:
                break;
        }
    }

    void sendCommand(uint8_t address, uint8_t value);

    void sendStartupCommands();

    void prefillOffsetsDirect() {
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


    void prefillOffsets2x2() {
        //TODO: Change numbers to being calculated
        for (int i = 0; i < 2; i++) {
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    offsets[x][i*16 + y] = i*32 + (y/8)*8 + x + (x/8)*(8);
                }
            }
        }
//        for(int y = 0; y < HEIGHT; y++)
//        {
//            for(int x = 0; x < WIDTH; x++)
//            {
////                offsets[x][y] = (y/8)*(2*8) + x;
//                offsets[x][y] = (y/8)*8 + x + (x/8)*(8);
//            }
//        }
     //   Serial.println(offsets[16][16]);
    }

    void startDisplayTask();


};

Max7219::Max7219(): Adafruit_GFX(MAX_WIDTH*8, MAX_HEIGHT*8) {
    init(MAX_MATRICES_NUMBER, MAX_WIDTH, MAX_HEIGHT, SPIClass(MAX_SPI_CLASS), PIN_OUTPUT_MAX_MOSI, PIN_OUTPUT_MAX_CLK, PIN_OUTPUT_MAX_CS, BULK2x2);
}

void Max7219::begin() {
    clearDisplay();
    pinMode(_spiClk, OUTPUT);
    pinMode(_spiSS, OUTPUT);
    pinMode(_spiMosi, OUTPUT);
    _spiClass.begin(_spiClk, -1, _spiMosi,  _spiSS);
    _spiClass.setFrequency(MAX_SPI_FREQ);
    _spiClass.setBitOrder(MSBFIRST);
    _spiClass.setDataMode(SPI_MODE0);
    sendStartupCommands();
    _initialized = true;
}

Max7219::Max7219(uint8_t width, uint8_t height, uint8_t numberOfMatrices, connType connType): Adafruit_GFX(0,0) {
    init(numberOfMatrices,width, height,  SPIClass(MAX_SPI_CLASS), PIN_OUTPUT_MAX_MOSI, PIN_OUTPUT_MAX_CLK, PIN_OUTPUT_MAX_CS, connType);

}

void Max7219::sendStartupCommands() {
    lastStartupCommandsSend = millis();
    sendCommand(MAX7219_TEST, 0x00);
    sendCommand(MAX7219_DECODE_MODE, 0x00); // Disable BCD mode.
    sendCommand(MAX7219_BRIGHTNESS, MAX_DEFAULT_BRIGHTNESS);  // Use lowest intensity.
    sendCommand(MAX7219_SCAN_LIMIT, 0x0f);  // Scan all digits.
    sendCommand(MAX7219_SHUTDOWN, 0x01);
}



void Max7219::sendCommand(uint8_t address, uint8_t value) {
    digitalWrite(_spiSS, 0);
    for(int i = 0; i < _matricesNumber; i++)
    {
        _spiClass.transfer(address);
        _spiClass.transfer(value);
    }
    digitalWrite(_spiSS, 1);
}

void Max7219::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (color > 0) {
            _buffer[offsets[x][y]] |= 1 << (y % 8);
        } else {
            _buffer[offsets[x][y]] &= ~(1 << (y % 8));
        }
    }

}

void Max7219::display() {
    pinMode(_spiClk, OUTPUT);
    pinMode(_spiSS, OUTPUT);
    pinMode(_spiMosi, OUTPUT);
//    if(millis()-lastStartupCommandsSend >= MAX_STARTUP_REFRESH)
//    {
//        Serial.println("REFRESH");
//        pinMode(_spiClk, OUTPUT);
//        pinMode(_spiSS, OUTPUT);
//        pinMode(_spiMosi, OUTPUT);
//        sendStartupCommands();
//        delayMicroseconds(100);
//    }
    for (int i = 1; i < 9; i++) {
        digitalWrite(_spiSS, LOW);
        //for(int dev = 0; dev < _width*_height; dev++){
        for (int dev = _matricesNumber - 1; dev >= 0; dev--) {
            _spiClass.transfer(i);
            _spiClass.transfer(_buffer[i - 1 + dev * 8]);
        }
        digitalWrite(_spiSS, HIGH);
    }
}

void Max7219::drawTestPattern() {
    if (_initialized) {
        begin();
    }
    clearDisplay();
    //println("ABC");

    Serial.println("CO JEST");
    for (int i = 0; i < 96; i++) {
        _buffer[i] = millis() % 255;
    }
    display();
    //fillRect(2,2, 10, 10, MAX_ON_PIXEL);
}

void Max7219::clearDisplay() {
    for(int i = 0; i < _matricesNumber*8; i++)
    {
        _buffer[i] = 0;
    }
}


#ifdef MAX_GLOBAL_INSTANCE

Max7219 Max7219ControlInstance = Max7219();


void displayGlobalInstance(void * display)
{
    auto* max = (Max7219*)display;
    while(max->isDisplayTaskRunning())
    {
        max->display();
        delay(1000/max->getFPS());
    }

}

void Max7219::startDisplayTask() {
//        _displayTask = true;
//        xTaskCreatePinnedToCore(
//            &displayGlobalInstance, /* Function to implement the task */
//            "MAX_TASK", /* Name of the task */
//            10000,  /* Stack size in words */
//            this,  /* Task input parameter */
//            0,  /* Priority of the task */
//            &maxTaskHandle,  /* Task handle. */
//            1); /* Core where the task should run */
}

#endif

void Max7219::test() {


    if(_initialized)
        begin();
    #ifdef MAX_FULLPOWER_TEST
    sendCommand(MAX7219_TEST, 0x01);
    return;
    #endif
    startDisplayTask();
    clearDisplay();
    setTextColor(MAX_ON_PIXEL);
    setCursor(0,0);
    print("AB");
    fillRect(1, 9, 6, 6, 1);
    drawTriangle(8,8, 15, 8, 15,15, 1);
    setCursor(0, 16);
    print("CD");
    drawCircle(4, 27, 3, 1);
    drawLine(8, 24, 15, 24, 1);
    drawLine(15, 24, 8, 31, 1);
    //drawTriangle(8, 24, 15, 24, 8,31, 1);
    display();
    Serial.println("MAX7219 initialized. Check if test pattern is on the screen!");
    //display();
}

bool Max7219::isDisplayTaskRunning() {
    return _displayTask;
}

int Max7219::getFPS() {
    return _fps;
}

void Max7219::writeFrameFromBuffer(uint8_t *frameStartAddress, int frameLength) {
//    Serial.print("WRITING ");
//    Serial.print((unsigned long)frameStartAddress);
//    Serial.print(" with length ");
//    Serial.println(frameLength);
//    for(int i = 0 ; i < frameLength; i++)
//    {
////        Serial.print("Before ");
////        Serial.print(_buffer[i]);
//
//        _buffer[i] = *(frameStartAddress+i);
////        Serial.print(" After ");
////        Serial.print(_buffer[i]);
////        Serial.println(" ");
//    }
    memcpy(_buffer, frameStartAddress, frameLength);
    display();
}


#endif //VISORV3_MAX7219_H
