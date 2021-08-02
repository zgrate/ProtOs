//
// Created by dzing on 03/02/2021.
//

#include "Arduino.h"
#include "control/ControlFunctions.h"
#include "MainSystem.h"


using namespace std;

void processIncomingS09(const shared_ptr<S09DrawUpdate> &pointer) {
    //Serial.println("Drawing on the screen!");
    if (pointer->getMode() == 0)
        main_ns::liveDrawUpdate(pointer->getScreenId(), pointer->getPixels());
    else
        main_ns::clearDisplay(pointer->getScreenId());
}

void processIncomingS03(const shared_ptr<S03Handshake> &pointer) {
    Serial.print("Got the app version ");
    Serial.println(pointer->getAppVersion());
    vTaskDelay(1000);
    auto response = shared_ptr<C03Handshake>(new C03Handshake(pointer->getPipeline()));
    response->setToSend("{'data':'TEST_DATA_TO_SEND'}");
    main_ns::forwardPacket(response);
}


void processIncomingPacket(const std::shared_ptr<ServerBoundPacket> &packet) {
    if (packet->getPacketId() == 3) {
        processIncomingS03(std::static_pointer_cast<S03Handshake>(packet));
    } else if (packet->getPacketId() == 5) {
        processIncomingS05(std::static_pointer_cast<S05RequestSensor>(packet));
    } else if (packet->getPacketId() == 9) {
        processIncomingS09(std::static_pointer_cast<S09DrawUpdate>(packet));
    } else if (packet->getPacketId() == 12) {
        processIncomingS0C(std::static_pointer_cast<S0CFileOperation>(packet));
    } else if (packet->getPacketId() == 13) {
        processIncomingS0D(std::static_pointer_cast<S0DFileUpload>(packet));
    } else if (packet->getPacketId() == 14) {
        processIncomingS0E(std::static_pointer_cast<S0EControlSet>(packet));
    } else if (packet->getPacketId() == 15) {
        processIncomingS0F(std::static_pointer_cast<S0FExternalControlData>(packet));
    }
}

String listDir(const String &dir) {
    Serial.println("LISTING DIR");
    auto root = SD_MMC.open(dir);
    if (!root.isDirectory()) {
        return "E;not_directory";
    }
    String list;
    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            list = list + "D:" + file.name();
        } else {
            list = list + "F:" + String((int) file.size(), DEC) + ":" + file.name();
        }
        list += "?";
        file = root.openNextFile();
    }
    return list;
}

std::shared_ptr<ClientBoundPacket>
download(const String &file, const PacketPipeline &pipeline, const uint8_t &requestId) {
    auto packet = std::shared_ptr<C06FileDownload>(new C06FileDownload(pipeline));
    packet->readFile(SD_MMC, file, requestId);
    return packet;
}

uint8_t isFile(const String &location) {
    if (!SD_MMC.exists(location))
        return -1;
    File f = SD_MMC.open(location);
    bool t = (f.isDirectory());
    f.close();
    return t ? 1 : 0;


}

void processIncomingS0C(const shared_ptr<S0CFileOperation> &pointer) {
    Serial.println("PROCESSING INCOMING S0C");
    auto response = shared_ptr<C05FileResponse>(new C05FileResponse(pointer->getPipeline()));
    response->setResponseType(OK_RES);
    response->setRequestId(pointer->getRequestId());
    switch (pointer->getOperationType()) {
        case UNKNOWN_OP:
            response->setResponseType(UNKNOWN_RESPONSE);
            break;
        case LS:
            response->setResponse(listDir(pointer->getWorkingFile()));
            break;
        case MKDIR:
            response->setResponse(String(SD_MMC.mkdir(pointer->getWorkingFile())));
            break;
        case TOUCH:
            break;
        case DEL:
            switch (isFile(pointer->getWorkingFile())) {
                case 0:
                    response->setResponse(String(SD_MMC.remove(pointer->getWorkingFile())));
                    break;
                case 1:
                    response->setResponse(String(SD_MMC.rmdir(pointer->getWorkingFile())));
                    break;
            }
            break;
        case MV:
            response->setResponseType(INVALID_RES);
            break;
        case CP:
            response->setResponseType(INVALID_RES);
            break;
        case DOWN:
            main_ns::forwardPacket(
                    download(pointer->getWorkingFile(), pointer->getPipeline(), pointer->getRequestId()));
            return;


    }
    main_ns::forwardPacket(response);

}

void processIncomingS0D(const shared_ptr<S0DFileUpload> &pointer) {
    int result = (isFile(pointer->getTargetFile()));
    if (result == 1) {
        auto p = shared_ptr<C05FileResponse>(new C05FileResponse(pointer->getPipeline()));
        p->setRequestId(pointer->getRequestId());
        p->setResponseType(FileResponseType::INVALID_RES);
        p->setResponse("");
        main_ns::forwardPacket(p);
        return;
    } else if (result == 0) {
        SD_MMC.remove(pointer->getTargetFile());
    }
    auto f = SD_MMC.open(pointer->getTargetFile(), "w");
    f.write(pointer->getBuffer(), pointer->getLength());
    auto p = shared_ptr<C05FileResponse>(new C05FileResponse(pointer->getPipeline()));
    p->setRequestId(pointer->getRequestId());
    p->setResponseType(FileResponseType::OK_RES);
    p->setResponse("");
    main_ns::forwardPacket(p);
}

void processIncomingS05(const shared_ptr<S05RequestSensor> &pointer) {
    Serial.println("Processing the sensor...");
    uint8_t type = 0;
    String response = main_ns::readSensor(pointer->getSensorId(), &type);
    auto sensor = shared_ptr<C04LogSensor>(new C04LogSensor(pointer->getPipeline()));
    sensor->setResponse(response);
    sensor->setLogSource(0);
    sensor->setSensorId(pointer->getSensorId());
    sensor->setType((SensorType) type);
    main_ns::forwardPacket(sensor);
}


void processIncomingS0E(const shared_ptr<S0EControlSet> &pointer) {
    main_ns::control(pointer->getControlId(), pointer->getControlValue());
}

void processIncomingS0F(const shared_ptr<S0FExternalControlData> &pointer) {
    Serial.println("Playing animation with number " + String(pointer->getNumberToPlay()));
    //TODO
}
