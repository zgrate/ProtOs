/*********************************************************************
This is a library for Chinese LED matrix displays

Written by Dominic Buchstaller.
Modified by Z-Grate
BSD license
*********************************************************************/


#ifndef _PxMATRIX_H
#define _PxMATRIX_H
#include "ConstantsAndSettings.h"

#ifdef PX_MATRIX_SCREEN

#ifndef _BV
#define _BV(x) (1 << (x))
#endif

#if defined(ESP8266) || defined(ESP32)
#define SPI_TRANSFER(x, y) spiClass.writeBytes(x,y)
#define SPI_BYTE(x) spiClass.write(x)
#define SPI_2BYTE(x) spiClass.write16(x)
#endif

#include "Adafruit_GFX.h"
#include "Arduino.h"
#include <SPI.h>

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#else
#include "WProgram.h"
#endif

#include <cstdlib>
#include "ConstantsAndSettings.h"
#include "SD_MMC.h"
#include "control/Definitions.h"

// Sometimes some extra width needs to be passed to Adafruit GFX constructor
// to render text close to the end of the display correctly
#ifndef ADAFRUIT_GFX_EXTRA
#define ADAFRUIT_GFX_EXTRA 0
#endif


#ifdef ESP32
#define GPIO_REG_SET(val) GPIO.out_w1ts = val
#define GPIO_REG_CLEAR(val) GPIO.out_w1tc = val
#endif

#ifdef ESP32

#include "soc/spi_struct.h"
#include "esp32-hal-gpio.h"

struct spi_struct_t {
    spi_dev_t *dev;
#if !CONFIG_DISABLE_HAL_LOCKS
    xSemaphoreHandle lock;
#endif
    uint8_t num;
};
#endif

// Either the panel handles the multiplexing and we feed BINARY to A-E pins
// or we handle the multiplexing and activate one of A-D pins (STRAIGHT)
enum mux_patterns {
    BINARY, STRAIGHT
};

// This is how the scanning is implemented. LINE just scans it left to right,
// ZIGZAG jumps 4 rows after every byte, ZAGGII alse revereses every second byte
enum scan_patterns {
    LINE, ZIGZAG, ZZAGG, ZAGGIZ, WZAGZIG, VZAG, ZAGZIG
};

// Specify s speciffic driver chip. Most panels implement a standard shifted
// register (SHIFT). Other chips/panels may need special treatment in oder to work
enum driver_chips {
    SHIFT, FM6124, FM6126A
};

//Specifies enum for free bites in register
enum free_pin {
    BIT_5, BIT_6, BIT_7, BIT_8
};


#define max_matrix_pixels (PxMATRIX_HEIGHT * PxMATRIX_WIDTH)
#define color_step (256 / PxMATRIX_COLOR_DEPTH)
#define color_half_step (int(color_step / 2))
#define color_third_step (int(color_step / 3))
#define color_two_third_step (int(color_third_step*2))

#define buffer_size (max_matrix_pixels * 3 / 8)

using namespace std;


class PxMatrixScreen : public Adafruit_GFX, public Screen {
public:

    PxMatrixScreen(const uint16_t &width, const uint16_t height, const uint8_t &mosi, const uint8_t &clk, const uint8_t &latch, const uint8_t &oe,
                   const uint8_t &regLatch);

    void begin(const uint8_t &row_pattern);

    void begin() override;

    void clear() override;

    // Updates the display
    void display(uint16_t show_time);

    void display() {
        display(drawTime);
    }

    // Draw pixels
    void drawPixelRGB565(const int16_t &x, const int16_t &y, const uint16_t &color);

    void drawPixel(int16_t x, int16_t y, uint16_t color) override;

    void drawPixelRGB888(const int16_t &x, const int16_t &y, const uint8_t &r, const uint8_t &g, const uint8_t &b);

    // Converts RGB888 to RGB565
    uint16_t color565(const uint8_t &r, const uint8_t &g, const uint8_t &b);

    // FLush the buffer of the display
    void flushDisplay();

    // Rotate display
    void setRotate(const bool &rotate);

    // Flip display
    void setFlip(const bool &flip);

    // Helps to reduce display update latency on larger displays
    void setFastUpdate(const bool &fast_update);

    // Control the minimum color values that result in an active pixel
    void setColorOffset(uint8_t r, uint8_t g, uint8_t b);


    // Set the time in microseconds that we pause after selecting each mux channel
    // (May help if some rows are missing / the mux chip is too slow)
    void setMuxDelay(const uint8_t &muxDelay);

//  inline void setRegister(uint8_t number);

    // Set the multiplex pattern {LINE, ZIGZAG, ZAGGIZ, WZAGZIG, VZAG} (default is LINE)
    void setScanPattern(const scan_patterns &scanPattern);

    // Set the number of panels that make up the display area width (default is 1)
    void setPanelsWidth(const uint8_t &panels);

    // Set the b of the panels (default is 255)
    void setBrightness(const uint8_t &b) override {
        this->brightness = b;
    }


    inline void setDrawTime(const uint16_t &drawtime);

    // Set driver chip type
    inline void setDriverChip(const driver_chips &driverChip);
    /**
     * Set state pins of shift register
     * @param pin to set
     * @param state new state of the pin - 1 - enabled, 0 - disabled
     */
    void setPinState(const free_pin &pin, const bool &state);

// Some standard colors
    uint16_t red = color565(255, 0, 0);
    uint16_t green = color565(0, 0, 255);
    uint16_t blue = color565(0, 255, 0);
    uint16_t white = color565(255, 255, 255);
    uint16_t yellow = color565(255, 0, 255);
    uint16_t cyan = color565(0, 255, 255);
    uint16_t magenta = color565(255, 255, 0);
    uint16_t black = color565(0, 0, 0);

