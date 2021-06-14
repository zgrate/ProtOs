//
// Created by dzing on 29/01/2021.
//

#ifndef VISORV3_CONSTANTSANDSETTINGS_H
#define VISORV3_CONSTANTSANDSETTINGS_H

#include <Arduino.h>
#include <cstring>
#include "control/Definitions.h"

/**
 * What configuration you want to get enabled? Here, you can switch between preprogrammed options, or define your own!
 */
//#define Z_CONFIGURATION
#define STANDARD_MAX_CONFIGURATION

/**
 * Import pin definitions
 * It is recommended to add your configuration here, as another option
 */
#if defined(Z_CONFIGURATION)

#include "configurationspresets/ZMatrixConfiguration.h"

#elif defined(STANDARD_MAX_CONFIGURATION)

#include "configurationspresets/SimpleMaxConfiguration.h"

#endif

#define CONFIGURATION_FILE "/config.json"

//Define in you want to disable debug output
//#define DISABLE_DEBUG

/**
 * Variable used by every infinite loop to determine if they should work. It should never be changed
 */
extern bool isRunning;


//Useful, global methods
/**
 * Gets all capabilities as a JSON formatted string
 * Note: You need to execute @link constructCapabilitiesList() first
 * @return capabilities list defined in the system
 */
String getCapabilitiesJson();

/**
 * Constructs the capabilities file from current configuration
 */
void constructCapabilitiesList();

/**
 * Writes state to the pin on shift register. Used with PxMatrix, as there are 4 free pins on shift register
 * @param pin
 * @param state
 */
void writeToRegisterPin(const uint8_t &pin, const uint8_t &state);

/**
 * Print text to the Serial. Can be disabled with a flag
 * @param text Text to print
 */
void debugPrint(const String &text);

/**
 * Int version of printing to the Serial. Can be disabled with a flag
 * @param number Number to print
 */
void debugPrint(const long &number);

/**
 * Gets type of the sensor with given ID
 * @param id id to check
 * @return sensor type
 */
SensorType getSensorType(const uint8_t &id);

/**
 * Gets type of the control with given ID
 * @param id id to check
 * @return control type
 */
ControlType getControlType(const uint8_t &id);

/**
 * Gets type of the screen with given ID
 * @param id id to check
 * @return screen type
 */
ScreenType getScreenType(const uint8_t &id);

#endif //VISORV3_CONSTANTSANDSETTINGS_H
