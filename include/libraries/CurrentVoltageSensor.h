//
// Created by dzing on 04/06/2021.
//

#ifndef VISORV3_CURRENTVOLTAGESENSOR_H
#define VISORV3_CURRENTVOLTAGESENSOR_H
#ifdef CURRENT_VOLTAGE_SENSOR

#include "Arduino.h"
#include "INA219.h"

class CurrentVoltageSensor : public Sensor {

    INA219 ina219 = INA219();
public:
    String requestData(const String &data) override {
        return String(ina219.readBusVoltage()) + "?" + String(ina219.readShuntCurrent());
    }

    void test() override {
        Serial.println(ina219.readBusVoltage());
        Serial.println(ina219.readShuntCurrent());

    }

    void begin() override {
        ina219.begin();
        ina219.configure(INA219_RANGE_32V, INA219_GAIN_320MV, INA219_BUS_RES_12BIT,
                         INA219_SHUNT_RES_12BIT_1S);
        ina219.calibrate(CURRENT_VOLTAGE_SHUNT_OHM, CURRONT_VOLTAGE_SHUNT_VOLTAGE);

    }

};

#endif

#endif //VISORV3_CURRENTVOLTAGESENSOR_H
