//
// Created by dzing on 08/06/2021.
//

#include "libraries/PxMatrix.h"

#ifdef PX_MATRIX_SCREEN

void PxMatrixScreen::setDrawTime(uint16_t drawtime) {
    _draw_time = drawtime;
}

uint16_t PxMatrixScreen::getDrawTime() const {
    return _draw_time;
}

void PxMatrixScreen::setFrequency(const int &frequency) {
    spiClass.setFrequency(frequency);

}

void PxMatrixScreen::test() {

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
    setTextColor(white);
    print("ABCDEFGHIJKLMNOPRSTUWXYZ");

    for (int i = 2; i < 8; i++) {

        writeFillRect(i * 10, 8, 10, 7, colors[i % 8]);
    }
    for (int i = 2; i < 8; i++) {

        writeFillRect(i * 10 + 60, 8, 10, 7, colors[i % 8]);
    }

    writeFillRect(0, 16, 150, 16, white);
    Serial.println("PXMatrix initialized! Check if TEST PATTERN is displayed without errors");
}

void PxMatrixScreen::fullBrightness() {
    if (!_initialized) {
        begin();
    }
    fillRect(0, 0, _width, _height, white);
}

void PxMatrixScreen::writeFrameFromBuffer(const uint8_t *buffer, const int &bufferLength) {
    memcpy(PxMATRIX_buffer[0], buffer, bufferLength);
}

uint8_t *PxMatrixScreen::getBufferAddress() {
    return PxMATRIX_buffer[0];
}

