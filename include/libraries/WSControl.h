//
// Created by dzing on 01/02/2021.
//

#ifndef VISORV3_WSCONTROL_H
#define VISORV3_WSCONTROL_H

#include "ConstantsAndSettings.h"
#ifdef WS_SUPPORT

#include "Arduino.h"
#define FASTLED_ALLOW_INTERRUPTS 1
#define FASTLED_INTERRUPT_RETRY_COUNT 2
#include "FastLED.h"
#include "Adafruit_GFX.h"


class WSControl: public Adafruit_GFX{

public:
    WSControl() : Adafruit_GFX(WS_WIDTH, WS_HEIGHT), ledSet(_ledsArray, WS_NUMBEROFLEDS) {
    }

    void begin() {
        FastLED.addLeds<WS2811, PIN_OUTPUT_WS, GRB>(_ledsArray, _ledsNumber);
        FastLED.setBrightness(_brightness);
        FastLED.setDither(DISABLE_DITHER);
        FastLED.setMaxRefreshRate(50, false);
        _initialized = true;
    }
    void drawPixel(int16_t x, int16_t y, uint16_t color) override{
        _ledsArray[y * _width + x] = color;
    }

    void display()
    {
        FastLED.show();
    }
    void setBrightness(const uint8_t& brightness)
    {
        this->_brightness = brightness;
        FastLED.setBrightness(brightness);
    }
    void test()
    {

        if(!_initialized)
            begin();
        #ifdef WS_FULLPOWER_TEST
        setBrightness(255);
        for(int i = 0; i < _ledsNumber; i++)
        {
            _leds[i] = CRGB::White;
        }
        display();
        return;
        #endif
        int increment = 255/_ledsNumber;
        for(int i = 0; i < _ledsNumber; i++)
        {
            _ledsArray[i] = CHSV(increment * i, 255, 255);
        }
        display();
        Serial.println("WS Graphics sent! Check if there is rainbow pattern on the WS");
    }
    bool set = false;
    void writeFrameFromBuffer(uint8_t *startAddress, int frameLength) {
        Serial.println((unsigned long)startAddress);
        Serial.println(frameLength);
        if(!set)
        {
            set = true;
        int frame = 0;
        FastLED.clearData();

        for(int i = 0; i < frameLength; i+=3)
        {
            _ledsArray[frame++] = CRGB(*(startAddress + i), *(startAddress + i + 1), *(startAddress + i + 2));
            //Serial.print(_leds[frame-1]);
            //Serial.print(" ");
        }

            }
        display();

        delay(1000);

    }

private:
    CRGB _ledsArray[WS_NUMBEROFLEDS];
    CRGBSet ledSet;
    int _ledsNumber = WS_NUMBEROFLEDS;
    int _pin = PIN_OUTPUT_WS;
    int _brightness = WS_DEFAULT_BRIGHTNESS;
    bool _initialized = false;


};

#ifdef WS_GLOBAL_INSTANCE

WSControl WsControlInstance = WSControl();

#endif

#endif
#endif //VISORV3_WSCONTROL_H
