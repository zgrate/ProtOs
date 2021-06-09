//
// Created by dzing on 03/02/2021.
//

#include "MainSystem.h"

#include "libraries/TestMode.h"
#include "libraries/PxMatrix.h"
#include "control/BluetoothManager.h"
#include "control/ControlFunctions.h"


TaskHandle_t Task1;

void mainTask(void *pvParameters) {
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    while (isRunning) {
        //MAIN.mainDisplayLoop();
        //WifiManagerInstance.loop();
        //TODO: OTHERS
        MainSystem::getMainSystem().mainDisplayLoop();
        vTaskDelay(10);
        yield();
    }
}

void MainSystem::setup() {
    constructCapabilitiesList();
    beginAll();
    executeTests();
    xTaskCreatePinnedToCore(
            mainTask,   /* Task function. */
            "Task1",     /* name of task. */
            10000,       /* Stack size of task */
            nullptr,        /* parameter of the task */
            1,           /* priority of the task */
            &Task1,      /* Task handle to keep track of created task */
            0);          /* pin task to core 0 */
}

unsigned long loopTime = 0;

unsigned long lastTick = 0;

unsigned long testTimer = 0;

void MainSystem::loop() {

    //delay(1000);//
//    testTimer = micros();
#ifdef PX_MATRIX_SCREEN
    pxMatrixScreen.display();
#else
    vTaskDelay(10);
#endif

//    Serial.println(micros()-testTimer);
    // #Max7219ControlInstance.drawTestPattern();

//    vTaskDelay(10);
    return;
    //BluetoothClientInstance.loopBLE();
    //yield();
    //Serial.println(lastTime);
    //
    if ((millis() - lastTick) >= 1000) {
        lastTick = millis();
        Serial.println(loopTime);
    }
    yield();

}

void MainSystem::loadNewFile(const String &filename) {
    if (data != nullptr) {
        delete data;
    }
    data = new LoadedData();
    //data->loadFile(filename); //TODO
    currentFramePx = 0;
    currentFrameMax = 0;
    currentFrameWs = 0;
    frameTime = 1000 / data->getFPS();

}

void main_ns::forwardPacket(const std::shared_ptr<ClientBoundPacket> &packet) {
#ifdef WIFI_SUPPORT
    if (packet->getPipeline() == PacketPipeline::WIFI) {
        MainSystem::getMainSystem().getWifiManager().sendPacket(packet);
    }
#endif
}

void MainSystem::beginAll() {
#ifdef PX_MATRIX_SCREEN
    pxMatrixScreen.begin();
#endif
#ifdef MAX_MATRIX_SCREEN
//    Max7219ControlInstance.init(MAX_MATRICES_NUMBER, MAX_WIDTH, MAX_HEIGHT, PIN_OUTPUT_MAX_MOSI, PIN_OUTPUT_MAX_CLK,
//         PIN_OUTPUT_MAX_CS, (ConnectionType)MAX_CONNECTION_TYPE);
    max7219Screen.begin();

#endif
#ifdef WS_MATRIX_SCREEN
    WsControlInstance.begin();
#endif
#ifdef THERMOMETER_HYDROMETER_SENSOR
    thermometerSensor.begin();
#endif
#ifdef CURRENT_VOLTAGE_SENSOR
    currentVoltageSensor.begin();
#endif
#ifdef FAN_QC_CONTROL
    qc3FanControl.begin();
#endif
#ifdef OLED_SUPPORT
    oledControl.begin();
#endif
#ifdef WIFI_SUPPORT
    wifiManager.begin();
#endif
}


void MainSystem::mainDisplayLoop() {
    if (data == nullptr) {
        vTaskDelay(10);
        return;
    }
    if (millis() - lastFrame >= frameTime) {
        unsigned long start = micros();
        lastFrame = millis();
#ifdef PX_MATRIX_SCREEN
        if (data->getPxFrames() != 0) {
            if (!data->isStreaming()) {
                pxMatrixScreen.writeFrameFromBuffer(data->getPXFrameStartAddress(currentFramePx),
                                                    data->getPxFrameSize());
            } else {
                if (currentFramePx == 0) {
                    data->seekPXMatrixData(0);
                }
                data->readPxToBuffer(pxMatrixScreen.getBufferAddress());
            }
            currentFramePx++;
            if (currentFramePx >= data->getPxFrames()) {
                currentFramePx = 0;
            }
        }
#endif
#ifdef MAX_MATRIX_SCREEN
        if (data->getMaxFrames() != 0) {
            max7219Screen.writeFrameFromBuffer(data->getMaxFrameStartAddress(currentFrameMax),
                                               data->getMaxFrameSize());
            currentFrameMax++;
            if (currentFrameMax >= data->getMaxFrames()) {
                currentFrameMax = 0;
            }
        }
#endif
#ifdef WS_MATRIX_SCREEN
        if (data->getWsFrames() != 0) {

            WsControlInstance.writeFrameFromBuffer(data->getWSFrameStartAddress(0), data->getWsFrameSize());
            currentFrameWs++;
            if (currentFrameWs >= data->getWsFrames()) {
                currentFrameWs = 0;
            }
        }
        loopTime = micros() - start;
#endif

    }
}

