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
#include "FanControls.h"
#include "OLEDControl.h"
#include "SDCardControl.h"
#include "control/WifiManager.h"
#include "control/Packets.h"
#include "CurrentVoltageSensor.h"
#include "INA219.h"

#ifdef SD_SUPPORT

void testSD() {
    if (!SD_MMC.begin("/sdcard", true)) {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD_MMC.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD_MMC card attached");
        return;
    }

    Serial.print("SD_MMC Card Type: ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
    Serial.printf("Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));
}

#endif

void executeTests() {
    Serial.println("STARTING TESTS....");
#ifdef THERMOMETER_HYDROMETER_SENSOR
    ThermometerControlInstance.test();
#endif
#ifdef WS_MATRIX_SCREEN
    WsControlInstance.test();
#endif
#ifdef MAX_MATRIX_SCREEN
    Max7219ControlInstance.test();
#endif
    PxMatrixControlInstance.test();
#ifdef CURRENT_VOLTAGE_SENSOR
    CurrentVoltageInstance.test(); //TODO:TEST?
#endif
#ifdef FAN_GLOBAL_INSTANCE
    QC3FanControlInstance.test();
#endif
#ifdef SD_SUPPORT
    testSD();
#endif
#ifdef OLED_SUPPORT //TODO TEST MUST ME MADE USING I2C SCANNER
    OledControlInstance.test();
#endif
    Serial.println("TESTs COMPLETED!");
};

void dw(int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 1);
};

#endif
#endif //VISORV3_TESTMODE_H
