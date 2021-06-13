//
// Created by dzing on 01/02/2021.
//

#ifndef VISORV3_WSCONTROL_H
#define VISORV3_WSCONTROL_H

#include "ConstantsAndSettings.h"

#ifdef WS_MATRIX_SCREEN

#include "Arduino.h"
#define FASTLED_ALLOW_INTERRUPTS 1
#define FASTLED_INTERRUPT_RETRY_COUNT 2
#include "FastLED.h"
#include "Adafruit_GFX.h"


class WSControl : public Adafruit_GFX, Screen {

public:
    WSControl() : Adafruit_GFX(WS_WIDTH, WS_HEIGHT) {
    }

    void begin() override {
        FastLED.addLeds<WS2812, PIN_OUTPUT_WS, GRB>(ledsArray, leds);
        FastLED.setBrightness(brightness);
        FastLED.setDither(DISABLE_DITHER);
        FastLED.setMaxRefreshRate(50, false);
        initialized = true;
    }

    void drawPixels(const vector<Pixel> &vector) override {
        for (auto p : vector) {
            ledsArray[p.y * _width + p.x] = CRGB(p.r, p.g, p.b);
        }
    }

    void clear() override {
        FastLED.clear(true);
    }

    void writeFrameFromBuffer(const uint8_t *buffer, const int &bufferLength) override {
        Serial.println((unsigned long) buffer);
        Serial.println(bufferLength);
        FastLED.clearData();
        for (int i = 0; i < bufferLength; i += 3) {
            ledsArray[i] = CRGB(*(buffer + i), *(buffer + i + 1), *(buffer + i + 2));
        }
        display();

        delay(1000);
    }

    void drawPixel(int16_t x, int16_t y, uint16_t color) override {
        ledsArray[y * _width + x] = color;
    }

    void display() {
        FastLED.show();
    }

    void setBrightness(const uint8_t &brightness) override {
        this->brightness = brightness;
        FastLED.setBrightness(brightness);
    }

    void test() {

        if (!initialized)
            begin();
#ifdef WS_FULLPOWER_TEST
        setBrightness(255);
        for(int i = 0; i < leds; i++)
        {
            ledsArray[i] = CRGB::White;
        }
        display();
        return;
#endif
        int increment = 255 / leds;
        for (int i = 0; i < leds; i++) {
            ledsArray[i] = CHSV(increment * i, 255, 255);
        }
        display();
        Serial.println("WS Graphics sent! Check if there is rainbow pattern on the WS");
    }

    bool set = false;

private:
    CRGB ledsArray[WS_NUMBEROFLEDS];
//    CRGBSet ledSet;
    int leds = WS_NUMBEROFLEDS;
    int brightness = WS_DEFAULT_BRIGHTNESS;
    bool initialized = false;


};

#endif
#endif //VISORV3_WSCONTROL_H
