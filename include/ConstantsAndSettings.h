//
// Created by dzing on 29/01/2021.
//

#ifndef VISORV3_CONSTANTSANDSETTINGS_H
#define VISORV3_CONSTANTSANDSETTINGS_H

#include <Arduino.h>
#include <cstring>
#include "control/Definitions.h"

//#define Z_CONFIGURATION
#define STANDARD_MAX_CONFIGURATION


//TEST MODE - CONNECT ESP THROUGH SERIAL AND CHECK IF EVERYTHING WORKS
#define TEST_MODE



//Import pin definitions
#ifdef Z_CONFIGURATION

#include "configurationspresets/ZMatrixConfiguration.h"

#endif

#ifdef STANDARD_MAX_CONFIGURATION

#include "configurationspresets/SimpleMaxConfiguration.h"

#endif

#define CONFIGURATION_FILE "/config.json"

//Define in you want to disable debug output
//#define DISABLE_DEBUG

extern bool isRunning;
extern String ipAddress;

//Useful, global methods
String getCapabilitiesJson();

void constructCapabilitiesList();

void writeToRegisterPin(const uint8_t &pin, const uint8_t &state);

void debugPrint(const String &text);

void debugPrint(const int &text);

SensorType getSensorType(const uint8_t &id);

ControlType getControlType(const uint8_t &id);

ScreenType getScreenType(const uint8_t &id);

#endif //VISORV3_CONSTANTSANDSETTINGS_H
