//
// Created by dzing on 06/05/2021.
//

#ifndef VISORV3_WIFIMANAGER_H
#define VISORV3_WIFIMANAGER_H

#include "ConstantsAndSettings.h"

#ifdef WIFI_SUPPORT

#include "WiFi.h"
#include "libraries/OLEDControl.h"

#include "control/Packets.h"
#include "control/ControlFunctions.h"

/**
 * Main wifi loop, used in thread
 * @param wifiManagerPointer WIFI Manager pointer
 */
void wifiLoop(void *wifiManagerPointer);

/**
 * Main class for connecting, processing and sending packets using WIFI
 */
class WifiManager {

    WiFiServer server = WiFiServer(wifiPort);
    TaskHandle_t wifiTask{};
    WiFiClient client;

public:
    /**
     * Main WIFI Loop, should be executed in thread
     */
    void loop() {
        client = server.available();   // listen for incoming clients
        if (client) {                             // if you get a client,
            debugPrint("New Client.");           // print a message out the serial port
            String currentLine = "";                // make a String to hold incoming data from the client
            while (client.connected()) {            // loop while the client's connected
                if (client.available()) {             // if there's bytes to read from the client,
                    //debugPrint("UWU");
                    auto p = constructPacket(client, PacketPipeline::WIFI);
                    if (p != nullptr) {
                        Serial.print(p->getPacketId());
                        processIncomingPacket(p);
                    }
                }
                vTaskDelay(1);
            }
            // close the connection:
            client.stop();
            debugPrint("Client Disconnected.");
        }
    }

    /**
     * Starts WIFI - connects to the network and receivers IP address
     */
    void begin() {
        Serial.print("Connecting to ");
        Serial.println(wifiSsid);
        String s = "WIZZ-";
        s = s + wifiHostname;
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
        WiFi.setHostname(s.c_str());
        WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        server.setNoDelay(true);
        server.begin();
        xTaskCreatePinnedToCore(
                wifiLoop,   /* Task function. */
                "WifiTask",     /* name of task. */
                10000,       /* Stack size of task */
                this,        /* parameter of the task */
                1,           /* priority of the task */
                &wifiTask,      /* Task handle to keep track of created task */
                0);          /* pin task to core 0 */

    }

    /**
     * Sends the packet to the connected client
     * @param packetToSend Packet to send
     * @return
     */
    bool sendPacket(const shared_ptr<ClientBoundPacket> &packetToSend) {
        if (!client.connected()) {
            return false;
        }
        debugPrint("preparing the packet....");
        uint16_t length = 0;
        uint8_t *buffor = packetToSend->getPacket(&length);
        debugPrint("Length of the packet is " + String(length));
        uint8_t crc = crc8(buffor, length);
        debugPrint("CRC is " + String(crc));
        delay(1);
        client.write(packetToSend->getPacketId());
        client.write(length);
        client.write(length >> 8);
        client.write(buffor, length);
        client.write(crc);
        client.flush();
        debugPrint("SEND PACKET!");
        delay(1000);

        delete[] buffor;
        return true;
    }

};



#endif
#endif //VISORV3_WIFIMANAGER_H