static MainSystem SINGLETON;

MainSystem &MainSystem::getMainSystem() {
    return SINGLETON;
}

void main_ns::liveDrawUpdate(const uint8_t &screenId, const vector<Pixel> &vector) {
    switch (screenId) { //TODO
#ifdef PX_MATRIX_SCREEN
        case PX_MATRIX_SCREEN:
            if (MainSystem::getMainSystem().getPxMatrixScreen().getAnimationMode() == AnimationMode::LIVE_ANIMATION) {
                MainSystem::getMainSystem().getPxMatrixScreen().drawPixels(vector);
            }
            break;
#endif
#ifdef MAX_MATRIX_SCREEN
        case MAX_MATRIX_SCREEN:
            if (MainSystem::getMainSystem().getMaxScreen().getAnimationMode() == AnimationMode::LIVE_ANIMATION) {
                MainSystem::getMainSystem().getMaxScreen().drawPixels(vector);
            }
            break;
#endif
#ifdef WS_MATRIX_SCREEN
            case WS_MATRIX_SCREEN:
                if (WsControlInstance.getAnimationMode() == AnimationMode::LIVE_ANIMATION) {
                    WsControlInstance.drawPixels(vector);
                }
                break;
#endif
        default:
            break;
    }

}

void main_ns::clearDisplay(const uint8_t &screenId) {
    switch (screenId) { //TODO
#ifdef PX_MATRIX_SCREEN
        case PX_MATRIX_SCREEN:
            if (MainSystem::getMainSystem().getPxMatrixScreen().getAnimationMode() == AnimationMode::LIVE_ANIMATION) {
                MainSystem::getMainSystem().getPxMatrixScreen().clear();
            }
            break;
#endif
#ifdef MAX_MATRIX_SCREEN
        case MAX_MATRIX_SCREEN:
            if (MainSystem::getMainSystem().getMaxScreen().getAnimationMode() == AnimationMode::LIVE_ANIMATION) {
                MainSystem::getMainSystem().getMaxScreen().clear();
            }
            break;
#endif
#ifdef WS_MATRIX_SCREEN
            case WS_MATRIX_SCREEN:
            if (WsControlInstance.getAnimationMode() == AnimationMode::LIVE_ANIMATION) {
                Max7219ControlInstance.clear();
            }
            break;
#endif
        default:
            break;
    }
}

String main_ns::readSensor(const uint8_t &sensorId, uint8_t *type) {
    //TODO: Map sensors dynamically
#ifdef THERMOMETER_HYDROMETER_SENSOR
    if (sensorId == THERMOMETER_HYDROMETER_SENSOR) { //TEMPERATURE_HUMIDITY
        *type = SensorType::TEMPERATURE_HUMIDITY;
        return MainSystem::getMainSystem().getThermometerSensor().requestData("");
    }
#endif
#ifdef CURRENT_VOLTAGE_SENSOR
    if (sensorId == 2) { //CURRENT_VOLTAGE
        *type = SensorType::CURRENT_VOLTAGE;
        //return readVoltageCurrent();
        return MainSystem::getMainSystem().getCurrentVoltageSensor().requestData("");
    }
#endif
    *type = SensorType::UNKNOWN_SENSOR;
    return "sensor_not_found";

}

void main_ns::control(const uint8_t &controlId, const String &stringValue) {
    //TODO: DYNAMICALLY SET CONTROLS! FOR TIME, ONLY FAN!
#ifdef FAN_QC_CONTROL
    if (controlId == FAN_QC_CONTROL) {
        MainSystem::getMainSystem().getQC3FanControl().control(stringValue);
    }
#endif
#ifdef PX_BRIGHTNESS_CONTROL
    if (controlId == PX_BRIGHTNESS_CONTROL) {
        MainSystem::getMainSystem().getPxMatrixScreen().setBrightness(stringValue.toInt());
    }
#endif

}

