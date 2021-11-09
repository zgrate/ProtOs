//
// Created by dzing on 01/08/2021.
//

#include "ConstantsAndSettings.h"

#ifdef BLUETOOTH_SUPPORT

#include "control/BluetoothManager.h"
#include "MainSystem.h"

BluetoothManager BluetoothClientInstance = BluetoothManager();

#endif

void BluetoothManager::SetAnimationCallback::onWrite(BLECharacteristic *chara) {
    BLECharacteristicCallbacks::onWrite(chara);
    Serial.println(chara->getValue().c_str());
}

void BluetoothManager::BluetoothCallback::onWrite(BLECharacteristic *chara) {

    Serial.println("WRITE");
    char *data = (char *) chara->getData();
    Serial.println(data[0]);
    string s = string(data);
    Serial.println("READ");
    Serial.println(s.c_str());
    std::string::size_type dots = s.find_first_of(':');
    if (dots != std::string::npos) {
        std::string mode = s.substr(0, dots);
        std::string val = s.substr(dots + 1);
        Serial.println(mode.c_str());
        Serial.println(val.c_str());
        if (mode == "P" or mode == "P") {
            main_ns::control(PX_BRIGHTNESS_CONTROL, String(val.c_str()));
        } else if (mode == "M" or mode == "m") {
            main_ns::control(MAX_BRIGTHNESS_CONTROL, String(val.c_str()));
        } else if (mode == "F" or mode == "f") {
            main_ns::control(FAN_QC_CONTROL, String(val.c_str()));
        } else if (mode == "T") {
            main_ns::load_animation("/test.zgd");
        }
    } else if (std::atoi(s.c_str()) != 0) {
        main_ns::load_animation_from_number(std::atoi(s.c_str()));
    } else {
        main_ns::load_animation(s.c_str());
    }
//            byte *data = chara->getData();
//            auto sh = readShort(data);
//
//            ByteStream stream = ByteStream(data + 2, sh);
//            auto packet = constructPacket(stream, PacketPipeline::BLUETOOTH);
//            if(packet == nullptr)
//                return;
//            processIncomingPacket(packet);

    return;
//            if (BluetoothManager::checkPin(data[1], data[0])) {
//                Serial.println("PIN CORRECT!");
//                //Serial.println(show_binary(data[2], 8).c_str());
//                //Serial.println(show_binary(data[3], 8).c_str());
//                //processData(data[2], data);
//            } else {
//                Serial.println("INCORRECT PIN!");
//            }
//            chara->setValue("ASDF");
//            chara->notify();
}
