//
// Created by dzing on 30/01/2021.
//

#ifndef VISORV3_TESTMODE_H
#define VISORV3_TESTMODE_H

#include "ConstantsAndSettings.h"

#ifdef TEST_MODE

#include "Arduino.h"
#include "WSControl.h"
#include "PxMatrix.h"
#include "ThermometerControl.h"
#include "Max7219.h"
#include "QCFanControl.h"
#include "OLEDControl.h"
#include "SDCardControl.h"
#include "control/WifiManager.h"
#include "control/Packets.h"
#include "INA219.h"

void executeTests() {

    Serial.println("STARTING TESTS....");
    ThermometerControlInstance.test();
    //WsControlInstance.test();
    //Max7219ControlInstance.test();
    PxMatrixControlInstance.test();
//CurrentMeterInstance.begin();
    FanControlInstance.test();
//WifiManagerInstance.connect();
    testSD();
#ifdef OLED_SUPPORT //TODO TEST MUST ME MADE USING I2C SCANNER
    OledControlInstance.test();
#endif
    Serial.println("TEST COMPLETED!");
};

void dw(int pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 1);
};

void testPins()
{
    for(int i = 1; i < 40; i++) {
        dw(i);
    }
};

#endif
#endif //VISORV3_TESTMODE_H
