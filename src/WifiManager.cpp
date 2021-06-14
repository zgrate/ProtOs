//
// Created by dzing on 09/06/2021.
//

#include "control/WifiManager.h"
#ifdef WIFI_SUPPORT
void wifiLoop(void *wifiManagerPointer) {
    auto *wifi = static_cast<WifiManager *>(wifiManagerPointer);
    while (isRunning) {
        wifi->loop();
        vTaskDelay(10);
    }
}
#endif