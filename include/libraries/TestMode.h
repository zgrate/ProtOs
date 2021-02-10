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


void executeTests()
{
    Serial.println("STARTING TESTS....");
    ThermometerControlInstance.test();
    WsControlInstance.test();
    Max7219ControlInstance.test();
    PxMatrixControlInstance.test();
    FanControlInstance.test();
    testSD();
#ifdef OLED_SUPPORT //TODO TEST MUST ME MADE USING I2C SCANNER
    OledControlInstance.test();
#endif
    Serial.println("TEST COMPLETED!");
}


#endif
#endif //VISORV3_TESTMODE_H
