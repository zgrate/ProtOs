//
// Created by dzing on 03/02/2021.
//

#include "MainSystem.h"

#include <memory>

#include "libraries/PxMatrix.h"
#include "control/BluetoothManager.h"
#include "control/ControlFunctions.h"


TaskHandle_t Task1;

unsigned long lastRefresh = 0;

void refreshScreen() {
    if ((millis() - lastRefresh) > DATA_REFRESH_TIME) {
        lastRefresh = millis();
        String voltage = MainSystem::getMainSystem().getCurrentVoltageSensor().requestData("");
        String temperature = MainSystem::getMainSystem().getThermometerSensor().requestData("");
        uint8_t fanspeed = MainSystem::getMainSystem().getQC3FanControl().getFanSpeed();
        main_ns::printOnExternalScreen(voltage, 0, false);
        main_ns::printOnExternalScreen(temperature, 1, false);
        main_ns::printOnExternalScreen("Fan: " + String(fanspeed), 2, true);
    }

}

void mainTask(void *pvParameters) {
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    while (isRunning) {
        //MAIN.mainDisplayLoop();
        //WifiManagerInstance.loop();
        //TODO: OTHERS
        MainSystem::getMainSystem().mainDisplayLoop();
        MainSystem::getMainSystem().getOledControl().oledLoop();
        // refreshScreen();
        // wifiLoop(&MainSystem::getMainSystem().getWifiManager());
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
            0,           /* priority of the task */
            &Task1,      /* Task handle to keep track of created task */
            0);          /* pin task to core 0 */
}


void MainSystem::loop() {
#ifdef PX_MATRIX_SCREEN
    getPxMatrixScreen().display();
#else
    vTaskDelay(10);
#endif

}

void MainSystem::loadNewFile(const String &filename) {

#ifdef BLUETOOTH_SUPPORT
    BluetoothClientInstance.stopAdvertising();
#endif
    //auto loadingdata = std::shared_ptr<LoadedData>(new LoadedData());
    //loadingdata->loadFile(filename); //TODO
    if (!currentData.loadFile(filename))
        return;
    if (currentData.isInitialized()) {
        Serial.println("Data loaded! ");
        main_ns::printOnExternalScreen(currentData.getAnimationName(), 5, true);
        currentData.printContents();
        currentFramePx = 0;
        currentFrameMax = 0;
        currentFrameWs = 0;
        frameTime = 1000 / currentData.getFPS();

        //currentData = loadingdata;

    }
#ifdef BLUETOOTH_SUPPORT
    BluetoothClientInstance.startAdvertising();
#endif

}

bool main_ns::readInputState(const int &pinNumber) {
    Serial.println(analogRead(pinNumber));
    return analogRead(pinNumber) < 2000;
}

void main_ns::forwardPacket(const std::shared_ptr<ClientBoundPacket> &packet) {
#ifdef WIFI_SUPPORT
    if (packet->getPipeline() == PacketPipeline::WIFI) {
        MainSystem::getMainSystem().getWifiManager().sendPacket(packet);
    }
#endif
#ifdef BLUETOOTH_SUPPORT
    if (packet->getPipeline() == PacketPipeline::BLUETOOTH) {
        BluetoothClientInstance.sendPacket(packet);
    }
#endif
}

