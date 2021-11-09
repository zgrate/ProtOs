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

    String lines[7];
    bool updated = true;
public:

    OLEDControl() = default;

    void begin() {
        oled.begin();
        for (int i = 0; i < 7; ++i) {
            lines[i] = "";
        }
        ini = true;
    }

    void test() {
        if (!ini)
            begin();
        oled.setFont(u8x8_font_amstrad_cpc_extended_f);
        oled.clearDisplay();
        setLine("ABCDEFGHIJKLMNO", 0);
        setLine("PRSTUWXYZ", 1);
//        oled.println("1234567890");
//        oled.println("!@#$%^&*()");
//        oled.println("<3 E> :) :D :>");
//        oled.println("--------------");
//        oled.println("BEEP BOOP BOOP");

    }

    void clear() {
        oled.clear();
    }

    void println(const String &line) {
        oled.println(line.c_str());
    }

    void setLine(const String &line, const int &lineNumber, const bool &update = true) {
        //oled.clearDisplay();
        if (lineNumber >= 0 and lineNumber <= 6) {
            lines[lineNumber] = line;
            if (update)
                updated = true;
        }
        //oled.clearLine(lineNumber);

        // oled.println(line.c_str());
    }

    void oledLoop() {
        if (updated) {
            oled.clearDisplay();
            oled.setCursor(0, 0);
            for (int i = 0; i < 7; i++) {
                oled.println(lines[i].c_str());
            }
            updated = false;
        }
    }
};
#endif


#endif //VISORV3_OLEDCONTROL_H
