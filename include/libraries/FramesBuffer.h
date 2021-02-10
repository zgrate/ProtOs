//
// Created by dzing on 25/01/2021.
//



#ifndef VISORV3_FRAMESBUFFER_H
#define VISORV3_FRAMESBUFFER_H



#include <Arduino.h>

class FramesBuffer {

public:
    FramesBuffer() {
        framesBuffer = nullptr;
        frames = 0;
        currentPosition = 0;
    };
    ~FramesBuffer() {
        free(framesBuffer);
    };

    void loadFramesFromFile(string path) {
        //TODO: LOADING
    };

    int initializeBufferWithFramesDecrease(uint8_t desiredFrames) {
            if (desiredFrames > PxMATRIX_MAX_FRAMES_BUFFER) {
                desiredFrames = PxMATRIX_MAX_FRAMES_BUFFER;
            } else if (desiredFrames < 1) {
                return 0;
            }
            bool swapped = false;
            while (desiredFrames > 0) {
                auto* buffer = (uint8_t *) realloc(framesBuffer, desiredFrames * sizeof(uint8_t));

                if(buffer == nullptr)
                    desiredFrames--;
                else {
                    swapped = true;
                    framesBuffer = buffer;
                    frames = desiredFrames;
                    currentPosition = 0;
                    break;
                }

            }
            if(framesBuffer == nullptr || !swapped) {
                return -1;
            }
            else {
                return desiredFrames;
            }

    };


private:
    uint8_t* framesBuffer;
    uint8_t frames;
    uint8_t currentPosition;

};

#endif //VISORV3_FRAMESBUFFER_H
