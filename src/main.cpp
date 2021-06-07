#include <Arduino.h>
#include "MainSystem.h"


void setup() {
    Serial.begin(115200);
    MAIN.setup();
}

void loop() {
    MAIN.loop();
    yield();

}