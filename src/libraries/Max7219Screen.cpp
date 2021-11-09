//
// Created by dzing on 08/06/2021.
//

#include "libraries/Max7219Screen.h"
#ifdef MAX_MATRIX_SCREEN
#if MAX_CONNECTION_TYPE == 3
uint8_t SIMPLE_VISOR_OFFSETS[32][24] = {
        {7,  6,  5,  4,  3,  2,  1,  0,  23, 22, 21, 20, 19, 18, 17, 16, 71, 70, 69, 68, 67, 66, 65, 64},
        {7,  6,  5,  4,  3,  2,  1,  0,  23, 22, 21, 20, 19, 18, 17, 16, 71, 70, 69, 68, 67, 66, 65, 64},
        {7,  6,  5,  4,  3,  2,  1,  0,  23, 22, 21, 20, 19, 18, 17, 16, 71, 70, 69, 68, 67, 66, 65, 64},
        {7,  6,  5,  4,  3,  2,  1,  0,  23, 22, 21, 20, 19, 18, 17, 16, 71, 70, 69, 68, 67, 66, 65, 64},
        {7,  6,  5,  4,  3,  2,  1,  0,  23, 22, 21, 20, 19, 18, 17, 16, 71, 70, 69, 68, 67, 66, 65, 64},
        {7,  6,  5,  4,  3,  2,  1,  0,  23, 22, 21, 20, 19, 18, 17, 16, 71, 70, 69, 68, 67, 66, 65, 64},
        {7,  6,  5,  4,  3,  2,  1,  0,  23, 22, 21, 20, 19, 18, 17, 16, 71, 70, 69, 68, 67, 66, 65, 64},
        {7,  6,  5,  4,  3,  2,  1,  0,  23, 22, 21, 20, 19, 18, 17, 16, 71, 70, 69, 68, 67, 66, 65, 64},

        {15, 14, 13, 12, 11, 10, 9,  8,  31, 30, 29, 28, 27, 26, 25, 24, 79, 78, 77, 76, 75, 74, 73, 72},
        {15, 14, 13, 12, 11, 10, 9,  8,  31, 30, 29, 28, 27, 26, 25, 24, 79, 78, 77, 76, 75, 74, 73, 72},
        {15, 14, 13, 12, 11, 10, 9,  8,  31, 30, 29, 28, 27, 26, 25, 24, 79, 78, 77, 76, 75, 74, 73, 72},
        {15, 14, 13, 12, 11, 10, 9,  8,  31, 30, 29, 28, 27, 26, 25, 24, 79, 78, 77, 76, 75, 74, 73, 72},
        {15, 14, 13, 12, 11, 10, 9,  8,  31, 30, 29, 28, 27, 26, 25, 24, 79, 78, 77, 76, 75, 74, 73, 72},
        {15, 14, 13, 12, 11, 10, 9,  8,  31, 30, 29, 28, 27, 26, 25, 24, 79, 78, 77, 76, 75, 74, 73, 72},
        {15, 14, 13, 12, 11, 10, 9,  8,  31, 30, 29, 28, 27, 26, 25, 24, 79, 78, 77, 76, 75, 74, 73, 72},
        {15, 14, 13, 12, 11, 10, 9,  8,  31, 30, 29, 28, 27, 26, 25, 24, 79, 78, 77, 76, 75, 74, 73, 72},

        {55, 54, 53, 52, 51, 50, 49, 48, 39, 38, 37, 36, 35, 34, 33, 32, 87, 86, 85, 84, 83, 82, 81, 80},
        {55, 54, 53, 52, 51, 50, 49, 48, 39, 38, 37, 36, 35, 34, 33, 32, 87, 86, 85, 84, 83, 82, 81, 80},
        {55, 54, 53, 52, 51, 50, 49, 48, 39, 38, 37, 36, 35, 34, 33, 32, 87, 86, 85, 84, 83, 82, 81, 80},
        {55, 54, 53, 52, 51, 50, 49, 48, 39, 38, 37, 36, 35, 34, 33, 32, 87, 86, 85, 84, 83, 82, 81, 80},
        {55, 54, 53, 52, 51, 50, 49, 48, 39, 38, 37, 36, 35, 34, 33, 32, 87, 86, 85, 84, 83, 82, 81, 80},
        {55, 54, 53, 52, 51, 50, 49, 48, 39, 38, 37, 36, 35, 34, 33, 32, 87, 86, 85, 84, 83, 82, 81, 80},
        {55, 54, 53, 52, 51, 50, 49, 48, 39, 38, 37, 36, 35, 34, 33, 32, 87, 86, 85, 84, 83, 82, 81, 80},
        {55, 54, 53, 52, 51, 50, 49, 48, 39, 38, 37, 36, 35, 34, 33, 32, 87, 86, 85, 84, 83, 82, 81, 80},

        {63, 62, 61, 60, 59, 58, 57, 56, 47, 46, 45, 44, 43, 42, 41, 40, 95, 94, 93, 92, 91, 90, 89, 88},
        {63, 62, 61, 60, 59, 58, 57, 56, 47, 46, 45, 44, 43, 42, 41, 40, 95, 94, 93, 92, 91, 90, 89, 88},
        {63, 62, 61, 60, 59, 58, 57, 56, 47, 46, 45, 44, 43, 42, 41, 40, 95, 94, 93, 92, 91, 90, 89, 88},
        {63, 62, 61, 60, 59, 58, 57, 56, 47, 46, 45, 44, 43, 42, 41, 40, 95, 94, 93, 92, 91, 90, 89, 88},
        {63, 62, 61, 60, 59, 58, 57, 56, 47, 46, 45, 44, 43, 42, 41, 40, 95, 94, 93, 92, 91, 90, 89, 88},
        {63, 62, 61, 60, 59, 58, 57, 56, 47, 46, 45, 44, 43, 42, 41, 40, 95, 94, 93, 92, 91, 90, 89, 88},
        {63, 62, 61, 60, 59, 58, 57, 56, 47, 46, 45, 44, 43, 42, 41, 40, 95, 94, 93, 92, 91, 90, 89, 88},
        {63, 62, 61, 60, 59, 58, 57, 56, 47, 46, 45, 44, 43, 42, 41, 40, 95, 94, 93, 92, 91, 90, 89, 88}
};
#endif

