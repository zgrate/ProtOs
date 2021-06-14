//
// Created by dzing on 06/05/2021.
//

#ifndef VISORV3_PACKETS_H
#define VISORV3_PACKETS_H


#include <map>
#include <memory>
#include "Arduino.h"
#include "FS.h"
#include <vector>
#include "control/Definitions.h"
#include "ConstantsAndSettings.h"

using namespace std;


uint16_t readShort(Stream &s);

uint32_t readULong(Stream &s);

uint64_t readULongLong(Stream &s);

String readString(Stream &s);

//TODO: I DONT NEED FROM HERE, THIS IS C++, YOU CAN JUST INCREMENT STUFF HERE
uint16_t readShort(const uint8_t *array, int from = 0);

uint32_t readULong(const uint8_t *array, int from = 0);

uint64_t readULongLong(const uint8_t *array, int from = 0);

String readString(const uint8_t *array, int from, int *length);

void writeShort(const uint16_t &number, uint8_t *bytes, const uint16_t &from = 0);

void writeULong(const uint32_t &number, uint8_t *bytes, const uint16_t &from = 0);

void writeULongLong(const uint64_t &number, uint8_t *bytes, const uint16_t &from = 0);

void writeString(const String &string, uint8_t *bytes, const uint16_t &from = 0);

/**
 * Pipeline, shows the source of the packet, used for later forwarding the packet to correct target
 */
enum class PacketPipeline {
    WIFI, BLUETOOTH, SERIAL_SOURCE, OTHER, UNKNOWN
};

/**
 * Caluclates CRC8 of the given buffer
 * @param addr Address of the buffer
 * @param len Length of the buffer
 * @return 8-bit crc8 chechsum
 */
uint8_t crc8(const uint8_t *addr, uint16_t len);

/**
 * Basepacket, used as a basis for other packets
 */
class BasePacket {
protected:
    uint8_t packetId;
    PacketPipeline pipeline;


public:
    BasePacket(uint8_t packetId, PacketPipeline pipeline) {
        this->packetId = packetId;
        this->pipeline = pipeline;
    }

    PacketPipeline getPipeline() const {
        return pipeline;
    }

    uint8_t getPacketId() const {
        return packetId;
    }
};

/**
 * Packet with a client as a target (ProtApp is a target)
 */
class ClientBoundPacket : public BasePacket {

public:
    /**
     * Generates the bytes from the packet
     * @param length Pointer to the variable where length will be saved
     * @return Pointer to the buffer. It should be deleted after usage, as it is dynamically allocated
     */
    virtual uint8_t *getPacket(uint16_t *length) = 0;

    /**
     * Constructor for the packet
     * @param i ID of the packet
     * @param pipeline Pipeline of the packet
     */
    explicit ClientBoundPacket(int i, PacketPipeline pipeline) : BasePacket(i, pipeline) {};
};

/**
 * Packet with a server as a target (ProtOs is a target)
 */
class ServerBoundPacket : public BasePacket {

public:
    /**
     * Constructor for the packet
     * @param i ID of the packet
     * @param pipeline Pipeline of the packet
     */
    explicit ServerBoundPacket(int i, PacketPipeline pipeline) : BasePacket(i, pipeline) {};

    /**
     * Reads the packet from the given buffer and length and constucts the packet
     * @param buffer Buffer
     * @param length Length of the buffer
     */
    virtual void readPacket(const uint8_t *buffer, const uint16_t &length) = 0;
};


/**
 * SERVER BOUND PACKETS
 *
 */

class S03Handshake : public ServerBoundPacket {

private:
    uint8_t appVersion = 0;

public:
    uint8_t getAppVersion() const {
        return appVersion;
    }

    explicit S03Handshake(PacketPipeline pipeline) : ServerBoundPacket(3, pipeline) {};

    void readPacket(const uint8_t *array, const uint16_t &length) override {
        appVersion = array[0];
    }
};

class S04ChangeAnimation : public ServerBoundPacket {
};

class S05RequestSensor : public ServerBoundPacket {
    uint8_t sensorId = 0;

public :
    explicit S05RequestSensor(PacketPipeline pipeline) : ServerBoundPacket(5, pipeline) {};

