//
// Created by dzing on 28/05/2021.
//

#ifndef VISORV3_DEFINITIONS_H
#define VISORV3_DEFINITIONS_H


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

enum SensorType{
    UNKNOWN_SENSOR = 0, TEMPERATURE_HUMIDITY = 1, TEMPERATURE = 2, HUMIDITY = 3, CURRENT_VOLTAGE = 4, IR_RECEIVER = 5,
    CONTACTRON = 6, MAGNETIC_SENSOR = 7, ADC = 8, DIGITAL_PIN = 9
};

enum AnimationMode{
    UNKNOWN_ANIMATION_MODE = 0, TEST_MODE = 1, ANIMATION_FROM_FILE = 2, LIVE_ANIMATION = 3
};

#endif //VISORV3_DEFINITIONS_H
