//
// Created by dzing on 03/02/2021.
//

#ifndef VISORV3_MAINSYSTEM_H
#define VISORV3_MAINSYSTEM_H

#include "Arduino.h"

class MainSystem{

public:
    void setup();
    void loop();

    void loadNewFile(const String& string);
};

extern MainSystem MAIN;


#endif //VISORV3_MAINSYSTEM_H