    uint8_t getSensorId() const {
        return sensorId;
    }

    void readPacket(const uint8_t *array, const uint16_t &length) override {
        sensorId = array[0];
    }

};

class S06TimeUpdate : public ServerBoundPacket {
};

class S07ChangeSystemProperty : public ServerBoundPacket {
};

class S08ChangeDrawingMode : public ServerBoundPacket {
};

class S09DrawUpdate : public ServerBoundPacket {

    vector<Pixel> pixels;
    uint8_t screenId = 0;
    uint8_t mode = 0;

public:

    uint8_t getMode() const {
        return mode;
    }

    void readPacket(const uint8_t *array, const uint16_t &length) override {
        screenId = array[0];
        mode = array[1];
        uint16_t numberOfPixels = ((uint16_t) array[3] << 8) | array[2];

        for (int i = 0; i < numberOfPixels; i++) {
            int offset = 4 + i * 7;
            uint16_t x = ((uint16_t) array[offset + 1] << 8) | array[offset + 0];
            uint16_t y = ((uint16_t) array[offset + 3] << 8) | array[offset + 2];
            Pixel newPixel = Pixel(x, y, array[offset + 4], array[offset + 5], array[offset + 6]);
            pixels.push_back(newPixel);
        }

    }

    const vector<Pixel> &getPixels() const {
        return pixels;
    }

    uint8_t getScreenId() const {
        return screenId;
    }

    explicit S09DrawUpdate(PacketPipeline pipeline) : ServerBoundPacket(9, pipeline) {}
};

class S0ARequestShutdown : public ServerBoundPacket {
};

class S0BRegisterEvent : public ServerBoundPacket {
};

class S0CFileOperation : public ServerBoundPacket {
    uint8_t requestId = 0;
    FileOperationType operationType = FileOperationType::UNKNOWN_OP;
    String workingFile = "";
    String parameters = "";

public:

    explicit S0CFileOperation(PacketPipeline pipeline) : ServerBoundPacket(12, pipeline) {}

    void readPacket(const uint8_t *array, const uint16_t &length) override {
        debugPrint("Reading Packet...");
        requestId = array[0];
        debugPrint(requestId);
        operationType = (FileOperationType) array[1]; //TODO: Exception
        debugPrint(operationType);
        int wFileLength = 0;
        workingFile = readString(array, 2, &wFileLength);
        debugPrint(workingFile);
        uint16_t index = 4 + wFileLength;
        debugPrint(index);
        int len = 0;
        parameters = readString(array, index, &len);
        debugPrint(parameters);
    }

    uint8_t getRequestId() const {
        return requestId;
    }

    FileOperationType getOperationType() const {
        return operationType;
    }

    const String &getWorkingFile() const {
        return workingFile;
    }

    const String &getParameters() const {
        return parameters;
    }
};

class S0DFileUpload : public ServerBoundPacket {

    uint8_t requestId = 0;
    String targetFile = "";
    uint16_t length = 0;
    uint8_t *buffer = nullptr;

public:
    explicit S0DFileUpload(PacketPipeline pipeline) : ServerBoundPacket(13, pipeline) {};

    uint8_t getRequestId() const {
        return requestId;
    }

    const String &getTargetFile() const {
        return targetFile;
    }

    uint16_t getLength() const {
        return length;
    }

    uint8_t *getBuffer() const {
        return buffer;
    }

    void readPacket(const uint8_t *array, const uint16_t &length) override {
        requestId = array[0];
        int len = 0;
        targetFile = readString(array, 1, &len);
        debugPrint(len);
        this->length = length - len - 3;
        this->buffer = new uint8_t[this->length];
        for (int i = 0; i < this->length; i++) {
            this->buffer[i] = array[i + len + 3];
        }
    }

    ~S0DFileUpload() {
        delete[] buffer;
        buffer = nullptr;
    }
};

class S0EControlSet : public ServerBoundPacket {

    uint8_t controlId = 0;
    String controlValue = "";


public :

