//
// Created by dzing on 09/06/2021.
//

#include "control/WifiManager.h"

void wifiLoop(void *pvParameters) {
    auto *wifi = static_cast<WifiManager *>(pvParameters);
    while (isRunning) {
        wifi->loop();
        vTaskDelay(10);
    }
}