void LoadedData::printContents() {
    Serial.println("---LOADED DATA---");
    Serial.print("Packet Length: ");
    Serial.println(packetLength);
    Serial.print("Filename: ");
    Serial.print(fileName);
    Serial.print(" Version: ");
    Serial.println(version);
    Serial.print("Animation name");
    Serial.println(animationName);
    Serial.print("Flags: ");
    Serial.println(flags);
    Serial.print("FPS: ");
    Serial.println(fps);

    Serial.println("MAX: ");
    Serial.print("Frames: ");
    Serial.print(maxFrames);
    Serial.print(" with size ");
    Serial.print(maxSizeX);
    Serial.print("x");
    Serial.print(maxSizeY);
    Serial.print(" with a memory address ");
    Serial.println((long) maxData);

    Serial.println("WS: ");
    Serial.print("Frames: ");
    Serial.print(wsFrames);
    Serial.print(" with size ");
    Serial.print(wsSizeX);
    Serial.print("x");
    Serial.print(wsSizeY);
    Serial.print(" with a memory address ");
    Serial.println((long) wsData);

    Serial.println("PX: ");
    Serial.print("PX MODE: ");
    Serial.print(isStreaming());
    Serial.print(" Frames: ");
    Serial.print(pxFrames);
    Serial.print(" with size ");
    Serial.print(pxSizeX);
    Serial.print("x");
    Serial.print(pxSizeY);
    Serial.print(" with a memory address ");
    Serial.println((long) pxData);
    Serial.print("PX FileName: ");
    Serial.print(pxFileName);
    Serial.print(" PX position: ");
    Serial.print(pxFileStart);


}

