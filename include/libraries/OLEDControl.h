//
// Created by dzing on 02/02/2021.
//

#ifndef VISORV3_OLEDCONTROL_H
#define VISORV3_OLEDCONTROL_H


#include "ConstantsAndSettings.h"

#ifdef OLED_SUPPORT

#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

class OLEDControl {
private:
    U8X8_SH1106_128X64_NONAME_HW_I2C oled = U8X8_SH1106_128X64_NONAME_HW_I2C(255, /* clock=*/ PIN_SCL, /* data=*/
                                                                             PIN_SDA/* reset=*/);
    bool ini = false;
public:

    OLEDControl() = default;

    void begin() {
        oled.begin();

        ini = true;
    }

    void test() {
        if (!ini)
            begin();
        oled.setFont(u8x8_font_amstrad_cpc_extended_f);
        oled.clear();
        oled.println("ABCDEFGHIJKLMNO");
        oled.println("PRSTUWXYZ");
        oled.println("1234567890");
        oled.println("!@#$%^&*()");
        oled.println("<3 E> :) :D :>");
        oled.println("--------------");
        oled.println("BEEP BOOP BOOP");

    }

    void reset() {
        oled.clear();
    }

    void println(const String &line) {
        oled.println(line.c_str());
    }


};
#endif


#endif //VISORV3_OLEDCONTROL_H
