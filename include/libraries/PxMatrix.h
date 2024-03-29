/*********************************************************************
This is a library for Chinese LED matrix displays

Written by Dominic Buchstaller.
Modified by Z-Grate
BSD license, check license.txt for more information
*********************************************************************/


#ifndef _PxMATRIX_H
#define _PxMATRIX_H

#ifdef PX_MATRIX_SCREEN
// Legacy suppport
#ifdef double_buffer
#define PxMATRIX_double_buffer true
#endif

#ifndef _BV
#define _BV(x) (1 << (x))
#endif

#if defined(ESP8266) || defined(ESP32)
#define SPI_TRANSFER(x, y) spiClass.writeBytes(x,y)
#define SPI_BYTE(x) spiClass.write(x)
#define SPI_2BYTE(x) spiClass.write16(x)
#endif

#ifdef __AVR__
#define SPI_TRANSFER(x,y) spiClass.transfer(x,y)
#define SPI_BYTE(x) spiClass.transfer(x)
#define SPI_2BYTE(x) spiClass.transfer16(x)
#endif

#include "Adafruit_GFX.h"
#include "Arduino.h"
#include <SPI.h>

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#else
#include "WProgram.h"
#endif

#ifdef __AVR__
#include <util/delay.h>
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

SPIClass spiClass(PxMATRIX_SPICLASS);


class PxMATRIX : public Adafruit_GFX, public Screen {
public:
    inline PxMATRIX(uint16_t width, uint16_t height, uint8_t MOSI, uint8_t CLK, uint8_t LATCH, uint8_t OE,
                    uint8_t REG_LATCH);

    inline void begin(uint8_t row_pattern);

    inline void begin() override;

    inline void clear() override;

    inline void clearDisplay(bool selected_buffer);

    // Updates the display
    inline void display(uint16_t show_time);

    inline void display();

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
    inline void setBrightness(const uint8_t &brightness) override;

    inline void setDrawTime(uint16_t drawtime);

    // Set driver chip type
    inline void setDriverChip(driver_chips driver_chip);

    inline void setPinState(free_pin pin, uint8_t state);

