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
INA219 CurrentControlCenter = INA219();

TaskHandle_t Task1;

void initCurrentMeter() {
    CurrentControlCenter.begin();
    CurrentControlCenter.configure(INA219_RANGE_32V, INA219_GAIN_320MV, INA219_BUS_RES_12BIT,
                                   INA219_SHUNT_RES_12BIT_1S);
    CurrentControlCenter.calibrate(SHUNT_OHM, SHUNT_VOLTAGE);
    Serial.println(CurrentControlCenter.readBusVoltage());
    Serial.println(CurrentControlCenter.readShuntCurrent());
}

String readVoltageCurrent() {
    return String(CurrentControlCenter.readBusVoltage()) + "?" + String(CurrentControlCenter.readShuntCurrent());
}

void mainTask(void *pvParameters) {
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    initCurrentMeter();
    WifiManagerInstance.connect();
    while (true) {
        //MAIN.mainDisplayLoop();
        WifiManagerInstance.loop();
        vTaskDelay(100);
        yield();
    }
}

void MainSystem::setup() {
    //testSD();

//    testSD();
    //initializeSDCard();
    //testPins();
    //return;


    executeTests();
    //loadNewFile("blob.zgd");
    xTaskCreatePinnedToCore(
            mainTask,   /* Task function. */
            "Task1",     /* name of task. */
            10000,       /* Stack size of task */
            nullptr,        /* parameter of the task */
            1,           /* priority of the task */
            &Task1,      /* Task handle to keep track of created task */
            0);          /* pin task to core 0 */
//    Max7219ControlInstance.begin();
//    Max7219ControlInstance.drawTestPattern();
//    delay(1000);
//    Max7219ControlInstance.drawTestPattern();
//    delay(1000);
//    Max7219ControlInstance.drawTestPattern();
    return;
    Max7219ControlInstance.writeFillRect(0, 0, 10, 10, 255);
    Max7219ControlInstance.display();
    //
    //data->printContents();
    return;

    //PxMatrixControlInstance.fullBrightness();
    executeTests();
    playInfo();
    delay(3000);
    playAnimationOnRepeat();
    return;

    delay(3000);
    Serial.println("TESTING YOUR PLAYBACK SOUND SYSTEM");
    PxMatrixControlInstance.drawPixel(0, 0, PxMatrixControlInstance.white);
    playAnimationOnRepeat();
    //Max7219ControlInstance.drawTestPattern();
    // BluetoothClientInstance.setupBLE();
    // PxMatrixControlInstance.test();
}

unsigned long loopTime = 0;

unsigned long lastTick = 0;

unsigned long testTimer = 0;

