//
// Created by dzing on 08/06/2021.
//

#include "libraries/Max7219Screen.h"

Max7219Screen::Max7219Screen() : Adafruit_GFX(MAX_WIDTH * 8, MAX_HEIGHT * 8) {
    init(MAX_MATRICES_NUMBER, MAX_WIDTH, MAX_HEIGHT, PIN_OUTPUT_MAX_MOSI, PIN_OUTPUT_MAX_CLK,
         PIN_OUTPUT_MAX_CS, (ConnectionType) MAX_CONNECTION_TYPE); //TODO:
}

void Max7219Screen::begin() {
    pinMode(_spiClk, OUTPUT);
    pinMode(_spiSS, OUTPUT);
    pinMode(_spiMosi, OUTPUT);
    maxSpiClass.begin(_spiClk, -1, _spiMosi, _spiSS);
    maxSpiClass.setFrequency(MAX_SPI_FREQ);
    maxSpiClass.setBitOrder(MSBFIRST);
    maxSpiClass.setDataMode(SPI_MODE0);
    sendStartupCommands();
    _initialized = true;
    clear();

}


void Max7219Screen::sendStartupCommands() {
    lastStartupCommandsSend = millis();
    sendCommand(MAX7219_TEST, 0x00);
    sendCommand(MAX7219_DECODE_MODE, 0x00); // Disable BCD mode.
    sendCommand(MAX7219_BRIGHTNESS, MAX_DEFAULT_BRIGHTNESS);  // Use lowest intensity.
    sendCommand(MAX7219_SCAN_LIMIT, 0x0f);  // Scan all digits.
    sendCommand(MAX7219_SHUTDOWN, 0x01);
}


void Max7219Screen::sendCommand(const uint8_t &address, const uint8_t &value) {
    digitalWrite(_spiSS, 0);
    for (int i = 0; i < _matricesNumber; i++) {
        maxSpiClass.transfer(address);
        maxSpiClass.transfer(value);
    }
    digitalWrite(_spiSS, 1);
}

void Max7219Screen::drawPixel(int16_t x, int16_t y, uint16_t color) {

#if MAX_CONNECTION_TYPE == 3
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (color > 0) {
            _buffer[offsets[x][y]] |= 1 << (x % 8);
        } else {
            _buffer[offsets[x][y]] &= ~(1 << (x % 8));
        }
    }
#else //TODO: THINK OF IT... Maybe just change rotation mode? of something....
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (color > 0) {
            _buffer[offsets[x][y]] |= 1 << (y % 8);
        } else {
            _buffer[offsets[x][y]] &= ~(1 << (y % 8));
        }
    }
#endif
}

void Max7219Screen::display() {
    pinMode(_spiClk, OUTPUT);
    pinMode(_spiSS, OUTPUT);
    pinMode(_spiMosi, OUTPUT);
    for (int i = 1; i < 9; i++) {
        digitalWrite(_spiSS, LOW);
        for (int dev = _matricesNumber - 1; dev >= 0; dev--) {
            maxSpiClass.transfer(i);
            maxSpiClass.transfer(_buffer[i - 1 + dev * 8]);
        }
        digitalWrite(_spiSS, HIGH);
    }
}

void Max7219Screen::drawTestPattern() {
    if (_initialized) {
        begin();
    }
    clear();
    for (int i = 0; i < 96; i++) {
        _buffer[i] = millis() % 255;
    }
    display();
}

void Max7219Screen::clear() {
    for (int i = 0; i < _matricesNumber * 8; i++) {
        _buffer[i] = 0;
    }
    display();
}

void Max7219Screen::test() {
    if (!_initialized)
        begin();
    clear();
    pinMode(_spiClk, OUTPUT);
    pinMode(_spiSS, OUTPUT);
    pinMode(_spiMosi, OUTPUT);
    int a = 0;
    for (int i = 1; i < 4; i += 2) {
        digitalWrite(_spiSS, LOW);
        for (int dev = _matricesNumber - 1; dev >= 0; dev--) {
            maxSpiClass.transfer(i);
            maxSpiClass.transfer(255);
        }
        digitalWrite(_spiSS, HIGH);
    }
//return;
#ifdef MAX_FULLPOWER_TEST
    sendCommand(MAX7219_TEST, 0x01);
    return;
#endif
    clear();
    setTextColor(MAX_ON_PIXEL);
    setCursor(0, 0);
    char c = 'A';
    for (int n = 0; n < _matricesNumber * 2; n++) {
        print(c);
        c++;
    }
    display();
    // clear();
    //drawLine(0, 0, 0, 16, 1);
    //display();
    return;
    fillRect(1, 9, 6, 6, 1);
    drawTriangle(8, 8, 15, 8, 15, 15, 1);
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

bool Max7219Screen::isDisplayTaskRunning() const {
    return _displayTask;
}

int Max7219Screen::getFPS() const {
    return _fps;
}

void Max7219Screen::writeFrameFromBuffer(const uint8_t *frameStartAddress, const int &frameLength) {
    memcpy(_buffer, frameStartAddress, frameLength);
    display();
}

void Max7219Screen::drawPixels(const std::vector<Pixel> &vector) {
    for (auto p : vector) {
        this->drawPixel(p.x, p.y, p.r + p.g + p.b); //TODO : IT SHOULD NOT BE LIKE THIS
    }
}

void Max7219Screen::setBrightness(const uint8_t &target) {
    if (target >= 0 && target <= 8)
        sendCommand(MAX7219_BRIGHTNESS, target);  // Use lowest intensity
}