uint8_t *GLOBAL_MAX_BUFFOR;
Max7219Screen::Max7219Screen() : Adafruit_GFX(MAX_WIDTH * 8, MAX_HEIGHT * 8) {
    init(MAX_MATRICES_NUMBER, MAX_WIDTH, MAX_HEIGHT, PIN_OUTPUT_MAX_MOSI, PIN_OUTPUT_MAX_CLK,
         PIN_OUTPUT_MAX_CS, (ConnectionType) MAX_CONNECTION_TYPE); //TODO:
}

void Max7219Screen::begin() {
    GLOBAL_MAX_BUFFOR = new uint8_t[MAX_MATRICES_NUMBER * 8 * MAX_FRAMES_BUFFER];
    init(MAX_MATRICES_NUMBER, MAX_WIDTH, MAX_HEIGHT, PIN_OUTPUT_MAX_MOSI, PIN_OUTPUT_MAX_CLK,
         PIN_OUTPUT_MAX_CS, (ConnectionType) MAX_CONNECTION_TYPE);
    pinMode(spiClk, OUTPUT);
    pinMode(spiSS, OUTPUT);
    pinMode(spiMosi, OUTPUT);
    maxSpiClass.begin(spiClk, 1, spiMosi, spiSS);
    maxSpiClass.setFrequency(MAX_SPI_FREQ);
    maxSpiClass.setBitOrder(MSBFIRST);
    maxSpiClass.setDataMode(SPI_MODE0);
    sendStartupCommands();
    initialized = true;
    //TODO


    clear();

}