void PxMatrixScreen::drawPixels(const vector<Pixel> &vector) {
    for (auto p : vector) {
        this->drawPixel(p.x, p.y, color565(p.r, p.b, p.g)); //TODO : IT SHOULD NOT BE LIKE THIS
    }
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
inline uint16_t PxMatrixScreen::color565(const uint8_t &r, const uint8_t &g, const uint8_t &b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Init code common to both constructors
inline void PxMatrixScreen::init(uint16_t width, uint16_t height, uint8_t MOSI, uint8_t CLK, uint8_t LATCH, uint8_t OE,
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

inline void PxMatrixScreen::fm612xWriteRegister(const uint16_t &regValue, const uint8_t &regPosition) {
    spi_t *spibus = spiClass.bus();
    // reg_value = 0x1234;  debug

    for (int i = 0; i < 47; i++)
        SPI_2BYTE(regValue);

    spiSimpleTransaction(spibus);

    spibus->dev->mosi_dlen.usr_mosi_dbitlen = 16 - regPosition - 1;
    spibus->dev->miso_dlen.usr_miso_dbitlen = 0;
    spibus->dev->data_buf[0] = regValue >> 8;
    spibus->dev->cmd.usr = 1;
    while (spibus->dev->cmd.usr);

    GPIO_REG_SET(1 << _LATCH_PIN);

    spibus->dev->mosi_dlen.usr_mosi_dbitlen = (regPosition - 8) - 1;
    spibus->dev->data_buf[0] = regValue >> (regPosition - 8);
    spibus->dev->cmd.usr = 1;
    while (spibus->dev->cmd.usr);
    spiEndTransaction(spibus);

    SPI_BYTE(regValue & 0xff);

    GPIO_REG_CLEAR(1 << _LATCH_PIN);

}

#else
inline void PxMatrixScreen::writeRegister(uint16_t reg_value, uint8_t reg_position)
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

inline void PxMatrixScreen::setDriverChip(driver_chips driver_chip) {
    _driver_chip = driver_chip;

    if (driverChip == FM6124 || driverChip == FM6126A) {

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

inline void PxMatrixScreen::setMuxPattern(mux_patterns mux_pattern) {
    _mux_pattern = mux_pattern;

    // We handle the multiplexing in the library and activate one of for
    // row drivers --> need A,B,C,D pins
//  if (_mux_pattern==STRAIGHT)
//  {
//    //pinMode(_C_PIN, OUTPUT);
//    //pinMode(_D_PIN, OUTPUT);
//  }
}


inline void PxMatrixScreen::setMuxDelay(const uint8_t &muxDelay) {
    _mux_delay = muxDelay;
}

inline void PxMatrixScreen::setScanPattern(scan_patterns scan_pattern) {
    _scan_pattern = scan_pattern;
}

inline void PxMatrixScreen::setPanelsWidth(uint8_t panels) {
    _panels_width = panels;
    _panel_width_bytes = (_width / _panels_width) / 8;
}

inline void PxMatrixScreen::setRotate(bool rotate) {
    _rotate = rotate;
}

inline void PxMatrixScreen::setFlip(bool flip) {
    _flip = flip;
}

inline void PxMatrixScreen::setFastUpdate(bool fast_update) {
    _fast_update = fast_update;
}

void PxMatrixScreen::setBrightness(const uint8_t &brightness) {
    _brightness = brightness;
}


PxMatrixScreen::PxMatrixScreen(uint16_t width, uint16_t height, uint8_t MOSI, uint8_t CLK, uint8_t LATCH, uint8_t OE,
                               uint8_t ABCDE_LATCH)
        : Adafruit_GFX(width + ADAFRUIT_GFX_EXTRA, height) {
    init(width, height, MOSI, CLK, LATCH, OE, ABCDE_LATCH);
}

inline void PxMatrixScreen::drawPixel(int16_t x, int16_t y, uint16_t color) {
    drawPixelRGB565(x, y, color);
}

inline void PxMatrixScreen::showBuffer() {
    _active_buffer = !_active_buffer;
}

inline void PxMatrixScreen::setColorOffset(uint8_t r, uint8_t g, uint8_t b) {
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

inline void
PxMatrixScreen::fillMatrixBuffer(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b, bool selected_buffer) {
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

inline void PxMatrixScreen::drawPixelRGB565(int16_t x, int16_t y, uint16_t color) {
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

inline void PxMatrixScreen::drawPixelRGB888(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b) {
    // writing = true;
#ifdef PxMATRIX_double_buffer
    fillMatrixBuffer(x, y, r, g, b, !_active_buffer);
#else
    fillMatrixBuffer(x, y, r, g, b, false);
#endif
// writing = false;
}

// the most basic function, get a single pixel
inline uint8_t PxMatrixScreen::getPixel(int8_t x, int8_t y) {
    return (0);//PxMATRIX_buffer[x+ (y/8)*LCDWIDTH] >> (y%8)) & 0x1;
}

void PxMatrixScreen::begin() {
    begin(PxMATRIX_ROW_PATTERN);
    debugPrint("STARTED PXMATRIX");
}


void PxMatrixScreen::spiInit() {
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

void PxMatrixScreen::begin(const uint8_t &row_pattern) {

    _row_pattern = row_pattern;
    if (_row_pattern == 4)
        _scan_pattern = ZIGZAG;
    _mux_pattern = BINARY;

    _pattern_color_bytes = (_height / _row_pattern) * (_width / 8);
    _row_sets_per_buffer = _rows_per_buffer / _row_pattern;
    _send_buffer_size = _pattern_color_bytes * 3;

    pinMode(_REG_LATCH_PIN, OUTPUT);

    spiInit();

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

void PxMatrixScreen::latch(uint16_t show_time, uint8_t value) {
    if (_driver_chip == SHIFT) {
        digitalWrite(_LATCH_PIN, HIGH);
        digitalWrite(_LATCH_PIN, LOW);
        if (show_time > 0) {
            digitalWrite(_OE_PIN, 0);
            unsigned long start_time = micros();
            //while ((micros() - start_time) < show_time)
            //asm volatile (" nop ");
            while ((micros() - start_time) < showTime)
                    asm volatile("nop");
            digitalWrite(_OE_PIN, 1);
        }
    }
}

void PxMatrixScreen::latch(const uint16_t &show_time) {

    if (_driver_chip == SHIFT) {
        //digitalWrite(_OE_PIN,0); // <<< remove this
        digitalWrite(_LATCH_PIN, HIGH);
        //delayMicroseconds(10);
        digitalWrite(_LATCH_PIN, LOW);

        //delayMicroseconds(10);
        if (show_time > 0) {
            //delayMicroseconds(show_time);
            digitalWrite(_OE_PIN, 0);


            delayMicroseconds(show_time);
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

void PxMatrixScreen::set_mux(uint8_t value) {
    value |= _other_pins_state;
    digitalWrite(_REG_LATCH_PIN, 0);
    SPI_TRANSFER(&value, 1);
    digitalWrite(_REG_LATCH_PIN, 1);
    digitalWrite(_REG_LATCH_PIN, 0);
    delayMicroseconds(_mux_delay);
}


void PxMatrixScreen::display(uint16_t show_time) {
    if (show_time < 10)
        show_time = 10;

    uint8_t (*bufferp)[PxMATRIX_COLOR_DEPTH][buffer_size] = &PxMATRIX_buffer;

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


                //delayFunction(show_time);
                delayMicroseconds(show_time);

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

    _display_color++;
    if (_display_color >= PxMATRIX_COLOR_DEPTH) {
        _display_color = 0;
    }
}

void PxMatrixScreen::flushDisplay() {
    for (int ii = 0; ii < _send_buffer_size; ii++)
        SPI_BYTE(0x00);
}


void PxMatrixScreen::clear() {
    memset(PxMATRIX_buffer, 0, PxMATRIX_COLOR_DEPTH * buffer_size);
}

void PxMatrixScreen::setPinState(free_pin pin, uint8_t state) {

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

#endif
