//
// Created by dzing on 29/01/2021.
//




#ifndef VISORV3_THERMOMETERCONTROL_H
#define VISORV3_THERMOMETERCONTROL_H

#include "ConstantsAndSettings.h"
#include "control/Definitions.h"

#ifdef THERMOMETER_HYDROMETER_SENSOR

#include <Arduino.h>
#include "DHTesp.h"
#include "FanControls.h"

class ThermometerControl : public Sensor {
private:
    bool _initialized = false;
    DHTesp dht = DHTesp();

public:
    ThermometerControl() {
        this->id = THERMOMETER_HYDROMETER_SENSOR;
    }

    String requestData(const String &data) override {
        return getTemperatureAndHumidity();
    }

    void test() override {
        if (!_initialized)
            begin();
        if (getTemperatureAndHumidity()) {
            Serial.println("Temperature Sensor connected and working!");
            Serial.println(getTemperatureAndHumidity());
        } else {
            Serial.println("Error! Temperature Sensor not working!");
        }
    }

    void begin() override {
        if (!_initialized) {
            dht.setup(PIN_DHT22TEMP, DHTesp::DHT11);
            _initialized = true;
        }
    }

    String getTemperatureAndHumidity() {
        // Reading temperature for humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
        TempAndHumidity newValues = dht.getTempAndHumidity();
        // Check if any reads failed and exit early (to try again).
        if (dht.getStatus() != 0) {
            Serial.println("DHT11 error status: " + String(dht.getStatusString()));
            return "error";
        }

        //float heatIndex = dht.computeHeatIndex(newValues.temperature, newValues.humidity);
        //float dewPoint = dht.computeDewPoint(newValues.temperature, newValues.humidity);
        // float cr = dht.getComfortRatio(cf, newValues.temperature, newValues.humidity);

//        for(auto fun: listOfListeners)
//        {
//            fun(dht.getTemperature(), dht.getHumidity());
//        }
        return ("" + String(newValues.temperature) + "?" + String(newValues.humidity));
    }


};

ThermometerControl ThermometerControlInstance = ThermometerControl();


#endif //VISORV3_THERMOMETERCONTROL_H
#endif