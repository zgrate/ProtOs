//
// Created by dzing on 11/05/2021.
//

#include "control/Packets.h"


uint8_t crc8(const uint8_t *addr, uint16_t len) {
    Serial.println("CRC8_Read");
    uint8_t crc = 0;
    for (uint16_t i = 0; i < len; i++) {
        uint8_t inbyte = addr[i];
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix)
                crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    Serial.println("GotCRC8");

    return crc;
}

uint16_t readShort(Stream &s) {
    uint8_t temp[2];
    s.readBytes(temp, 2);
    return ((uint16_t) temp[1] << 8) | temp[0];
}

uint8_t bufferArray[65500];

# pragma message "WE ARE IMPORTING THIS"

std::shared_ptr<ServerBoundPacket> constructPacket(Stream &s, PacketPipeline pipeline) {
    Serial.println("Got a package! Reading it....");
    uint8_t id = s.read();
    uint16_t length = readShort(s);
    //uint8_t bufferArray[length];
    vTaskDelay(1);
    uint16_t readBytes = 0;

    if (length < 512) {
        readBytes = s.readBytes(bufferArray, length);
        vTaskDelay(10);
    } else {
        while (readBytes < length) {
            readBytes += s.readBytes(bufferArray + readBytes, 255);
            vTaskDelay(10);
        }
        //TODO
    }
    Serial.println("Read the packet fully!");
    uint8_t crc8Received;
    Serial.print(readBytes);
    Serial.print(" len ");
    Serial.println(length);
    if (readBytes > length) {

        crc8Received = bufferArray[readBytes - 1];
    } else {
        crc8Received = s.read();
    }

    Serial.println(crc8Received);
    uint8_t fin = crc8(bufferArray, length);
    if (crc8Received != fin) {
        Serial.print("CRC8 ERROR: GOT: ");
        Serial.print(crc8Received);
        Serial.print(" Calculated: ");
        Serial.println(fin);
        return nullptr;
    }


    std::shared_ptr<ServerBoundPacket> Packet;

    switch (id) {
        case 3:
            Packet = std::shared_ptr<ServerBoundPacket>(new S03Handshake(pipeline));
            break;
        case 5:
            Packet = std::shared_ptr<ServerBoundPacket>(new S05RequestSensor(pipeline));
            break;

        case 9:
            Packet = std::shared_ptr<ServerBoundPacket>(new S09DrawUpdate(pipeline));
            //Packet = static_pointer_cast<shared_ptr<BasePacket>>(packet);
            break;
        case 12:
            Packet = std::shared_ptr<ServerBoundPacket>(new S0CFileOperation(pipeline));
            break;
        case 13:
            Packet = std::shared_ptr<ServerBoundPacket>(new S0DFileUpload(pipeline));
            break;
        case 14:
            Packet = std::shared_ptr<ServerBoundPacket>(new S0EControlSet(pipeline));
            break;
        default:
            //ERROR! Packet not found!
            Serial.print("Packet id unknown! ID: ");
            Serial.println(id);
            return nullptr;
    }
    Serial.print("Got a packet ");
    Serial.print(id);
    Serial.print(" with len ");
    Serial.println(length);
    Packet->readPacket(bufferArray, length);
    return Packet;


}

uint32_t readULong(Stream &s) {
    uint8_t temp[4];
    s.readBytes(temp, 4);
    return ((uint32_t) temp[3] << 24) | ((uint32_t) temp[2] << 16) | ((uint32_t) temp[1] << 8) | temp[0];
}

uint64_t readULongLong(Stream &s) {
    uint8_t temp[8];
    s.readBytes(temp, 8);
    return ((uint64_t) temp[7] << 52) | ((uint64_t) temp[6] << 48) | ((uint64_t) temp[5] << 40) |
           ((uint64_t) temp[4] << 32) | ((uint64_t) temp[3] << 24) | ((uint64_t) temp[2] << 16) |
           ((uint64_t) temp[1] << 8) | temp[0];
}

String readString(Stream &s) {
    uint16_t length = readShort(s);
    char *buffer[length];
    s.readBytes(reinterpret_cast<char *>(buffer), length); //TODO - Check
    return String(reinterpret_cast<const char *>(buffer)); //TODO - WTF CHECK
}

void writeShort(const uint16_t &number, uint8_t *bytes, const uint16_t &from) {
    Serial.print("Writing number to buffer ");
    Serial.println(number);
    bytes[from + 0] = (uint8_t) number;
    bytes[from + 1] = (uint8_t) (number >> 8);
}

void writeULongLong(const uint64_t &number, uint8_t *bytes, const uint16_t &from) {
    bytes[from + 0] = number;
    bytes[from + 1] = number << 8;//TODO - FIX NUMBERS
    bytes[from + 2] = number << 16;
    bytes[from + 3] = number << 24;
    bytes[from + 4] = number << 32;
    bytes[from + 5] = number << 40;
    bytes[from + 6] = number << 48;
    bytes[from + 7] = number << 52;
}

void writeULong(const uint32_t &number, uint8_t *bytes, const uint16_t &from) {
    bytes[from + 0] = number;
    bytes[from + 1] = number << 8;
    bytes[from + 2] = number << 16;
    bytes[from + 3] = number << 24;
}

void writeString(const String &string, uint8_t *bytes, const uint16_t &from) {
    writeShort(string.length(), bytes, from);
    for (int i = 0; i < string.length(); i++) {
        bytes[from + 2 + i] = string[i];
    }
}

uint16_t readShort(const uint8_t *array, int from) {
    return ((uint16_t) array[from + 1] << 8) | array[from + 0];
}

String readString(const uint8_t *array, int from, int *length) {
    *length = readShort(array, from);
    Serial.print("reading string of length ");
    Serial.println(*length);
    char c[*length];
    for (int i = 0; i < *length; i++) {
        c[i] = array[from + 2 + i];
    }
    return String(string(c, *length).c_str());
}