    uint16_t colors[8] = {black, white, red, green, blue, yellow, cyan, magenta};

    inline uint16_t getDrawTime() const;

    void setFrequency(const int &frequency);

    void test();

    void writeFrameFromBuffer(const uint8_t *buffer, const int &bufferLength) override;

    uint8_t *getBufferAddress();

    void drawPixels(const vector<Pixel> &vector) override;

    hw_timer_t * timer = NULL;
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
private:
//buffersize = (32 * 128)*3/8
    SPIClass spiClass = SPIClass(PxMATRIX_SPICLASS);

    // the display buffer for the LED matrix
    uint8_t PxMATRIX_buffer[PxMATRIX_COLOR_DEPTH][buffer_size];
#ifdef PxMATRIX_double_buffer
    uint8_t PxMATRIX_buffer2[PxMATRIX_COLOR_DEPTH][buffer_size];
#endif

    bool writing = false;
    // GPIO pins
    uint8_t latchPin = PIN_OUTPUT_PX_STROBO;
    uint8_t oePin = PIN_OUTPUT_PX_OE;
    uint8_t regLatchPin = PIN_OUTPUT_PX_REGLATCH;


    // spiClass pins
    uint8_t spiClk = PIN_OUTPUT_PX_CLK;
    uint8_t spiMosi = PIN_OUTPUT_PX_MOSI;
    uint8_t spiSs = PIN_OUTPUT_PX_STROBO;

    uint16_t drawTime = PxMATRIX_SHOWTIME;


    uint8_t otherPinsState = B00000000;

    uint8_t _width;
    uint8_t _height;
    uint8_t panelsWidth;
    uint8_t rowsPerBuffer;
    uint8_t rowSetsPerBuffer;
    uint8_t panelWidthBytes;


    // Color offset
    uint8_t colorROffset;
    uint8_t colorGOffset;
    uint8_t colorBOffset;

    // Panel Brightness
    uint8_t brightness = 255;

    // Color pattern that is pushed to the display
    uint8_t displayColor;

    // Holds some pre-computed values for faster pixel drawing
    uint32_t rowOffset[PxMATRIX_HEIGHT];

    // Holds the display row pattern type
    uint8_t rowPattern;

    // Number of bytes in one color
    uint8_t patternColorBytes;

    // Total number of bytes that is pushed to the display at a time
    // 3 * patternColorBytes
    uint16_t sendBufferSize;

    AnimationMode animationMode = AnimationMode::ANIMATION_FROM_FILE;


    // Hols configuration
    bool isRotated;
    bool isFlipped;
    bool fastUpdate = true;

    //Delay of register of muxes
    uint8_t _mux_delay;

    // Holds the scan pattern
    scan_patterns scanPattern;

    // Holds the used driver chip
    driver_chips driverChip;

    // Used for test pattern
    uint16_t testPixelCounter;
    uint16_t testLineCounter;
    unsigned long testLastCall;
    bool initialized = false;

    // Generic function that draw one pixel
    void fillMatrixBuffer(int16_t x, const int16_t &y, const uint8_t &r, const uint8_t &g, const uint8_t &b);

    // Light up LEDs and hold for show_time microseconds
    void latch(const uint16_t &show_time);

    void latch(const uint16_t &showTime, const uint8_t &value);

    // Set row multiplexer
    void setMux(uint8_t value);

    void spiInit();

    void fm612xWriteRegister(const uint16_t &regValue, const uint8_t &regPosition);

    void fullBrightness();

};

class CyclicRGBBuffer {

    uint8_t *buffer;
    uint32_t length;
    uint32_t frameLength;

    uint8_t *readPointer;
    uint8_t *writePointer;

    String file_path;

    File fileInstance;

    uint32_t currentFilePointer;
    uint32_t numberOfFrames;

    CyclicRGBBuffer() {
        frameLength = buffer_size;
        length = buffer_size * 30;
        buffer = (uint8_t *) malloc(length * 8);
        readPointer = buffer;
        writePointer = buffer + frameLength;
        file_path = "";
    }

    ~CyclicRGBBuffer() {
        free(buffer);
    }

    inline void setFile(const String &file) {
        file_path = file;
    }

    inline uint8_t *getReadPointer() const {
        return readPointer;
    }

    inline uint32_t getFrameLength() const {
        return frameLength;
    }

    void ensureCorrectFile() {

        fileInstance = SD_MMC.open(file_path);

    }

    void readFully() {

    }

    void readNextFrame() {

    }

};



//void writeToRegisterPin(uint8_t pin, uint8_t state) {
//    //TODO
//    PxMatrixControlInstance.setPinState(static_cast<free_pin>(pin), state);
//}





//void IRAM_ATTR old_display_updater(void * display){
//    for(;;)
//    {
//        PxMatrixClient.display(PxMatrixClient.getDrawTime());
//
//    }
//}

//void threadDisplay(void *pvParameters) {
////    for (;;) {
////        PxMatrixControlInstance.display(PxMatrixControlInstance.getDrawTime());
////        yield();
////    }
//}
//
//TaskHandle_t handle;
//



//
//
////    xTaskCreatePinnedToCore(
////            threadDisplay, /* Function to implement the task */
////            "Screen Task", /* Name of the task */
////            10000,  /* Stack size in words */
////            NULL,  /* Task input parameter */
////            0,  /* Priority of the task */
////            &handle,  /* Task handle. */
////            0); /* Core where the task should run */
//
//
//}


#endif

#endif