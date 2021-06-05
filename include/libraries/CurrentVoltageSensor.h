//
// Created by dzing on 04/06/2021.
//

#ifndef VISORV3_CURRENTVOLTAGESENSOR_H
#define VISORV3_CURRENTVOLTAGESENSOR_H
#ifdef CURRENT_VOLTAGE_SENSOR

#include "Arduino.h"


class CurrentVoltageSensor : public Sensor {

    INA219 CurrentControlCenter = INA219();
public:
    String requestData(const String &data) override {
        return String(CurrentControlCenter.readBusVoltage()) + "?" + String(CurrentControlCenter.readShuntCurrent());
    }

    void test() override {
        Serial.println(CurrentControlCenter.readBusVoltage());
        Serial.println(CurrentControlCenter.readShuntCurrent());

    }

    void begin() override {
        CurrentControlCenter.begin();
        CurrentControlCenter.configure(INA219_RANGE_32V, INA219_GAIN_320MV, INA219_BUS_RES_12BIT,
                                       INA219_SHUNT_RES_12BIT_1S);
        CurrentControlCenter.calibrate(CURRENT_VOLTAGE_SHUNT_OHM, CURRONT_VOLTAGE_SHUNT_VOLTAGE);

    }

};


static CurrentVoltageSensor CurrentVoltageInstance = CurrentVoltageSensor();

#endif

#endif //VISORV3_CURRENTVOLTAGESENSOR_H
