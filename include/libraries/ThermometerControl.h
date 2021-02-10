//
// Created by dzing on 29/01/2021.
//




#ifndef VISORV3_THERMOMETERCONTROL_H
#define VISORV3_THERMOMETERCONTROL_H

#include "ConstantsAndSettings.h"
#ifdef THERMOMETER_SUPPORT

#include <Arduino.h>
#include "DHTesp.h"
#include "QCFanControl.h"

class ThermometerControl{
private:
    bool _initialized = false;
    DHTesp dht = DHTesp();
    std::vector<void(*)(float, float)> listOfListeners;
public:

    void begin()
    {
        dht.setup(PIN_DHT22TEMP, DHTesp::DHT11);
        _initialized = true;
    }

    void registerListener(void func(float, float))
    {
        listOfListeners.push_back(func);
    }

    bool getTemperature() {
        // Reading temperature for humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
        TempAndHumidity newValues = dht.getTempAndHumidity();
        // Check if any reads failed and exit early (to try again).
        if (dht.getStatus() != 0) {
            Serial.println("DHT11 error status: " + String(dht.getStatusString()));
            return false;
        }

        //float heatIndex = dht.computeHeatIndex(newValues.temperature, newValues.humidity);
        //float dewPoint = dht.computeDewPoint(newValues.temperature, newValues.humidity);
       // float cr = dht.getComfortRatio(cf, newValues.temperature, newValues.humidity);

        for(auto fun: listOfListeners)
        {
            fun(dht.getTemperature(), dht.getHumidity());
        }
        debugPrint(" T:" + String(newValues.temperature) + " H:" + String(newValues.humidity));
        return true;
    }

    void test()
    {
        if(!_initialized)
            begin();
        if(getTemperature()){
            Serial.println("Temperature Sensor connected and working!");
        }
        else{
            Serial.println("Error! Temperature Sensor not working!");
        }
    }
};

#ifdef THERMOMETER_GLOBAL_INSTANCE

    ThermometerControl ThermometerControlInstance = ThermometerControl();

#endif


#endif //VISORV3_THERMOMETERCONTROL_H
#endif