//void LoadedData::loadFile(String filename) {
//    this->fileName = filename;
//    Serial.print("Opening file ");
//    Serial.println(filename);
//    File mainFile = SD_MMC.open("/" + filename);
//    if (mainFile.available() > 0) {
//        uint8_t d[4];
//        mainFile.read(d, 4);
//        packetLength = 0 + (d[0] << 24) + (d[1] << 16) + (d[2] << 8) + (d[3]);
//        version = mainFile.read();
//
//        if (version != DATA_VERSION) {
//            mainFile.close();
//            return;
//        }
//
//        // uint8_t stringLength = mainFile.read();
//        animationName = mainFile.readStringUntil((char) 0);
//
////            mainFile.readBytes(c, stringLength);
////
////            for(char cha : c)
////            {
////                Serial.print((int)cha);
////                Serial.print(" ");
////            }
//
//
//
////            for(int i = 0; i < stringLength; i++){
////
////                c[i] = (char)mainFile.read();
////                Serial.print(c[i]);
////                Serial.print(", ");
////            }
//        Serial.println();
//        Serial.println(animationName);
//
////            animationName = String(c);
//
//        flags = mainFile.read();
//        fps = mainFile.read();
//        mainFile.read(unused, 2);
//
//        while (mainFile.available() > 0 && mainFile.position() < packetLength) {
//            uint8_t type = mainFile.read();
//            if (type == MAX_DATA) {
//                Serial.println("MAX_DATA");
//                uint8_t temp = mainFile.read();
//                maxFrames = temp;
//                maxSizeX = mainFile.read();
//                maxSizeY = mainFile.read();
//
//                maxFrameSize = maxSizeX * maxSizeY / 8;
//
//                uint8_t calculatedFrames = maxFrames;
//
//                while (calculatedFrames > 0) {
//
//                    void *pVoid = (realloc(maxData, maxFrameSize * calculatedFrames));
//                    if (pVoid != nullptr) {
//                        maxData = static_cast<uint8_t *>(pVoid);
//                        break;
//                    } else {
//                        calculatedFrames--;
//                    }
//                }
//                if (calculatedFrames == 0) {
//                    ESP.restart();
//                }
//                mainFile.read(maxData, calculatedFrames * maxFrameSize);
//                mainFile.seek(mainFile.position() + (maxFrames - calculatedFrames) * maxFrameSize);
//                maxFrames = calculatedFrames;
//            } else if (type == WS_DATA) {
//                Serial.println("WS_DATA");
//                uint8_t temp = mainFile.read();
//                wsFrames = temp;
//                wsSizeX = mainFile.read();
//                wsSizeY = mainFile.read();
//
//                wsFrameSize = wsSizeX * wsSizeY * 3;
//
//                uint8_t calculatedFrames = wsFrames;
//
//                while (calculatedFrames > 0) {
//
//                    void *pVoid = (realloc(wsData, wsFrameSize * calculatedFrames));
//                    if (pVoid != nullptr) {
//                        wsData = static_cast<uint8_t *>(pVoid);
//                        break;
//                    } else {
//                        calculatedFrames--;
//                    }
//                }
//                if (calculatedFrames == 0) {
//                    ESP.restart();
//                }
//                mainFile.read(wsData, calculatedFrames * wsFrameSize);
//                mainFile.seek(mainFile.position() + (wsFrames - calculatedFrames) * wsFrameSize);
//                wsFrames = calculatedFrames;
//            } else if (type == PX_IN_FILE_DATA) {
//                Serial.println("PX_IN_FILE");
//                pxMode = PX_IN_FILE_DATA;
//                uint8_t temp = mainFile.read();
//                pxFrames = temp;
//                pxSizeX = mainFile.read();
//                pxSizeY = mainFile.read();
//
//                pxFrameSize = pxSizeX * pxSizeY * 3 / 8;
//
//                uint8_t calculatedFrames = pxFrames;
//
//                while (calculatedFrames > 0) {
//
//                    void *pVoid = (realloc(pxData, pxFrameSize * calculatedFrames));
//                    if (pVoid != nullptr) {
//                        pxData = static_cast<uint8_t *>(pVoid);
//                        break;
//                    } else {
//                        calculatedFrames--;
//                    }
//                }
//                if (calculatedFrames == 0) {
//                    ESP.restart();
//                }
//                mainFile.read(pxData, calculatedFrames * pxFrameSize);
//                mainFile.seek(mainFile.position() + (pxFrames - calculatedFrames) * pxFrameSize);
//                pxFrames = calculatedFrames;
//            } else if (type == PX_EXTERNAL_STREAMING) {
//                Serial.println("PX_EXTERNAL_STREAM");
//                //uint8_t len = mainFile.read();
//
////                    char s[len];
////                    for(int i = 0; i < len; i++){
////                        s[i] = (char)mainFile.read();
////                    }
////                    pxFileName = String(s);
//                pxFileName = mainFile.readStringUntil((char) 0);
//                if (!SD_MMC.exists("/" + pxFileName)) {
//                    Serial.println(pxFileName + " not found!");
//                    pxFileName = filename.substring(0, filename.lastIndexOf('.')) + ".ser";
//                    if (!SD_MMC.exists("/" + pxFileName)) {
//                        Serial.println("ERROR: FILE NOT FOUND!");
//                        return;
//                    }
//                }
//
//                pxFile = SD_MMC.open("/" + pxFileName);
//
//                if (pxFile.available() > 0) {
//                    pxMode = PX_EXTERNAL_STREAMING;
//                    uint8_t r = pxFile.read();
//
//                    if (r != PX_EXTERNAL_STREAMING) {
//                        Serial.println("ERROR: THIS IS NOT PX_EXTERNAL_STREAMING!");
//                        return;
//                    }
//                    uint8_t framesBytes[4];
//                    pxFile.read(framesBytes, 4);
//
//                    //pxFrames = *((uint32_t *)framesBytes);
//                    pxFrames = framesBytes[3] | (framesBytes[2] << 8) | (framesBytes[1] << 16) | (framesBytes[0] << 24);
//                    //pxFrames += (((uint32_t)d[0])<<24);
//                    //pxFrames += (((uint32_t)d[1])<<16);
//                    //pxFrames += (((uint32_t)d[2])<<8);
//                    //pxFrames += (((uint32_t)d[3]));
//
//                    pxSizeX = pxFile.read();
//                    pxSizeY = pxFile.read();
//                    pxFrameSize = pxSizeX * pxSizeY * 3 / 8;
//
//                    pxFileStart = pxFile.position();
//
//                }
//
//            }
//        }
//
//    }
//}
//
void LoadedData::seekPXMatrixData(int deltaPosition) {
    if (isStreaming()) {
        pxFile.seek(pxFileStart + deltaPosition);
    }
}

void LoadedData::readPxToBuffer(uint8_t *buffer) {
    if (isStreaming() && pxFile.available() > 0) {
        pxFile.read(buffer, pxFrameSize);
    }
}