void MainSystem::loop() {

    //delay(1000);//
//    testTimer = micros();
    PxMatrixControlInstance.display();
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

void MainSystem::loadNewFile(String filename) {
    if(data != nullptr)
    {
        delete data;
    }
    data = new LoadedData();
    data->loadFile(move(filename));
    currentFramePx = 0;
    currentFrameMax = 0;
    currentFrameWs = 0;
    frameTime = 1000 / data->getFPS();

}

void MainSystem::forwardPacket(const std::shared_ptr<ClientBoundPacket> &packet) {
    if (packet->getPipeline() == PacketPipeline::WIFI) {
        WifiManagerInstance.sendPacket(packet);
    }
}

void MainSystem::initializeAllHardware() {
    PxMatrixControlInstance.begin();
    PxMatrixControlInstance.startDisplayThread();
    Max7219ControlInstance.begin();
    WsControlInstance.begin();

}


void MainSystem::mainDisplayLoop() {
    if(millis()-lastFrame >= frameTime)
    {
        unsigned long start = micros();
        lastFrame = millis();
        if(data->getPxFrames() != 0) {
            if (data->getPXMatrixMode() == PX_IN_FILE_DATA) {
                PxMatrixControlInstance.writeFrameFromBuffer(data->getPXFrameStartAddress(currentFramePx),
                                                             data->getPxFrameSize());
            }
            else if(data->getPXMatrixMode() == PX_EXTERNAL_STREAMING)
            {
                if(currentFramePx == 0)
                {
                    data->seekPXMatrixData(0);
                }
                data->  readPxToBuffer(PxMatrixControlInstance.getBufferAddress());
            }
            currentFramePx++;
            if(currentFramePx >= data->getPxFrames())
            {
                currentFramePx = 0;
            }
        }

        if(data->getMaxFrames() != 0)
        {
            Max7219ControlInstance.writeFrameFromBuffer(data->getMaxFrameStartAddress(currentFrameMax), data->getMaxFrameSize());
            currentFrameMax++;
            if(currentFrameMax >= data->getMaxFrames())
            {
                currentFrameMax = 0;
            }
        }

        if(data->getWsFrames() != 0)
        {

            WsControlInstance.writeFrameFromBuffer(data->getWSFrameStartAddress(0), data->getWsFrameSize());
            currentFrameWs++;
            if (currentFrameWs >= data->getWsFrames()) {
                currentFrameWs = 0;
            }
        }
        loopTime = micros() - start;
    }
}

void MainSystem::liveDrawUpdate(const uint8_t &screenId, const vector<Pixel> &vector) {
    switch (screenId) { //TODO
        default:
            if (PxMatrixControlInstance.getAnimationMode() == AnimationMode::LIVE_ANIMATION) {
                PxMatrixControlInstance.drawPixels(vector);
            }
    }
}

void MainSystem::clearDisplay(const uint8_t &screenId) {
    switch (screenId) { //TODO
        default:
            PxMatrixControlInstance.clearDisplay();
    }
}

String MainSystem::readSensor(const uint8_t &sensorId, uint8_t *type) {
    //TODO: Map sensors dynamically
    if (sensorId == 1) { //TEMPERATURE_HUMIDITY
        *type = SensorType::TEMPERATURE_HUMIDITY;
        return ThermometerControlInstance.requestData("");
    } else if (sensorId == 2) { //CURRENT_VOLTAGE
        *type = SensorType::CURRENT_VOLTAGE;
        return readVoltageCurrent();
    } else {
        *type = SensorType::UNKNOWN_SENSOR;
        return "sensor_not_found";
    }
}

void MainSystem::control(const uint8_t &controlId, const String &stringValue) {
    //TODO: DYNAMICALLY SET CONTROLS! FOR TIME, ONLY FAN!
    if (controlId == 1) {
        FanControlInstance.setFanSpeed(stringValue.toInt());
    } else if (controlId == 2) {
        PxMatrixControlInstance.setBrightness(stringValue.toInt());
    }

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
    Serial.println((long)maxData);

    Serial.println("WS: ");
    Serial.print("Frames: ");
    Serial.print(wsFrames);
    Serial.print(" with size ");
    Serial.print(wsSizeX);
    Serial.print("x");
    Serial.print(wsSizeY);
    Serial.print(" with a memory address ");
    Serial.println((long)wsData);

    Serial.println("PX: ");
    Serial.print("PX MODE: ");
    Serial.print(pxMode);
    Serial.print(" Frames: ");
    Serial.print(pxFrames);
    Serial.print(" with size ");
    Serial.print(pxSizeX);
    Serial.print("x");
    Serial.print(pxSizeY);
    Serial.print(" with a memory address ");
    Serial.println((long)pxData);
    Serial.print("PX FileName: ");
    Serial.print(pxFileName);
    Serial.print(" PX position: ");
    Serial.print(pxFileStart);


}

void LoadedData::loadFile(String filename) {
    this->fileName = filename;
    Serial.print("Opening file ");
    Serial.println(filename);
    File mainFile = SD_MMC.open("/" + filename);
    if(mainFile.available() > 0){
        uint8_t d[4];
        mainFile.read(d, 4);
        packetLength = 0 + (d[0]<<24) + (d[1]<<16) + (d[2]<<8) + (d[3]);
        version = mainFile.read();

        if(version != DATA_VERSION){
            mainFile.close();
            return;
        }

        // uint8_t stringLength = mainFile.read();
        animationName = mainFile.readStringUntil((char)0);

//            mainFile.readBytes(c, stringLength);
//
//            for(char cha : c)
//            {
//                Serial.print((int)cha);
//                Serial.print(" ");
//            }



//            for(int i = 0; i < stringLength; i++){
//
//                c[i] = (char)mainFile.read();
//                Serial.print(c[i]);
//                Serial.print(", ");
//            }
        Serial.println();
        Serial.println(animationName);

//            animationName = String(c);

        flags = mainFile.read();
        fps = mainFile.read();
        mainFile.read(unused, 2);

        while(mainFile.available() > 0 && mainFile.position() < packetLength){
            uint8_t type = mainFile.read();
            if(type == MAX_DATA){
                Serial.println("MAX_DATA");
                uint8_t temp = mainFile.read();
                maxFrames = temp;
                maxSizeX = mainFile.read();
                maxSizeY = mainFile.read();

                maxFrameSize = maxSizeX*maxSizeY/8;

                uint8_t calculatedFrames = maxFrames;

                while(calculatedFrames > 0){

                    void* pVoid = (realloc(maxData, maxFrameSize * calculatedFrames));
                    if(pVoid != nullptr)
                    {
                        maxData = static_cast<uint8_t *>(pVoid);
                        break;
                    }
                    else{
                        calculatedFrames--;
                    }
                }
                if(calculatedFrames == 0)
                {
                    ESP.restart();
                }
                mainFile.read(maxData, calculatedFrames * maxFrameSize);
                mainFile.seek(mainFile.position() + (maxFrames - calculatedFrames) * maxFrameSize);
                maxFrames = calculatedFrames;
            }
            else if(type == WS_DATA)
            {
                Serial.println("WS_DATA");
                uint8_t temp = mainFile.read();
                wsFrames = temp;
                wsSizeX = mainFile.read();
                wsSizeY = mainFile.read();

                wsFrameSize = wsSizeX * wsSizeY * 3;

                uint8_t calculatedFrames = wsFrames;

                while(calculatedFrames > 0){

                    void* pVoid = (realloc(wsData, wsFrameSize * calculatedFrames));
                    if(pVoid != nullptr)
                    {
                        wsData = static_cast<uint8_t *>(pVoid);
                        break;
                    }
                    else{
                        calculatedFrames--;
                    }
                }
                if(calculatedFrames == 0)
                {
                    ESP.restart();
                }
                mainFile.read(wsData, calculatedFrames * wsFrameSize);
                mainFile.seek(mainFile.position() + (wsFrames - calculatedFrames) * wsFrameSize);
                wsFrames = calculatedFrames;
            }
            else if(type == PX_IN_FILE_DATA)
            {
                Serial.println("PX_IN_FILE");
                pxMode = PX_IN_FILE_DATA;
                uint8_t temp = mainFile.read();
                pxFrames = temp;
                pxSizeX = mainFile.read();
                pxSizeY = mainFile.read();

                pxFrameSize = pxSizeX * pxSizeY * 3 / 8;

                uint8_t calculatedFrames = pxFrames;

                while(calculatedFrames > 0){

                    void* pVoid = (realloc(pxData, pxFrameSize * calculatedFrames));
                    if(pVoid != nullptr)
                    {
                        pxData = static_cast<uint8_t *>(pVoid);
                        break;
                    }
                    else{
                        calculatedFrames--;
                    }
                }
                if(calculatedFrames == 0)
                {
                    ESP.restart();
                }
                mainFile.read(pxData, calculatedFrames * pxFrameSize);
                mainFile.seek(mainFile.position() + (pxFrames - calculatedFrames) * pxFrameSize);
                pxFrames = calculatedFrames;
            }
            else if(type == PX_EXTERNAL_STREAMING)
            {
                Serial.println("PX_EXTERNAL_STREAM");
                //uint8_t len = mainFile.read();

//                    char s[len];
//                    for(int i = 0; i < len; i++){
//                        s[i] = (char)mainFile.read();
//                    }
//                    pxFileName = String(s);
                pxFileName = mainFile.readStringUntil((char)0);
                if(!SD_MMC.exists("/"+pxFileName))
                {
                    Serial.println(pxFileName + " not found!");
                    pxFileName = filename.substring(0, filename.lastIndexOf('.'))+".ser";
                    if(!SD_MMC.exists("/"+pxFileName))
                    {
                        Serial.println("ERROR: FILE NOT FOUND!");
                        return;
                    }
                }

                pxFile = SD_MMC.open("/"+pxFileName);

                if(pxFile.available() > 0)
                {
                    pxMode = PX_EXTERNAL_STREAMING;
                    uint8_t r = pxFile.read();

                    if(r != PX_EXTERNAL_STREAMING)
                    {
                        Serial.println("ERROR: THIS IS NOT PX_EXTERNAL_STREAMING!");
                        return;
                    }
                    uint8_t framesBytes[4];
                    pxFile.read(framesBytes, 4);

                    //pxFrames = *((uint32_t *)framesBytes);
                    pxFrames = framesBytes[3] | (framesBytes[2] << 8) | (framesBytes[1] << 16) | (framesBytes[0] << 24);
                    //pxFrames += (((uint32_t)d[0])<<24);
                    //pxFrames += (((uint32_t)d[1])<<16);
                    //pxFrames += (((uint32_t)d[2])<<8);
                    //pxFrames += (((uint32_t)d[3]));

                    pxSizeX = pxFile.read();
                    pxSizeY = pxFile.read();
                    pxFrameSize = pxSizeX * pxSizeY * 3 / 8;

                    pxFileStart = pxFile.position();

                }

            }
        }

    }
}

void LoadedData::seekPXMatrixData(int deltaPosition) {
    if(getPXMatrixMode() == PX_EXTERNAL_STREAMING)
    {
        pxFile.seek(pxFileStart+deltaPosition);
    }
}

void LoadedData::readPxToBuffer(uint8_t *buffer) {
    if(getPXMatrixMode() == PX_EXTERNAL_STREAMING && pxFile.available() > 0)
    {
        pxFile.read(buffer, pxFrameSize);
    }
}