    explicit S0EControlSet(PacketPipeline pipeline) : ServerBoundPacket(14, pipeline) {};

    uint8_t getControlId() const {
        return controlId;
    }

    const String &getControlValue() const {
        return controlValue;
    }

    void readPacket(const uint8_t *array, const uint16_t &length) override {
        controlId = array[0];
        int len = 0;
        controlValue = readString(array, 1, &len);
    }

};


/**
 * CLIENT BOUND PACKETS
 */
class C03Handshake : public ClientBoundPacket {

    String toSend;

public:

    void setToSend(const String &s) {
        this->toSend = s;
    }

    explicit C03Handshake(PacketPipeline pipeline) : ClientBoundPacket(3, pipeline) {};

    uint8_t *getPacket(uint16_t *length) override {
        if (toSend.length() == 0) {
            *length = 0;
            return nullptr;
        }
        auto *packet = new uint8_t[toSend.length()];
        for (auto i = 0; i < toSend.length(); i++) {
            packet[i] = toSend[i];
        }
        *length = (toSend.length());
        return packet;
    }
};

class C04LogSensor : public ClientBoundPacket {

    uint8_t sensorId = 0;
    uint8_t logSource = 0;
    SensorType type = SensorType::UNKNOWN_SENSOR;
    String response = "";


public:
    explicit C04LogSensor(PacketPipeline pipeline) : ClientBoundPacket(4, pipeline) {};

    void setSensorId(const uint8_t &sensorIden) {
        sensorId = sensorIden;
    }

    void setLogSource(const uint8_t &source) {
        logSource = source;
    }

    void setType(SensorType sensorType) {
        type = sensorType;
    }

    void setResponse(const String &responseS) {
        response = responseS;
    }

    uint8_t *getPacket(uint16_t *length) override {
        *length = 3 + 2 + response.length();
        auto *array = new uint8_t[*length];
        array[0] = sensorId;
        array[1] = logSource;
        array[2] = (uint8_t) type;
        writeString(response, array, 3);
        return array;
    }


};

class C05FileResponse : public ClientBoundPacket {
    uint8_t request_id = 0;
    FileResponseType response_type = UNKNOWN_RESPONSE;
    String response = "";

public:
    void setRequestId(uint8_t requestId) {
        request_id = requestId;
    }

    void setResponseType(FileResponseType responseType) {
        response_type = responseType;
    }

    void setResponse(const String &res) {
        response = res;
    }

    explicit C05FileResponse(PacketPipeline pipeline) : ClientBoundPacket(5, pipeline) {};

    uint8_t *getPacket(uint16_t *length) override {
        *length = 4 + response.length();
        auto *buffer = new uint8_t[*length];
        buffer[0] = request_id;
        buffer[1] = (uint8_t) response_type;
        writeString(response, buffer, 2);
        return buffer;
    }
};

class C06FileDownload : public ClientBoundPacket {
//    uint8_t request_id = 0;
//    String filename = "";
    uint16_t length = 0;
    uint8_t *buffer{};


public:
    explicit C06FileDownload(PacketPipeline pipeline) : ClientBoundPacket(6, pipeline) {};

    void readFile(fs::FS &filesystem, const String &fileToSend, const uint8_t &request_id) {
        if (!filesystem.exists(fileToSend)) {
            return;
        }
        auto file = filesystem.open(fileToSend);
        if (file.isDirectory()) {
            return;
        }
        length = file.size();
        String filename = file.name();
        buffer = new byte[1 + 2 + filename.length() + length];
        buffer[0] = request_id;
        writeString(filename, buffer, 1);
        file.read(buffer + 3 + filename.length(), length);
        length = 1 + 2 + filename.length() + length;
    }

    uint8_t *getPacket(uint16_t *len) override {
        *len = length;
        return buffer;
    }


};

/**
 * Method used to contruct the packet from the steam of data
 * @param s Stream of data to read from
 * @param pipeline Pipeline the packet is currently in
 * @return the pointer to the constructed and read packet
 */
std::shared_ptr<ServerBoundPacket> constructPacket(Stream &s, PacketPipeline pipeline);


#endif //VISORV3_PACKETS_H
