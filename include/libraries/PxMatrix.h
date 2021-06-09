/*********************************************************************
This is a library for Chinese LED matrix displays

Written by Dominic Buchstaller.
Modified by Z-Grate
BSD license, check license.txt for more information
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


#ifdef ESP8266
#define GPIO_REG_SET(val) GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS,val)
#define GPIO_REG_CLEAR(val) GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS,val)
#endif
#ifdef ESP32
#define GPIO_REG_SET(val) GPIO.out_w1ts = val
#define GPIO_REG_CLEAR(val) GPIO.out_w1tc = val
#endif
#ifdef __AVR__
#define GPIO_REG_SET(val) (val < 8) ? PORTD |= _BV(val) : PORTB |= _BV(val-8)
#define GPIO_REG_CLEAR(val) (val < 8) ? PORTD &= ~_BV(val) : PORTD &= ~_BV(val-8)
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

// HW spiClass PINS
#define SPI_BUS_CLK 18
#define SPI_BUS_MOSI 23
#define SPI_BUS_SS 5
#define PX_REG 5

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

    PxMatrixScreen(uint16_t width, uint16_t height, uint8_t MOSI, uint8_t CLK, uint8_t LATCH, uint8_t OE,
                   uint8_t REG_LATCH);

    void begin(uint8_t row_pattern);

    void begin() override;

    void clear() override;

    // Updates the display
    void display(uint16_t show_time);

    void display() {
        display(_draw_time);
    }

    // Draw pixels
    inline void drawPixelRGB565(int16_t x, int16_t y, uint16_t color);

    inline void drawPixel(int16_t x, int16_t y, uint16_t color) override;

    inline void drawPixelRGB888(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b);

    // Does nothing for now (always returns 0)
    uint8_t getPixel(int8_t x, int8_t y);

    // Converts RGB888 to RGB565
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

    // Helpful for debugging (place in display update loop)
    //inline void displayTestPattern(uint16_t showtime);

    // Helpful for debugging (place in display update loop)
    //inline void displayTestPixel(uint16_t show_time);

    // FLush the buffer of the display
    inline void flushDisplay();

    // Rotate display
    inline void setRotate(bool rotate);

    // Flip display
    inline void setFlip(bool flip);

    // Helps to reduce display update latency on larger displays
    inline void setFastUpdate(bool fast_update);

    // When using double buffering, this displays the draw buffer
    inline void showBuffer();

    // Control the minimum color values that result in an active pixel
    inline void setColorOffset(uint8_t r, uint8_t g, uint8_t b);

    // Set the multiplex implemention {BINARY, STRAIGHT} (default is BINARY)
    inline void setMuxPattern(mux_patterns mux_pattern);

    // Set the time in microseconds that we pause after selecting each mux channel
    // (May help if some rows are missing / the mux chip is too slow)
    inline void setMuxDelay(uint8_t mux_delay);

//  inline void setRegister(uint8_t number);

    // Set the multiplex pattern {LINE, ZIGZAG, ZAGGIZ, WZAGZIG, VZAG} (default is LINE)
    inline void setScanPattern(scan_patterns scan_pattern);

    // Set the number of panels that make up the display area width (default is 1)
    inline void setPanelsWidth(uint8_t panels);

    // Set the brightness of the panels (default is 255)
    void setBrightness(const uint8_t &brightness) override;

    inline void setDrawTime(uint16_t drawtime);

    // Set driver chip type
    inline void setDriverChip(driver_chips driver_chip);

    inline void setPinState(free_pin pin, uint8_t state);

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

    void setFrequency(int frequency);

    void test();

    void writeFrameFromBuffer(const uint8_t *buffer, const int &bufferLength) override;

    uint8_t *getBufferAddress();

    void drawPixels(const vector<Pixel> &vector) override;

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
    uint8_t _LATCH_PIN;
    uint8_t _OE_PIN;
    uint8_t _REG_LATCH_PIN = PX_REG;


    // spiClass pins
    uint8_t _SPI_CLK = SPI_BUS_CLK;
    uint8_t _SPI_MOSI = SPI_BUS_MOSI;
    uint8_t _SPI_SS = SPI_BUS_SS;

    uint16_t _draw_time = PxMATRIX_SHOWTIME;


    uint8_t _other_pins_state = B00000000;

    uint8_t _width;
    uint8_t _height;
    uint8_t _panels_width;
    uint8_t _rows_per_buffer;
    uint8_t _row_sets_per_buffer;
    uint8_t _panel_width_bytes;

    bool taskStarted = false;

    // Color offset
    uint8_t _color_R_offset;
    uint8_t _color_G_offset;
    uint8_t _color_B_offset;

    // Panel Brightness
    uint8_t _brightness = 255;

    // Color pattern that is pushed to the display
    uint8_t _display_color;

    // Holds some pre-computed values for faster pixel drawing
    uint32_t _row_offset[PxMATRIX_HEIGHT];

    // Holds the display row pattern type
    uint8_t _row_pattern;

    // Number of bytes in one color
    uint8_t _pattern_color_bytes;

    // Total number of bytes that is pushed to the display at a time
    // 3 * _pattern_color_bytes
    uint16_t _send_buffer_size;

    AnimationMode animationMode = AnimationMode::ANIMATION_FROM_FILE;

    // This is for double buffering
    bool _active_buffer;

    // Hols configuration
    bool _rotate;
    bool _flip;
    bool _fast_update = true;

    // Holds multiplex pattern
    mux_patterns _mux_pattern;

    //Delay of register of muxes
    uint8_t _mux_delay;

    //Task that handles matrix display refresh
    hw_timer_t *timer = nullptr;


    // Holds the scan pattern
    scan_patterns _scan_pattern;

    // Holds the used driver chip
    driver_chips _driver_chip;

    // Used for test pattern
    uint16_t _test_pixel_counter;
    uint16_t _test_line_counter;
    unsigned long _test_last_call;
    bool _initialized = false;

    // Generic function that draw one pixel
    inline void fillMatrixBuffer(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b, bool selected_buffer);

    // Init code common to both constructors
    inline void
    init(uint16_t width, uint16_t height, uint8_t MOSI, uint8_t CLK, uint8_t LATCH, uint8_t OE, uint8_t ABCDE_LATCH);

    // Light up LEDs and hold for show_time microseconds
    inline void latch(uint16_t show_time);

    inline void latch(uint16_t show_time, uint8_t value);

    // Set row multiplexer
    inline void set_mux(uint8_t value);

    inline void spi_init();

// Write configuration register in some driver chips
    inline void writeRegister(uint16_t reg_value, uint8_t reg_position);

    inline void fm612xWriteRegister(uint16_t reg_value, uint8_t reg_position);

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


//hw_timer_t * timer = NULL;
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//void IRAM_ATTR display_updater() {
//    // Increment the counter and set the time of ISR
//    portENTER_CRITICAL_ISR(&timerMux);
//    PxMatrixControlInstance.display(PxMatrixControlInstance.getDrawTime());
//    //   display.displayTestPattern(70);
//    portEXIT_CRITICAL_ISR(&timerMux);
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
//void PxMatrixScreen::startDisplayThread() {
//    if (!_initialized)
//        begin();
//    timer = timerBegin(0, 80, true);
//    timerAttachInterrupt(timer, &display_updater, true);
//    timerAlarmWrite(timer, PxMATRIX_INTERRUPT_TIMER, true);
//    timerAlarmEnable(timer);
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