    inline void startDisplayThread();

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


CyclicRGBBuffer cyclicBuffer();

void defaultDelay(uint16_t delayTime) {
    delayMicroseconds(delayTime);
}

void (*delayFunction)(uint16_t) = &defaultDelay;

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
inline uint16_t PxMATRIX::color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Init code common to both constructors
inline void PxMATRIX::init(uint16_t width, uint16_t height, uint8_t MOSI, uint8_t CLK, uint8_t LATCH, uint8_t OE,
                           uint8_t ABCD_LATCH) {
    _SPI_MOSI = MOSI;
    _SPI_CLK = CLK;
    _LATCH_PIN = LATCH;
    _OE_PIN = OE;
    _display_color = 0;
    _SPI_SS = LATCH;
    _REG_LATCH_PIN = ABCD_LATCH;


    if (width > PxMATRIX_WIDTH) {
#ifdef DEBUG_ESP_PORT
        DEBUG_ESP_PORT.print("[PxMatrix] Width larger than PxMATRIX_WIDTH.\n");
#endif
    }

    if (height > PxMATRIX_HEIGHT) {
#ifdef DEBUG_ESP_PORT
        DEBUG_ESP_PORT.print("[PxMatrix] Height larger than PxMATRIX_HEIGHT.\n");
#endif
    }

    _width = width;
    _height = height;
    _brightness = PxMATRIX_DEFAULT_BRIGHTNESS;
    _panels_width = PxMatrix_WIDTH_NUMBER;

    _rows_per_buffer = _height / 2;
    _panel_width_bytes = (_width / _panels_width) / 8;

    _active_buffer = false;

    _color_R_offset = 0;
    _color_G_offset = 0;
    _color_B_offset = 0;

    _test_last_call = 0;
    _test_pixel_counter = 0;
    _test_line_counter = 0;
    _rotate = 0;
    _flip = 0;
    _fast_update = true;

    _row_pattern = 0;
    _scan_pattern = LINE;
    _driver_chip = SHIFT;

    _mux_delay = 0;

    // setPanelsWidth(PxMatrix_WIDTH_NUMBER);
    clear();
#ifdef PxMATRIX_double_buffer
    clearDisplay(1);
#endif
}

#ifdef ESP32

inline void PxMATRIX::fm612xWriteRegister(uint16_t reg_value, uint8_t reg_position) {
    spi_t *spibus = spiClass.bus();
    // reg_value = 0x1234;  debug

    for (int i = 0; i < 47; i++)
        SPI_2BYTE(reg_value);

    spiSimpleTransaction(spibus);

    spibus->dev->mosi_dlen.usr_mosi_dbitlen = 16 - reg_position - 1;
    spibus->dev->miso_dlen.usr_miso_dbitlen = 0;
    spibus->dev->data_buf[0] = reg_value >> 8;
    spibus->dev->cmd.usr = 1;
    while (spibus->dev->cmd.usr);

    GPIO_REG_SET(1 << _LATCH_PIN);

    spibus->dev->mosi_dlen.usr_mosi_dbitlen = (reg_position - 8) - 1;
    spibus->dev->data_buf[0] = reg_value >> (reg_position - 8);
    spibus->dev->cmd.usr = 1;
    while (spibus->dev->cmd.usr);
    spiEndTransaction(spibus);

    SPI_BYTE(reg_value & 0xff);

    GPIO_REG_CLEAR(1 << _LATCH_PIN);

}

#else
inline void PxMATRIX::writeRegister(uint16_t reg_value, uint8_t reg_position)
{
  if (_driver_chip == FM6124 || _driver_chip == FM6126A){

    if (_driver_chip == FM6124) {
       Serial.println("\nFM6124 - REG: " + String(reg_position));
    } else {
       Serial.println("\nFM6126A - REG: " + String(reg_position));
    }

    // All FM6126A code is based on the excellent guesswork by shades66 in https://github.com/hzeller/rpi-rgb-led-matrix/issues/746
    // Register 12 - brightness/gain settings, three 6bit values, aaaaaabbbbbbcccccc a= darkness?
    //               seems to add red to the background when the leds are off, b=main brightness c=finer brightness
    //               (i'm not sure if b & c are actually as 12 bit value but with b set to all 1's the value in c doesn't seem to make much difference)

    // Register 13 - not sure what it's doing yet, just that 1 specific bit within seems to be an overall enable function.

    // Now set all the values at the top to the same value for each of register 12/13 to get the same settings across the panel, the current code loads different settings into each 32 columns.
    // clocking in the register is simply clocking in the value (i've 2 panels so 128bits of data) and for the last 12/13 bits depending on the register setting the latch to high. the final drop of latch to low clocks in the configuration. this is done by sending the same value to r1/r2/g1/g2/b1/b2 at the same time to load the config into all the FM6126 chips

    // Some necessary magic bit fields
    // b12  - 1  adds red tinge
    // b12  - 9/8/7/6/5  =  4 bit brightness
    // b13  - 9   =1 screen on
    // b13  - 6   =1 screen off
    pinMode(_SPI_CLK,OUTPUT);
    pinMode(_SPI_MOSI,OUTPUT);
    digitalWrite(_SPI_CLK,HIGH); // CCK LOW
    digitalWrite(_OE_PIN,LOW);
    digitalWrite(_LATCH_PIN,HIGH);
    digitalWrite(_A_PIN,HIGH);
    digitalWrite(_B_PIN,LOW);
    digitalWrite(_C_PIN,LOW);
    digitalWrite(_D_PIN,LOW);

    uint8_t reg_bit=0;
    for (uint32_t bit_counter=0; bit_counter < _send_buffer_size*8; bit_counter++)
    {
      reg_bit=bit_counter%16;
      if ((reg_value>>reg_bit)&1)
        digitalWrite(_SPI_MOSI,HIGH);
      else
        digitalWrite(_SPI_MOSI,LOW);

      delay(1);
      digitalWrite(_SPI_CLK,LOW); // CLK HIGH
      delay(1);
      digitalWrite(_SPI_CLK,HIGH); // CLK LOW
      delay(1);
      if ((bit_counter ==  (_send_buffer_size*8 - reg_position-1)))
      {
        digitalWrite(_LATCH_PIN,LOW);
      }
    }
    digitalWrite(_LATCH_PIN,HIGH);
  }
  digitalWrite(_OE_PIN,HIGH);

}
#endif

inline void PxMATRIX::setDriverChip(driver_chips driver_chip) {
    _driver_chip = driver_chip;

    if (driver_chip == FM6124 || driver_chip == FM6126A) {

        uint16_t b12a = 0b0111111111111111; //亮度: high
        b12a = 0b0111100011111111; //亮度: low
        // uint16_t b12b=0b0111100000111111;
        // uint16_t b12c=0b0111111111111111;
        // uint16_t b12d=0b0111100000111111;

        uint16_t b13a = 0b0000000001000000;
        // uint16_t b13b=0b0000000001000000;
        // uint16_t b13c=0b0000000001000000;
        // uint16_t b13d=0b0000000001000000;

#ifdef ESP32
        pinMode(_OE_PIN, OUTPUT);
        pinMode(_LATCH_PIN, OUTPUT);
        digitalWrite(_OE_PIN, HIGH);
        pinMode(_LATCH_PIN, LOW);

        fm612xWriteRegister(b12a, 11);
        fm612xWriteRegister(b13a, 12);

#else
        writeRegister(b12a, 12);
        writeRegister(b13a, 13);
#endif


    }
}

inline void PxMATRIX::setMuxPattern(mux_patterns mux_pattern) {
    _mux_pattern = mux_pattern;

    // We handle the multiplexing in the library and activate one of for
    // row drivers --> need A,B,C,D pins
//  if (_mux_pattern==STRAIGHT)
//  {
//    //pinMode(_C_PIN, OUTPUT);
//    //pinMode(_D_PIN, OUTPUT);
//  }
}


inline void PxMATRIX::setMuxDelay(uint8_t mux_delay) {
    _mux_delay = mux_delay;
}

inline void PxMATRIX::setScanPattern(scan_patterns scan_pattern) {
    _scan_pattern = scan_pattern;
}

inline void PxMATRIX::setPanelsWidth(uint8_t panels) {
    _panels_width = panels;
    _panel_width_bytes = (_width / _panels_width) / 8;
}

inline void PxMATRIX::setRotate(bool rotate) {
    _rotate = rotate;
}

inline void PxMATRIX::setFlip(bool flip) {
    _flip = flip;
}

inline void PxMATRIX::setFastUpdate(bool fast_update) {
    _fast_update = fast_update;
}

inline void PxMATRIX::setBrightness(const uint8_t &brightness) {
    _brightness = brightness;
}


inline PxMATRIX::PxMATRIX(uint16_t width, uint16_t height, uint8_t MOSI, uint8_t CLK, uint8_t LATCH, uint8_t OE,
                          uint8_t ABCDE_LATCH)
        : Adafruit_GFX(width + ADAFRUIT_GFX_EXTRA, height) {
    init(width, height, MOSI, CLK, LATCH, OE, ABCDE_LATCH);
}

inline void PxMATRIX::drawPixel(int16_t x, int16_t y, uint16_t color) {
    drawPixelRGB565(x, y, color);
}

inline void PxMATRIX::showBuffer() {
    _active_buffer = !_active_buffer;
}

inline void PxMATRIX::setColorOffset(uint8_t r, uint8_t g, uint8_t b) {
    if ((color_half_step + r) < 0)
        r = -color_half_step;
    if ((color_half_step + r) > 255)
        r = 255 - color_half_step;

    if ((color_half_step + g) < 0)
        g = -color_half_step;
    if ((color_half_step + g) > 255)
        g = 255 - color_half_step;

    if ((color_half_step + b) < 0)
        b = -color_half_step;
    if ((color_half_step + b) > 255)
        b = 255 - color_half_step;

    _color_R_offset = r;
    _color_G_offset = g;
    _color_B_offset = b;
}

inline void PxMATRIX::fillMatrixBuffer(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b, bool selected_buffer) {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
        return;
//    if (_rotate) {
//        uint16_t temp_x = x;
//        x = y;
//        y = _height - 1 - temp_x;
//    }


    if (!_flip)
        x = _width - 1 - x;

    uint32_t base_offset;
    uint32_t total_offset_r = 0;
    uint32_t total_offset_g = 0;
    uint32_t total_offset_b = 0;

//    if (_scan_pattern == WZAGZIG || _scan_pattern == VZAG) {
//        // get block coordinates and constraints
//        uint8_t rows_per_buffer = _height / 2;
//        uint8_t rows_per_block = rows_per_buffer / 2;
//        // this is a defining characteristic of WZAGZIG and VZAG:
//        // two byte alternating chunks bottom up for WZAGZIG
//        // two byte up down down up for VZAG
//        uint8_t cols_per_block = 16;
//        uint8_t panel_width = _width / _panels_width;
//        uint8_t blocks_x_per_panel = panel_width / cols_per_block;
//        uint8_t panel_index = x / panel_width;
//        // strip down to single panel coordinates, restored later using panel_index
//        x = x % panel_width;
//        uint8_t base_y_offset = y / rows_per_buffer;
//        uint8_t buffer_y = y % rows_per_buffer;
//        uint8_t block_x = x / cols_per_block;
//        uint8_t block_x_mod = x % cols_per_block;
//        uint8_t block_y = buffer_y / rows_per_block; // can only be 0/1 for height/pattern=4
//        uint8_t block_y_mod = buffer_y % rows_per_block;
//
//        // translate block address to new block address
//        // invert block_y so remaining translation will be more sane
//        uint8_t block_y_inv = 1 - block_y;
//        uint8_t block_x_inv = blocks_x_per_panel - block_x - 1;
//        uint8_t block_linear_index;
//        if (_scan_pattern == WZAGZIG) {
//            // apply x/y block transform for WZAGZIG, only works for height/pattern=4
//            block_linear_index = block_x_inv * 2 + block_y_inv;
//        } else if (_scan_pattern == VZAG) {
//            // apply x/y block transform for VZAG, only works for height/pattern=4 and 32x32 panels until a larger example is found
//            block_linear_index = block_x_inv * 3 * block_y + block_y_inv * (block_x_inv + 1);
//        }
//        // render block linear index back into normal coordinates
//        uint8_t new_block_x = block_linear_index % blocks_x_per_panel;
//        uint8_t new_block_y = 1 - block_linear_index / blocks_x_per_panel;
//        x = new_block_x * cols_per_block + block_x_mod + panel_index * panel_width;
//        y = new_block_y * rows_per_block + block_y_mod + base_y_offset * rows_per_buffer;
//    }

    // This code sections computes the byte in the buffer that will be manipulated.
    if (_scan_pattern != LINE && _scan_pattern != WZAGZIG && _scan_pattern != VZAG) {
        // Precomputed row offset values
        base_offset = _row_offset[y] - (x / 8) * 2;
        uint8_t row_sector = 0;
        uint16_t row_sector__offset = _width / 4;
        for (uint8_t yy = 0; yy < _height; yy += 2 * _row_pattern) {
            if ((yy <= y) && (y < yy + _row_pattern))
                total_offset_r = base_offset - row_sector__offset * row_sector;
            if ((yy + _row_pattern <= y) && (y < yy + 2 * _row_pattern))
                total_offset_r = base_offset - row_sector__offset * row_sector;

            row_sector++;
        }
    } else {
        // can only be non-zero when _height/(2 inputs per panel)/_row_pattern > 1
        // i.e.: 32x32 panel with 1/8 scan (A/B/C lines) -> 32/2/8 = 2
        uint8_t vert_index_in_buffer = (y % _rows_per_buffer) / _row_pattern; // which set of rows per buffer
        // can only ever be 0/1 since there are only ever 2 separate input sets present for this variety of panels (R1G1B1/R2G2B2)
        uint8_t which_buffer = y / _rows_per_buffer;
        uint8_t x_byte = x / 8;
        // assumes panels are only ever chained for more width
        uint8_t which_panel = x_byte / _panel_width_bytes;
        uint8_t in_row_byte_offset = x_byte % _panel_width_bytes;
        // this could be pretty easily extended to vertical stacking as well
        total_offset_r = _row_offset[y] - in_row_byte_offset - _panel_width_bytes * (_row_sets_per_buffer *
                                                                                     (_panels_width * which_buffer +
                                                                                      which_panel) +
                                                                                     vert_index_in_buffer);
    }
//    Serial.print(x);
//    Serial.print(" ");
//    Serial.print(y);
//    Serial.print(" ");
//    Serial.print(total_offset_r);
//    Serial.print(" ");

    uint8_t bit_select = x % 8;
//    //Some panels have a byte wise row-changing scanning pattern and/or a bit changing pattern that will be taken care of here.
//    if ((y % (_row_pattern * 2)) < _row_pattern) {
//        // Variant of ZIGZAG pattern with bit oder reversed on lower part (starts on upper part)
//        if (_scan_pattern == ZAGGIZ) {
//            total_offset_r--;
//            bit_select = 7 - bit_select;
//        }
//
//        if (_scan_pattern == ZAGZIG)
//            total_offset_r--;
//
//        // Byte split pattern (lower part)
//        if (_scan_pattern == ZZAGG)
//            if (bit_select > 3) total_offset_r--;
//    } else {
//        if (_scan_pattern == ZIGZAG) total_offset_r--;
//
//        // Byte split pattern (upper part)
//        if (_scan_pattern == ZZAGG) {
//            if (bit_select <= 3) bit_select += 4;
//            else {
//                bit_select -= 4;
//                total_offset_r--;
//            }
//        }
//    }

//    Serial.print(_pattern_color_bytes);
//    Serial.print(" ");

    total_offset_g = total_offset_r - _pattern_color_bytes;
    total_offset_b = total_offset_g - _pattern_color_bytes;
//    Serial.print(total_offset_g);
//    Serial.print(" ");
//    Serial.println(total_offset_b);


    uint8_t (*PxMATRIX_bufferp)[PxMATRIX_COLOR_DEPTH][buffer_size] = &PxMATRIX_buffer;

#ifdef PxMATRIX_double_buffer
    PxMATRIX_bufferp = selected_buffer ? &PxMATRIX_buffer2 : &PxMATRIX_buffer;
#endif

    //Color interlacing
    for (int this_color = 0; this_color < PxMATRIX_COLOR_DEPTH; this_color++) {
        uint8_t color_tresh = this_color * color_step + color_half_step;

        if (r > color_tresh + _color_R_offset)
            (*PxMATRIX_bufferp)[this_color][total_offset_r] |= _BV(bit_select);
        else
            (*PxMATRIX_bufferp)[this_color][total_offset_r] &= ~_BV(bit_select);

        if (g > color_tresh + _color_G_offset)
            (*PxMATRIX_bufferp)[(this_color + color_third_step) % PxMATRIX_COLOR_DEPTH][total_offset_g] |= _BV(
                    bit_select);
        else
            (*PxMATRIX_bufferp)[(this_color + color_third_step) % PxMATRIX_COLOR_DEPTH][total_offset_g] &= ~_BV(
                    bit_select);

        if (b > color_tresh + _color_B_offset)
            (*PxMATRIX_bufferp)[(this_color + color_two_third_step) % PxMATRIX_COLOR_DEPTH][total_offset_b] |= _BV(
                    bit_select);
        else
            (*PxMATRIX_bufferp)[(this_color + color_two_third_step) % PxMATRIX_COLOR_DEPTH][total_offset_b] &= ~_BV(
                    bit_select);
    }
}

inline void PxMATRIX::drawPixelRGB565(int16_t x, int16_t y, uint16_t color) {
    // writing = true;
    uint8_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
    uint8_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
    uint8_t b = (((color & 0x1F) * 527) + 23) >> 6;
#ifdef PxMATRIX_double_buffer
    fillMatrixBuffer(x, y, r, g, b, !_active_buffer);
#else
    fillMatrixBuffer(x, y, r, g, b, false);
#endif
// writing = false;
}

inline void PxMATRIX::drawPixelRGB888(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b) {
    // writing = true;
#ifdef PxMATRIX_double_buffer
    fillMatrixBuffer(x, y, r, g, b, !_active_buffer);
#else
    fillMatrixBuffer(x, y, r, g, b, false);
#endif
// writing = false;
}

// the most basic function, get a single pixel
inline uint8_t PxMATRIX::getPixel(int8_t x, int8_t y) {
    return (0);//PxMATRIX_buffer[x+ (y/8)*LCDWIDTH] >> (y%8)) & 0x1;
}

inline void PxMATRIX::begin() {
    begin(PxMATRIX_ROW_PATTERN);
    debugPrint("STARTED PXMATRIX");
}


void PxMATRIX::spi_init() {
    //spiClass = SPIClass(VSPI);
#ifdef ESP32
    spiClass.begin(_SPI_CLK, -1, _SPI_MOSI, _SPI_SS);
#else
    spiClass.begin();
#endif

#if defined(ESP32) || defined(ESP8266)
    spiClass.setFrequency(PxMATRIX_SPI_FREQUENCY);
#endif

    spiClass.setDataMode(SPI_MODE0);
    spiClass.setBitOrder(MSBFIRST);



    // delayMicroseconds(10);


    // digitalWrite(P_REG_OE, 1);
    // digitalWrite(P_REG_LATCH, 0);

    // SPI_TRANSFER(&abacasd, 1);

    // delayMicroseconds(10);

    // digitalWrite(P_REG_LATCH, 1);
    // delayMicroseconds(10);
    // digitalWrite(P_REG_LATCH, 0);
    // delayMicroseconds(10);
    // digitalWrite(P_REG_OE, 0);
    // delay(100);



}

void PxMATRIX::begin(uint8_t row_pattern) {

    _row_pattern = row_pattern;
    if (_row_pattern == 4)
        _scan_pattern = ZIGZAG;
    _mux_pattern = BINARY;

    _pattern_color_bytes = (_height / _row_pattern) * (_width / 8);
    _row_sets_per_buffer = _rows_per_buffer / _row_pattern;
    _send_buffer_size = _pattern_color_bytes * 3;

    pinMode(_REG_LATCH_PIN, OUTPUT);

    spi_init();

    pinMode(_OE_PIN, OUTPUT);
    pinMode(_LATCH_PIN, OUTPUT);
    pinMode(_REG_LATCH_PIN, OUTPUT);
    digitalWrite(_OE_PIN, HIGH);
    set_mux(0);
    // Precompute row offset values
    for (uint8_t yy = 0; yy < _height; yy++)
        _row_offset[yy] = ((yy) % _row_pattern) * _send_buffer_size + _send_buffer_size - 1;
    _initialized = true;
}

void PxMATRIX::latch(uint16_t show_time, uint8_t value) {
    if (_driver_chip == SHIFT) {
        digitalWrite(_LATCH_PIN, HIGH);
        digitalWrite(_LATCH_PIN, LOW);
        if (show_time > 0) {
            digitalWrite(_OE_PIN, 0);
            unsigned long start_time = micros();
            //while ((micros() - start_time) < show_time)
            //asm volatile (" nop ");
            while ((micros() - start_time) < show_time)
                    asm volatile("nop");
            digitalWrite(_OE_PIN, 1);
        }
    }
}

void PxMATRIX::latch(uint16_t show_time) {

    if (_driver_chip == SHIFT) {
        //digitalWrite(_OE_PIN,0); // <<< remove this
        digitalWrite(_LATCH_PIN, HIGH);
        //delayMicroseconds(10);
        digitalWrite(_LATCH_PIN, LOW);

        //delayMicroseconds(10);
        if (show_time > 0) {
            //delayMicroseconds(show_time);
            digitalWrite(_OE_PIN, 0);


            delayFunction(show_time);
            //delayMicroseconds(show_time);

//            while ((micros() - start_time) < show_time)
//                asm volatile("nop");
//                yield();
            // delay(show_time);
            //vTaskDelay(show_time / portTICK_PERIOD_MS);
            digitalWrite(_OE_PIN, 1);
        }


    }

    if (_driver_chip == FM6124 || _driver_chip == FM6126A) {
        //digitalWrite(_OE_PIN,0); // <<< remove this
        digitalWrite(_LATCH_PIN, LOW);
        digitalWrite(_SPI_CLK, LOW);
        for (uint8_t latch_count = 0; latch_count < 3; latch_count++) {
            digitalWrite(_SPI_CLK, HIGH);
            delayMicroseconds(1);
            digitalWrite(_SPI_CLK, LOW);
            delayMicroseconds(1);
        }
        digitalWrite(_LATCH_PIN, HIGH);
        digitalWrite(_OE_PIN, 0); //<<<< insert this
        delayMicroseconds(show_time);
        digitalWrite(_OE_PIN, 1);
    }
}

void PxMATRIX::display() {
    display(_draw_time);
}

void PxMATRIX::set_mux(uint8_t value) {
    value |= _other_pins_state;
    digitalWrite(_REG_LATCH_PIN, 0);
    SPI_TRANSFER(&value, 1);
    digitalWrite(_REG_LATCH_PIN, 1);
    digitalWrite(_REG_LATCH_PIN, 0);
    delayMicroseconds(_mux_delay);
}


unsigned long lastTime = 0;


void PxMATRIX::display(uint16_t show_time) {
    if (show_time < 10)
        show_time = 10;
    unsigned long start_time = 0;

    uint8_t (*bufferp)[PxMATRIX_COLOR_DEPTH][buffer_size] = &PxMATRIX_buffer;
    unsigned long nanoStart = micros();

    for (uint8_t i = 0; i < _row_pattern; i++) {
        // yield();
        if (_driver_chip == SHIFT) {
            if (_fast_update && (_brightness == 255)) {
                spiClass.setFrequency(PxMATRIX_SPI_FREQUENCY);
                // This will clock data into the display while the outputs are still
                // latched (LEDs on). We therefore utilize spiClass transfer latency as LED
                // ON time and can reduce the waiting time (show_time). This is rather
                // timing sensitive and may lead to flicker however promises reduced
                // update times and increased brightness



                set_mux((i + _row_pattern - 1) % _row_pattern);
                digitalWrite(_LATCH_PIN, HIGH);
                digitalWrite(_OE_PIN, 0);
                start_time = micros();

                digitalWrite(_LATCH_PIN, LOW);
                delayMicroseconds(1);

                SPI_TRANSFER(&(*bufferp)[_display_color][i * _send_buffer_size], _send_buffer_size);


                delayFunction(show_time);
                //delayMicroseconds(show_time);

//                while ((micros() - start_time) < show_time)
//                        asm volatile("nop");
                digitalWrite(_OE_PIN, 1);
            } else {
                SPI_TRANSFER(&(*bufferp)[_display_color][i * _send_buffer_size], _send_buffer_size);
                set_mux(i);
                latch(show_time * ((uint16_t) _brightness) / 255);
            }
        }
    }

    lastTime = micros() - nanoStart;
    _display_color++;
    if (_display_color >= PxMATRIX_COLOR_DEPTH) {
        _display_color = 0;
    }
}

void PxMATRIX::flushDisplay() {
    for (int ii = 0; ii < _send_buffer_size; ii++)
        SPI_BYTE(0x00);
}


void PxMATRIX::clear() {
    memset(PxMATRIX_buffer, 0, PxMATRIX_COLOR_DEPTH * buffer_size);
}

void PxMATRIX::setPinState(free_pin pin, uint8_t state) {

    //_other_pins_state |= ((0x01<<pin) & 0xF0);
    if (state == 1) {
        switch (pin) {
            case BIT_5:
                _other_pins_state |= B00010000;
                break;
            case BIT_6:
                _other_pins_state |= B00100000;
                break;
            case BIT_7:
                _other_pins_state |= B01000000;
                break;
            case BIT_8:
                _other_pins_state |= B10000000;
                break;
        }
    } else {
        switch (pin) {
            case BIT_5:
                _other_pins_state &= ~(B00010000);
                break;
            case BIT_6:
                _other_pins_state &= ~(B00100000);
                break;
            case BIT_7:
                _other_pins_state &= ~(B01000000);
                break;
            case BIT_8:
                _other_pins_state &= ~(B10000000);;
                break;
        }
    }

}

PxMATRIX PxMatrixControlInstance = PxMATRIX(PxMATRIX_WIDTH, PxMATRIX_HEIGHT, PIN_OUTPUT_PX_MOSI, PIN_OUTPUT_PX_CLK,
                                            PIN_OUTPUT_PX_STROBO, PIN_OUTPUT_PX_OE, PIN_OUTPUT_PX_REGLATCH);

void writeToRegisterPin(uint8_t pin, uint8_t state) {
    //TODO
    PxMatrixControlInstance.setPinState(static_cast<free_pin>(pin), state);
}


//hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR display_updater() {
    // Increment the counter and set the time of ISR
    portENTER_CRITICAL_ISR(&timerMux);
    PxMatrixControlInstance.display(PxMatrixControlInstance.getDrawTime());
    //   display.displayTestPattern(70);
    portEXIT_CRITICAL_ISR(&timerMux);
}

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

