#include <Arduino.h>
#include "MainSystem.h"


void setup() {
    Serial.begin(115200);
    //manager.setupBLE();
    MAIN.setup();
    //executeTests();
    //PxMatrixControlInstance.test();
    //OledControlInstance.test();
}

void loop() {
    MAIN.loop();
    yield();

}