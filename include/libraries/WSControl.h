//
// Created by dzing on 01/02/2021.
//

#ifndef VISORV3_WSCONTROL_H
#define VISORV3_WSCONTROL_H

#include "ConstantsAndSettings.h"
#ifdef WS_SUPPORT

#include "Arduino.h"
#include "FastLED.h"
#include "Adafruit_GFX.h"


class WSControl: public Adafruit_GFX{

public:
    WSControl(): Adafruit_GFX(WS_WIDTH, WS_HEIGHT){

    }

    void begin() {
        FastLED.addLeds<WS2811, PIN_OUTPUT_WS, GRB>(_leds, _ledsNumber);
        FastLED.setBrightness(_brightness);
        _initialized = true;
    }
    void drawPixel(int16_t x, int16_t y, uint16_t color) override{
        _leds[y*_width + x] = color;
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
            _leds[i] = CHSV(increment*i, 255, 255);
        }
        display();
        Serial.println("WS Graphics sent! Check if there is rainbow pattern on the WS");
    }
private:
    CRGB _leds[WS_NUMBEROFLEDS];
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