TaskHandle_t handle;

void PxMATRIX::startDisplayThread() {
    if (!_initialized)
        begin();
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, PxMATRIX_INTERRUPT_TIMER, true);
    timerAlarmEnable(timer);



//    xTaskCreatePinnedToCore(
//            threadDisplay, /* Function to implement the task */
//            "Screen Task", /* Name of the task */
//            10000,  /* Stack size in words */
//            NULL,  /* Task input parameter */
//            0,  /* Priority of the task */
//            &handle,  /* Task handle. */
//            0); /* Core where the task should run */


}


void PxMATRIX::setDrawTime(uint16_t drawtime) {
    _draw_time = drawtime;
}

uint16_t PxMATRIX::getDrawTime() const {
    return _draw_time;
}

void PxMATRIX::setFrequency(int frequency) {
    spiClass.setFrequency(frequency);

}

void PxMATRIX::test() {

    if (!_initialized) {
        begin();
        //startDisplayThread();
    }
#ifdef PxMATRIX_FULLBRIGHTNESS
    setBrightness(255);
    setFastUpdate(true);
    fullBrightness();
    return;
#endif
    clear();
    setCursor(0, 0);
    setTextColor(PxMatrixControlInstance.white);
    print("ABCDEFGHIJKLMNOPRSTUWXYZ");

    for (int i = 2; i < 8; i++) {

        writeFillRect(i * 10, 8, 10, 7, PxMatrixControlInstance.colors[i % 8]);
    }
    for (int i = 2; i < 8; i++) {

        writeFillRect(i * 10 + 60, 8, 10, 7, PxMatrixControlInstance.colors[i % 8]);
    }

    writeFillRect(0, 16, 150, 16, PxMatrixControlInstance.white);
    Serial.println("PXMatrix initialized! Check if TEST PATTERN is displayed without errors");
}

void PxMATRIX::fullBrightness() {
    if (!_initialized) {
        begin();
    }
    fillRect(0, 0, _width, _height, white);
}

void PxMATRIX::writeFrameFromBuffer(const uint8_t *buffer, const int &bufferLength) {
    memcpy(PxMATRIX_buffer[0], buffer, bufferLength);
}

uint8_t *PxMATRIX::getBufferAddress() {
    return PxMATRIX_buffer[0];
}

void PxMATRIX::drawPixels(const vector <Pixel> &vector) {
    for (auto p : vector) {
        this->drawPixel(p.x, p.y, color565(p.r, p.b, p.g)); //TODO : IT SHOULD NOT BE LIKE THIS
    }
}

#endif

#endif