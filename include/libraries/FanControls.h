#ifndef QC3CLIENT
#define QC3CLIENT

#include "ConstantsAndSettings.h"

#ifdef FAN_QC_SUPPORT

#include "Arduino.h"

class QC3ClientClass {
private:
    enum QC3Pins { //order is important!!!
        dp = 4,
        dn = 5
    };
    uint8_t pinsInit = 1, qcInit = 0;
    uint8_t pins[4];
    uint16_t voltageLevel = 5000;
    uint32_t initTimer = 0;
    uint8_t continuousMode = 0;

    void setPinVoltage(QC3Pins pin, int16_t milliVolts) {

        if (pin != dp && pin != dn) return;

        int pinNumber = 0;
        int p = 0;
        if (pin == dp) {
            pinNumber = PIN_DAC_USB_DPLUS;
            p = PIN_OUTPUT_USB_DPLUS;
        } else {
            pinNumber = PIN_DAC_USB_DMINUS;
            p = PIN_OUTPUT_USB_DMINUS;
        }

        if (milliVolts == 0) {
            dacWrite(pinNumber, 0);
            //pinMode(pinNumber, OUTPUT);
            //digitalWrite(pinNumber, 0);
//        pinMode(dX2k2, OUTPUT);
//        pinMode(dX10k, OUTPUT);
//        digitalWrite(dX2k2, LOW);
//        digitalWrite(dX10k, LOW);
        } else if (milliVolts == 3300) {
            dacWrite(pinNumber, 255);
            //pinMode(pinNumber, OUTPUT);
            //digitalWrite(pinNumber, 1);
//        pinMode(dX2k2, OUTPUT);
//        pinMode(dX10k, OUTPUT);
//        digitalWrite(dX2k2, HIGH);
//        digitalWrite(dX10k, HIGH);
        } else if (milliVolts == 600) {
            dacWrite(pinNumber, 50);
            //digitalWrite(pinNumber, 0);
            //pinMode(pinNumber, INPUT);
//        pinMode(dX2k2, OUTPUT);
//        pinMode(dX10k, OUTPUT);
//        digitalWrite(dX2k2, LOW);
//        digitalWrite(dX10k, HIGH);
        } else if (milliVolts == 2700) { //never used
//        pinMode(dX2k2, OUTPUT);
//        pinMode(dX10k, OUTPUT);
//        digitalWrite(dX2k2, HIGH);
//        digitalWrite(dX10k, LOW);
        } else {
            pinMode(p, INPUT);
            //pinMode(dX10k, INPUT);
        }
    }

public:
    QC3ClientClass() {}

    uint8_t begin(uint8_t QC2Mode = 0, uint8_t blocking = 1) {
        if (pinsInit) {
            if (initTimer == 0) {
                setPinVoltage(dp, 600);
                setPinVoltage(dn, -1);

                if (blocking) {
                    delay(1300);
                    setPinVoltage(dp, 600);
                    setPinVoltage(dn, 0);
                    delay(100);
                    initTimer = 1;
                    qcInit = 3 - (!!QC2Mode);
                    if (qcInit == 3) { //QC3 handshake
                        setPinVoltage(dp, 600);
                        setPinVoltage(dn, 3300);
                        delay(100); //stabilize
                        voltageLevel = 5000;
                    }
                    return 0;
                }
                initTimer = millis() + 1; // +1 to avoid millis() = 0 bug on super fast startup
                return 1;
            } else if (blocking == 0 && millis() - initTimer > 1300) {
                setPinVoltage(dp, 600);
                setPinVoltage(dn, 0);
                delay(100);
                qcInit = 3 - (!!QC2Mode);
                if (qcInit == 3) { //QC3 handshake
                    setPinVoltage(dp, 600);
                    setPinVoltage(dn, 3300);
                    delay(100); //stabilize
                    voltageLevel = 50;
                }
                return 0;
            }
        } else if (qcInit) {
            return 0;
        }
        return -1;
    }

    void end() {
        setPinVoltage(dp, 0);
        setPinVoltage(dn, 0);
        initTimer = 0;
        qcInit = 0;
    }

    uint16_t setMillivolts(uint16_t millivolts) {
        if (qcInit && millivolts <= 12000 && millivolts >= 3600) {
            uint16_t normalizedMillivolts = millivolts / 100;

            if (normalizedMillivolts % 2) {
                normalizedMillivolts--;
            }
            normalizedMillivolts *= 100;

            if (normalizedMillivolts == voltageLevel) return voltageLevel;

            if (qcInit == 3) {
                if (continuousMode == 0) { //handshake
                    setPinVoltage(dp, 600);
                    setPinVoltage(dn, 3300);
                    delay(1);
                    continuousMode = 1;
                    voltageLevel = 5000;
                }

                while (voltageLevel < normalizedMillivolts && voltageLevel < 12000) {
                    setPinVoltage(dp, 3300);
                    setPinVoltage(dn, 3300);
                    delay(1);
                    setPinVoltage(dp, 600);
                    setPinVoltage(dn, 3300);
                    delay(1);
                    voltageLevel += 200;
                }
                while (voltageLevel > normalizedMillivolts && voltageLevel > 3600) {
                    setPinVoltage(dp, 600);
                    setPinVoltage(dn, 600);
                    delay(1);
                    setPinVoltage(dp, 600);
                    setPinVoltage(dn, 3300);
                    delay(1);
                    voltageLevel -= 200;
                }
            } else if (qcInit == 2 && (normalizedMillivolts == 12000 || normalizedMillivolts == 9000 ||
                                       normalizedMillivolts == 5000)) {
                switch (normalizedMillivolts) {
                    case 12000:
                        setPinVoltage(dp, 600);
                        setPinVoltage(dn, 600);
                        delay(1);
                        voltageLevel = normalizedMillivolts;
                        break;
                    case 9000:
                        setPinVoltage(dp, 3300);
                        setPinVoltage(dn, 600);
                        delay(1);
                        voltageLevel = normalizedMillivolts;
                        break;
                    case 5000:
                        setPinVoltage(dp, 600);
                        setPinVoltage(dn, 0);
                        delay(1);
                        voltageLevel = normalizedMillivolts;
                        break;
                }
            } else {
                return 0;
            }
            return voltageLevel;
        }
        return 0;
    }

    uint16_t getMillivolts() const {
        return voltageLevel;
    }
};

class QC3FanControl : public Control {

private:
    QC3ClientClass qc3ClientClass;
    uint8_t currentFanSpeedPercentage;
    int millisIncrement = (QC_MAX_VOLTAGE - QC_MIN_VOLTAGE) / 100;
    bool _initialized = false;
public:

    void begin() override {
        qc3ClientClass.begin();
        _initialized = true;
    }

    String control(const String &data) override {
        int percentage = data.toInt();
        if (percentage > 0 && percentage <= 100) {
            currentFanSpeedPercentage = percentage;
            qc3ClientClass.setMillivolts(QC_MIN_VOLTAGE + percentage * millisIncrement);
            return "ok";
        }
        return "invalid";
    }

    uint8_t getFanSpeed() const {
        return currentFanSpeedPercentage;
    }

    void test() override {
        if (!_initialized)
            begin();
        control("60");
        Serial.print("Fan set to 60%. Check if voltage is ");
        Serial.println(qc3ClientClass.getMillivolts());
    }

};

QC3FanControl QC3FanControlInstance = QC3FanControl();

#endif
#endif