void Max7219Screen::sendStartupCommands() {
    lastStartupCommandsSend = millis();
    sendCommand(MAX7219_TEST, 0x00);
    sendCommand(MAX7219_DECODE_MODE, 0x00); // Disable BCD mode.
    sendCommand(MAX7219_BRIGHTNESS, this->brightness);  // Use lowest intensity.
    sendCommand(MAX7219_SCAN_LIMIT, 0x0f);  // Scan all digits.
    sendCommand(MAX7219_SHUTDOWN, 0x01);
}


void Max7219Screen::sendCommand(const uint8_t &address, const uint8_t &value) {
    digitalWrite(spiSS, 0);
    for (int i = 0; i < matricesNumber; i++) {
        maxSpiClass.transfer(address);
        maxSpiClass.transfer(value);
    }
    digitalWrite(spiSS, 1);
}

void Max7219Screen::drawPixel(int16_t x, int16_t y, uint16_t color) {

#if MAX_CONNECTION_TYPE == 3
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (color > 0) {
            maxBuffer[offsets[x][y]] |= 1 << (x % 8);
        } else {
            maxBuffer[offsets[x][y]] &= ~(1 << (x % 8));
        }
    }
#else //TODO: THINK OF IT... Maybe just change rotation mode? of something....
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (color > 0) {
            maxBuffer[offsets[x][y]] |= 1 << (y % 8);
        } else {
            maxBuffer[offsets[x][y]] &= ~(1 << (y % 8));
        }
    }
#endif
}


void Max7219Screen::display() {
#ifdef MAX_STARTUP_REFRESH
    if ((millis() - lastStartupCommandsSend) > MAX_STARTUP_REFRESH) {
        lastStartupCommandsSend = millis();
        sendStartupCommands();
        delay(1);
    }
#endif
    pinMode(spiClk, OUTPUT);
    pinMode(spiSS, OUTPUT);
    pinMode(spiMosi, OUTPUT);

    for (int i = 1; i < 9; i++) {
        digitalWrite(spiSS, LOW);
        for (int dev = matricesNumber - 1; dev >= 0; dev--) {
            maxSpiClass.transfer(i);
            maxSpiClass.transfer(maxBuffer[i - 1 + dev * 8]);
        }
        digitalWrite(spiSS, HIGH);
    }
}

void Max7219Screen::clear() {
    for (int i = 0; i < matricesNumber * 8; i++) {
        maxBuffer[i] = 0;
    }
    display();
}

void Max7219Screen::test() {
    if (!initialized)
        begin();
    clear();

#ifdef MAX_FULLPOWER_TEST
    pinMode(spiMosi, OUTPUT);
    pinMode(spiClk, OUTPUT);
    pinMode(spiSS, OUTPUT);
    sendCommand(MAX7219_TEST, 0x01);
    return;
#endif
    sendStartupCommands();
    setTextColor(MAX_ON_PIXEL);
    setCursor(0, 0);
    char c = 'A';
    for (int n = 0; n < matricesNumber; n++) {
        print(c);
        c++;
    }
    display();
    Serial.println("SEND!");
}

int Max7219Screen::getFPS() const {
    return fps;
}

void Max7219Screen::writeFrameFromBuffer(const uint8_t *frameStartAddress, const int &frameLength) {
    memcpy(maxBuffer, frameStartAddress, frameLength);
    display();
}

void Max7219Screen::drawPixels(const std::vector<Pixel> &vector) {
    for (auto p : vector) {
        this->drawPixel(p.x, p.y, p.r + p.g + p.b); //TODO : IT SHOULD NOT BE LIKE THIS
    }
}

void Max7219Screen::setBrightness(const uint8_t &target) {

    if (target >= 0 && target <= 8) {
        this->brightness = target;
        sendCommand(MAX7219_BRIGHTNESS, target);
    }
}

#endif