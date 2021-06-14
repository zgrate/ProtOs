//
// Created by dzing on 28/05/2021.
//

#ifndef VISORV3_DEFINITIONS_H
#define VISORV3_DEFINITIONS_H

#include <map>
#include "ArduinoJson.h"

/**
 * Defines one pixel in the system, used in packets
 */
class Pixel {

public:
    uint8_t x, y, r, g, b;

    Pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) : x(x), y(y), r(r), g(g), b(b) {}
};


/**
 * File operation type, used in file manager packet
 */
enum FileOperationType {

    UNKNOWN_OP = 0, LS = 1, MKDIR = 2, TOUCH = 3, DEL = 4, MV = 5, CP = 6, DOWN = 7

};

/**
 * File response type, used in file manager packets
 */
enum FileResponseType {
    UNKNOWN_RESPONSE = 0, OK_RES = 1, NOT_FOUND_RES = 2, INVALID_RES = 3
};

/**
 * Type of the sensor, determined and sent thought the capabilities list
 */
enum SensorType {
    UNKNOWN_SENSOR = 0, TEMPERATURE_HUMIDITY = 1, TEMPERATURE = 2, HUMIDITY = 3, CURRENT_VOLTAGE = 4, IR_RECEIVER = 5,
    CONTACTRON = 6, MAGNETIC_SENSOR = 7, ADC = 8, DIGITAL_PIN = 9
};
/**
 * Type of the control, determined and sent thought the capabilities list
 */
enum ControlType {
    UNKNOWN_CONTROL = 0, FAN_CONTROL = 1, OLED_CONTROL = 2, PX_BRIGHTNESS = 3, MAX_BRIGHTNESS_CONTROL = 4
};
/**
 * Mode of the animation
 */
enum AnimationMode {
    UNKNOWN_ANIMATION_MODE = 0, TESTING_MODE = 1, ANIMATION_FROM_FILE = 2, LIVE_ANIMATION = 3
};
/**
 * Type of the screen, determined and sent thought the capabilities list
 */
enum ScreenType {
    UNKNOWN_SCREEN = 0, PXMATRIX_SCREEN = 1, MAX_SCREEN = 2
};


enum DataType {
    INVALID = 0, MAX_DATA = 1, WS_DATA = 2, PX_IN_FILE_DATA = 2, PX_EXTERNAL_STREAMING_DATA = 3
};
/**
 * Used to calculate pixels for MAX7219:
 * DIRECT - connected as 1 long string of matrices
 * BULK2x2 - connected as squares 2x2
 * SIMPLE_VISOR - connected in configuraiton: 2 matrices -> 4 matrices -> 2 matrices -> 4 matrices
 */
enum ConnectionType {
    DIRECT = 1, BULK2x2 = 2, SIMPLE_VISOR = 3
};

/**
 * This class manages screen in the system
 */
class Screen {

protected:
    AnimationMode animationMode = AnimationMode::UNKNOWN_ANIMATION_MODE;

public:
    uint8_t id = 0;

    /**
     * Configures the screen and getting ready to display stuff
     */
    virtual void begin() = 0;

    /**
     * Draw pixel to the screen
     * @param vector List of pixels to draw
     */
    virtual void drawPixels(const std::vector<Pixel> &vector) = 0;

    /**
     * Clears the matrix (sets all the pixels to BLACK color)
     */
    virtual void clear() = 0;

    /**
     * Sets the brightness of the matrix
     * Note: This is implementation - specific.
     * @param target Target brightness of the matrix
     */
    virtual void setBrightness(const uint8_t &target) = 0;

    /**
     * Reads the screen data from the buffer, used in SD or similar stuff
     * @param buffer buffer to read
     * @param bufferLength  length of the buffer
     */
    virtual void writeFrameFromBuffer(const uint8_t *buffer, const int &bufferLength) = 0;

    /**
     * Get current animation mode of the screen
     * @return Animation Mode
     */
    AnimationMode getAnimationMode() {
        return animationMode;
    };

    /**
     * Sets current animation mode of the screen
     * @param mode
     */
    void setAnimationMode(const AnimationMode &mode) {
        animationMode = mode;
    };

};

/**
 * Class describing the sensor and allows the data to be downloaded from it
 * TODO: What if there is an error in the sensor? There should be an error logger system
 */
class Sensor {
public:
    uint8_t id = 0;

    /**
     * Starts the sensor and gets ready to read data from the sensor
     */
    virtual void begin() = 0;

    /**
     * Request the data from the sensor
     * @param data parameters for the sensor to get, can be empty string if no data is needed
     * @return read data from the sensor or error //TODO - HERE IT SHOULD GIVE OUT ERRORS
     */
    virtual String requestData(const String &data) = 0;

    /**
     * Runs the read test, from the sensor, and prints it on the serial
     * TODO: IT should test, not only dump stuff on the screen
     */
    virtual void test() = 0;
};

/**
 * Class for all the stuff you can control
 */
class Control {
public:
    uint8_t id = 0;

    /**
     * Starts the connection with a controller, if needed
     */
    virtual void begin() = 0;

    /**
     * Sets the control to data value
     * @param data text to pass to the controller
     * @return response from the controller if exists //TODO: Check for error checking
     */
    virtual String control(const String &data) = 0;

    /**
     * Tests the controller by setting the controller to some value
     */
    virtual void test() = 0;
};

enum CapabilitiesType {
    UNKNOWN_CAPA = 0,
    SCREEN_CAPA = 1,
    SENSOR_CAPA = 2,
    CONTROL_CAPA = 3,
    SD_CARD_CAPA = 4,
    TIME_CAPA = 5,
    WIFI_CAPA = 6,
    BT_CAPA = 7
};

class CapabilitiesEntry {
public:
    uint8_t id = 0;
    CapabilitiesType type = CapabilitiesType::UNKNOWN_CAPA;
    uint8_t detailType = 0;
    std::map<String, String> details;

    CapabilitiesEntry(uint8_t id, CapabilitiesType type, uint8_t detailType) {
        this->id = id;
        this->type = type;
        this->detailType = detailType;
    };

    void addDetail(const String &key, const String &value) {
        details[key] = value;
    }

    void addDetail(const String &key, const long &value) {
        details[key] = String(value);
    }

    DynamicJsonDocument toJson() {
        DynamicJsonDocument doc(1024);
        doc["id"] = id;
        doc["type"] = type;
        doc["detail_type"] = detailType;
        auto a = doc.createNestedObject("details");
        for (const auto &e : details) {
            a[e.first] = e.second;
        }
        return doc;
    }

};

#endif //VISORV3_DEFINITIONS_H
