#include <Arduino.h>
#include "MainSystem.h"


void setup() {
    Serial.begin(115200);
    MainSystem::getMainSystem().setup();
}

void loop() {
    MainSystem::getMainSystem().loop();
    yield();
}