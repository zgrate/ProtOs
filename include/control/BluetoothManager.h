//
// Created by dzing on 30/01/2021.
//

#ifndef VISORV3_BLUETOOTHMANAGER_H
#define VISORV3_BLUETOOTHMANAGER_H
/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include "ConstantsAndSettings.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <bitset>
#include "map"
#include "control/ControlFunctions.h"

class BluetoothManager {



    BLEServer *pServer = NULL;
    BLECharacteristic *pCharacteristic = NULL;
    bool deviceConnected = false;
    bool oldDeviceConnected = false;
    uint32_t value = 0;

    static string show_binary(unsigned int u, int num_of_bits) {
        string a = "";

        int t = pow(2, num_of_bits);   // t is the max number that can be represented

        for (t; t > 0; t = t / 2)           // t iterates through powers of 2
            if (u >= t) {                // check if u can be represented by current value of t
                u -= t;
                a += "1";               // if so, add a 1
            } else {
                a += "0";               // if not, add a 0
            }

        return a;                     // returns string
    }

    static bool checkPin(uint8_t low, uint8_t high) {
        int pin = high << 8;
        pin |= low;
        Serial.println(pin);
        return pin == BLUETOOTH_CONNECTION_PIN;
    }

    class BluetoothCallback : public BLECharacteristicCallbacks {
    public:
        explicit BluetoothCallback(BluetoothManager* instance)
        {
            bluetoothManagerInstance = instance;
        }
    private:
        BluetoothManager* bluetoothManagerInstance;
        void onRead(BLECharacteristic *chara) override {
            Serial.println("READ");

        }

        void onWrite(BLECharacteristic *chara) override {
            Serial.println("WRITE");

            uint8_t *data = chara->getData();
            if(BluetoothManager::checkPin(data[1], data[0])){
                Serial.println("PIN CORRECT!");
                //Serial.println(show_binary(data[2], 8).c_str());
                //Serial.println(show_binary(data[3], 8).c_str());
                //processData(data[2], data);
            }
            else{
                Serial.println("INCORRECT PIN!");
            }
//            chara->setValue("ASDF");
//            chara->notify();
        }
    };

//AT+BAND807D3AA2B9DA
    class MyServerCallbacks : public BLEServerCallbacks {
        void onConnect(BLEServer *pServer) {
            Serial.println("Connected");
            BLEDevice::startAdvertising();
        };

        void onDisconnect(BLEServer *pServer) {
            Serial.println("Disconnected");
        }
    };


    static void notifyCallback(
            BLERemoteCharacteristic *pBLERemoteCharacteristic,
            uint8_t *pData,
            size_t length,
            bool isNotify) {
        Serial.print("Notify callback for characteristic ");
        Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
        Serial.print(" of data length ");
        Serial.println(length);
        Serial.print("data: ");
        Serial.println(pData[0]);
    }
public:
    void setupBLE() {
        Serial.begin(115200);

        // Create the BLE Device
        BLEDevice::init("BeepBooperLE");

        // Create the BLE Server
        pServer = BLEDevice::createServer();
        pServer->setCallbacks(new MyServerCallbacks());
        // Create the BLE Service
        BLEService *pService = pServer->createService(BLUETOOTH_SERVICE_UUID);

        // Create a BLE Characteristic
        pCharacteristic = pService->createCharacteristic(
                BLUETOOTH_CHARACTERISTIC_UUID,
                BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_WRITE |
                BLECharacteristic::PROPERTY_NOTIFY
        );

        // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
        // Create a BLE Descriptor
        pCharacteristic->addDescriptor(new BLE2902());
        pCharacteristic->setCallbacks(new BluetoothCallback(this));

        // Start the service
        pService->start();

        // Start advertising
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(BLUETOOTH_SERVICE_UUID);
        pAdvertising->setScanResponse(false);
        //pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
        BLEDevice::startAdvertising();
        Serial.println("Waiting a client connection to notify...");
    }

    void loopBLE() {
        // notify changed value
//        if (deviceConnected) {
//            //Serial.println(pCharacteristic->getValue());
////        pCharacteristic->setValue("TEST");
////        pCharacteristic->notify();
////        value++;
//            delay(1000); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
//        }
//        // disconnecting
//        if (!deviceConnected && oldDeviceConnected) {
//            delay(500); // give the bluetooth stack the chance to get things ready
//            pServer->startAdvertising(); // restart advertising
//            Serial.println("start advertising");
//            oldDeviceConnected = deviceConnected;
//        }
//        // connecting
//        if (deviceConnected && !oldDeviceConnected) {
//            // do stuff here on connecting
//            oldDeviceConnected = deviceConnected;
//        }
    }
};

//TODO: Only on global instance
#ifdef BLUETOOTH_GLOBAL_INSTANCE
    BluetoothManager BluetoothClientInstance = BluetoothManager();
#endif

#endif //VISORV3_BLUETOOTHMANAGER_H