void MainSystem::beginAll() {
#ifdef SD_SUPPORT
    beginAndTestSD();
#endif

#ifdef PX_MATRIX_SCREEN
    getPxMatrixScreen().begin();
    startDisplayThread();
#endif
#ifdef MAX_MATRIX_SCREEN
    max7219Screen.begin();
#endif
#ifdef WS_MATRIX_SCREEN
    wsControl.begin();
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
#ifdef BLUETOOTH_SUPPORT
    if (!main_ns::readInputState(PIN_INPUT_DIPSWITCH_1)) {
#else
        if(true){
#endif
        wifiManager.begin();
    }
#endif
#ifdef BLUETOOTH_SUPPORT
#ifdef WIFI_SUPPORT
    if (main_ns::readInputState(PIN_INPUT_DIPSWITCH_1)) {
#else
        if(true){
#endif
        BluetoothClientInstance.setupBLE();
    }
#endif


}

unsigned long timer = 0;
bool loaded = false;
bool single_frame_px_animation = false;

void MainSystem::mainDisplayLoop() {
    if (timer == 0) {
        timer = millis();
    } else if (!loaded && (millis() - timer) > 3000) {
        startSystem();
        loaded = true;
    }
    if (!currentData.isInitialized()) {
        vTaskDelay(10);
        return;
    }
    if (millis() - lastFrame >= frameTime) {
        lastFrame = millis();
#ifdef PX_MATRIX_SCREEN
        if (currentData.getPxFrames() != 0 && !single_frame_px_animation) {
            if (currentFramePx == 0) {
                currentData.seekPXMatrixData(0);
            }
            currentData.readPxToBuffer(getPxMatrixScreen().getBufferAddress());
            //getPxMatrixScreen().showBuffer();

//            if (!currentData->isStreaming()) {
////                getPxMatrixScreen().writeFrameFromBuffer(data->getPXFrameStartAddress(currentFramePx),
////                                                         data->getPxFrameSize());
////TODO: Only streaming for now!

//            } else {
//
//            }
            currentFramePx++;
            if (currentFramePx >= currentData.getPxFrames()) {
                currentFramePx = 0;
            }
        }
#endif
#ifdef MAX_MATRIX_SCREEN
        if (currentData.getMaxFrames() != 0) {
            max7219Screen.writeFrameFromBuffer(currentData.getMaxFrameStartAddress(currentFrameMax),
                                               currentData.getMaxFrameSize());
            currentFrameMax++;
            if (currentFrameMax >= currentData.getMaxFrames()) {
                currentFrameMax = 0;
            }
        }
#endif
#ifdef WS_MATRIX_SCREEN
        if (data->getWsFrames() != 0) {

            wsControl.writeFrameFromBuffer(data->getWSFrameStartAddress(0), data->getWsFrameSize());
            currentFrameWs++;
            if (currentFrameWs >= data->getWsFrames()) {
                currentFrameWs = 0;
            }
        }
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
//                if (WsControlInstance.getAnimationMode() == AnimationMode::LIVE_ANIMATION) {
//                    WsControlInstance.drawPixels(vector);
//                }
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
                MainSystem::getMainSystem().getPxMatrixScreen().clearDisplay();
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
            //if (WsControlInstance.getAnimationMode() == AnimationMode::LIVE_ANIMATION) {
            //MainSystem::getMainSystem().getWsControl().clear();
            //}
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
        Serial.println("Setting PX brigthness...");
        MainSystem::getMainSystem().getPxMatrixScreen().setBrightness(stringValue.toInt());
    }
#endif
#ifdef MAX_BRIGTHNESS_CONTROL
    if (controlId == MAX_BRIGTHNESS_CONTROL) {
        Serial.println("Setting MAX Brightness... ");
        MainSystem::getMainSystem().getMaxScreen().setBrightness(stringValue.toInt());
    }
#endif
}

void main_ns::printOnExternalScreen(const String &line, const int &lineNumber, const bool &update) {
#ifdef OLED_SUPPORT
    MainSystem::getMainSystem().getOledControl().setLine(line, lineNumber, update);
#endif
}

void main_ns::load_animation(const char *string) {
    MainSystem::getMainSystem().loadNewFile("/" + String(string) + ".zgd");
}

void main_ns::load_animation_from_number(int number) {
    MainSystem::getMainSystem().loadNewFile("/" + String(number) + ".zgd");
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
    Serial.print(maxFrameSize);
//
//    Serial.println("WS: ");
//    Serial.print("Frames: ");
//    Serial.print(wsFrames);
//    Serial.print(" with size ");
//    Serial.print(wsSizeX);
//    Serial.print("x");
//    Serial.print(wsSizeY);
//    Serial.print(" with a memory address ");
//    Serial.println((long) wsData);

    Serial.println("PX: ");
    Serial.print("PX MODE: ");
    Serial.print(isStreaming());
    Serial.print(" Frames: ");
    Serial.print(pxFrames);
    Serial.print(" with size ");
    Serial.print(" with a memory address ");

    Serial.print("PX FileName: ");
    Serial.print(pxFileName);
    Serial.print(" PX position: ");
    Serial.print(pxFileStart);


}

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

bool LoadedData::isInitialized() const {
    return initialized;
}

const String &LoadedData::getAnimationName() const {
    return animationName;
}

bool LoadedData::loadFile(const String &filename) {
    Serial.println(String("LOADING FILE " + filename));
    if (!SD_MMC.exists(filename)) {
        Serial.println("File not exists!");
        return false;
    }
    /**
     * Loading important stuff
     */
    auto file = SD_MMC.open(filename);
    if (!file) {
        Serial.println("Unknown file error!");
        return false;
    }
    this->fileName = filename;
    uint8_t type = file.read();
    if (type != PX_INFO_FILE) {
        Serial.println("File is not PX_INFO!");
        file.close();
        return false;
    }
    //this->packetLength = readULong(file); //TODO: Use this variable to help determine if we have a reading problem
    this->version = (file.read());
    if (this->version != LOADED_DATA_VERSION) {
        Serial.println("Version don't match!");
        file.close();
        return false;
    }
    this->animationName = readString(file);

    this->flags = file.read();
    this->fps = file.read();

    /**
     * Start reading headers!
     */
    Serial.println(file.available());

    while (file.available() > 0) {
        uint8_t readType = file.read();
        Serial.println("Header " + String(readType));
        if (readType == MAX_HEADER) {
            Serial.println("Loading MAX Data....");
            this->maxFrames = readShort(file); //MAX 65k (lower!)

            if (this->maxFrames > 0) {
                this->maxFrameSize = readShort(file);
                Serial.println(this->maxFrameSize);
#ifdef MAX_MATRIX_SCREEN
                /**
                 * Max Loading
                 */
                if (this->maxFrameSize != MAX_MATRICES_NUMBER * 8 || this->maxFrames > MAX_FRAMES_BUFFER) {
                    file.seek(file.position() + this->maxFrameSize * maxFrames);
                    this->maxFrames = 0;
                    this->maxFrameSize = 0;
                    Serial.println("Error loading MAX Data!");
                    //return false;
                    //TODO: Send Error ""up""
                } else {
                    Serial.println(file.available());
                    this->maxData = GLOBAL_MAX_BUFFOR;
                    for (int i = 0; i < maxFrames; i++) {
                        file.read(maxData + maxFrameSize * i, maxFrameSize);
                    }
                    Serial.println("Loaded MAX Data!");
                    Serial.println(file.available());
                }

#else
                file.seek(file.position() + this->maxFrameSize * maxFrames);
                    this->maxFrames = 0;
                        //TODO: Send Error ""up""
#endif

            }
        } else if (readType == PX_HEADER) {
            Serial.println("Loading PX Data...");
            this->pxFrames = readShort(file);
            Serial.println(this->pxFrames);
            if (this->pxFrames > 0) {
                this->pxStreaming = file.read();
                Serial.println(this->pxStreaming);
                this->pxFileName = readString(file);
                Serial.println(this->pxFileName);
                this->pxFrameSize = readShort(file);
                if (this->pxFrameSize != PxMATRIX_FRAME_SIZE) {
                    Serial.println("PxMatrix Frame size is invalid!");
                    return false;
                    //file.seek(file.position() + this->pxFrameSize * pxFrames);
                    //TODO
                }
                Serial.println("Dir");
                vTaskDelay(10);
                int lastpos = filename.lastIndexOf("/");
                String pxFileDir = String(String(filename.substring(0, lastpos + 1)) + this->pxFileName);
                Serial.println(pxFileDir);
                delay(100);

                if (!SD_MMC.exists(pxFileDir)) {
                    //TODO - ERROR
                    Serial.println("Streaming file don't exists");
                    return false;
                } else {
                    if (this->pxFile) {
                        this->pxFile.close();
                    }
                    this->pxFile = SD_MMC.open(pxFileDir);

                }
                Serial.println("PX Loaded!!");
            }
        } else {
            Serial.println("Critical error reading the file! This is probably not a valid file!");
            return false;
        }

    }
    file.close();
    initialized = true;
    return true;


}

#ifdef PX_MATRIX_SCREEN
//void mainPxScreenThread() {
//    portENTER_CRITICAL_ISR(&MainSystem::getMainSystem().getPxMatrixScreen().timerMux);
//    MainSystem::getMainSystem().getPxMatrixScreen().display();
//    //   display.displayTestPattern(70);
//    portEXIT_CRITICAL_ISR(&MainSystem::getMainSystem().getPxMatrixScreen().timerMux);
//
//}
#endif