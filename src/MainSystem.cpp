//
// Created by dzing on 03/02/2021.
//

#include "MainSystem.h"

#include "libraries/TestMode.h"
#include "libraries/PxMatrix.h"
#include "control/BluetoothManager.h"
#include "control/ControlFunctions.h"
#include "libraries/Max7219.h"
MainSystem MAIN = MainSystem();


void MainSystem::setup() {
    //executeTests();
    Max7219ControlInstance.drawTestPattern();
   // BluetoothClientInstance.setupBLE();
   // PxMatrixControlInstance.test();
}

void MainSystem::loop() {
    //BluetoothClientInstance.loopBLE();
}

void MainSystem::loadNewFile(const String& string) {

}

