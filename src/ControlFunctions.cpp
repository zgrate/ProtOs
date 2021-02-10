//
// Created by dzing on 03/02/2021.
//

#include "Arduino.h"
#include "control/ControlFunctions.h"
#include "MainSystem.h"


//FIRST 2 is PIN, THIRD IS CODE
//READ FROM [3]
void processIncomingCode1(uint8_t* data){
    Serial.print("RECEIVED CODE ");
    Serial.println(data[3]);
    MAIN.loadNewFile("testfile");
}