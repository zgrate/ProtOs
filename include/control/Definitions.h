//
// Created by dzing on 28/05/2021.
//

#ifndef VISORV3_DEFINITIONS_H
#define VISORV3_DEFINITIONS_H

#include <map>
#include "ArduinoJson.h"

class Pixel {

public:

    uint8_t x, y, r, g, b;

    Pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) : x(x), y(y), r(r), g(g), b(b) {}
};


enum FileOperationType{

    UNKNOWN_OP = 0, LS = 1, MKDIR = 2, TOUCH = 3, DEL = 4, MV = 5, CP = 6, DOWN = 7

};

enum FileResponseType{
    UNKNOWN_RESPONSE = 0, OK_RES = 1, NOT_FOUND_RES = 2, INVALID_RES = 3
};

enum SensorType {
    UNKNOWN_SENSOR = 0, TEMPERATURE_HUMIDITY = 1, TEMPERATURE = 2, HUMIDITY = 3, CURRENT_VOLTAGE = 4, IR_RECEIVER = 5,
    CONTACTRON = 6, MAGNETIC_SENSOR = 7, ADC = 8, DIGITAL_PIN = 9
};

enum AnimationMode {
    UNKNOWN_ANIMATION_MODE = 0, TEST_MODE = 1, ANIMATION_FROM_FILE = 2, LIVE_ANIMATION = 3
};


class Sensor {
public:
    uint8_t id = 0;

    virtual String requestData(const String &data) = 0;

    virtual void test() = 0;
};

class Control {
public:
    uint8_t id = 0;

    virtual String control(const String &data) = 0;

    virtual void test() = 0;
};

enum CapabilitiesType {
    UNKNOWN_CAPA = 0, SCREEN_CAPA = 1, SENSOR_CAPA = 2, CONTROL_CAPA = 3
};

class CapabilitiesEntry {
public:
    uint8_t id = 0;
    CapabilitiesType type = CapabilitiesType::UNKNOWN_CAPA;
    std::map<String, String> details;

    CapabilitiesEntry(uint8_t id, CapabilitiesType type) {
        this->id = id;
        this->type = type;
    };

    void addDetail(const String &key, const String &value) {
        details[key] = value;
    }

    DynamicJsonDocument toJson() {
        DynamicJsonDocument doc(1024);
        doc["id"] = id;
        doc["type"] = type;
        auto a = doc.createNestedObject("details");
        for (const auto &e : details) {
            a[e.first] = e.second;
        }
        return doc;
    }

};

#endif //VISORV3_DEFINITIONS_H
