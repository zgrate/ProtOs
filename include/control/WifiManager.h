//
// Created by dzing on 06/05/2021.
//

#ifndef VISORV3_WIFIMANAGER_H
#define VISORV3_WIFIMANAGER_H

#include "ConstantsAndSettings.h"
#include "WiFi.h"
#include "libraries/OLEDControl.h"

#include "control/Packets.h"
#include "control/ControlFunctions.h"

class WifiManager {

    WiFiServer server = WiFiServer(WIFI_PORT);
    WiFiClient client;

public:
    void connect() {
        Serial.print("Connecting to ");
        Serial.println(WIFI_SSID);
        String s = "WIZZ-";
        s = s + WIFI_HOSTNAME;
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
        WiFi.setHostname(s.c_str());
        WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());
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


    }

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
                        //Serial.print("Received packet ");
                        Serial.print(p->getPacketId());
                        //debugPrint("");
                        //debugPrint("End of packet!");
                        processIncomingPacket(p);
                    }


//
//                    String string1 = client.readString();
//
//                    debugPrint(string1);
//                    PxMatrixControlInstance.setBrightness(string1.toInt());


//                    char c = client.read();             // read a byte, then
//                    Serial.write(c);                    // print it out the serial monitor
//                    if (c == '\n') {                    // if the byte is a newline character
//
//                        // if the current line is blank, you got two newline characters in a row.
//                        // that's the end of the client HTTP request, so send a response:
//                        if (currentLine.length() == 0) {
//                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//                            // and a content-type so the client knows what's coming, then a blank line:
//                            client.println("HTTP/1.1 200 OK");
//                            client.println("Content-type:text/html");
//                            client.println();
//
//                            // the content of the HTTP response follows the header:
//                            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
//                            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");
//
//                            // The HTTP response ends with another blank line:
//                            client.println();
//                            // break out of the while loop:
//                            break;
//                        } else {    // if you got a newline, then clear currentLine:
//                            currentLine = "";
//                        }
//                    } else if (c != '\r') {  // if you got anything else but a carriage return character,
//                        currentLine += c;      // add it to the end of the currentLine
//                    }
//
//                    // Check to see if the client request was "GET /H" or "GET /L":
//                    if (currentLine.endsWith("GET /H")) {
//                        OledControlInstance.reset();
//                        OledControlInstance.println("TEST");             // GET /H turns the LED on
//                    }
//                    if (currentLine.endsWith("GET /L")) {
//                        OledControlInstance.reset();
//                        OledControlInstance.println("TOST");                 // GET /L turns the LED off
//                    }
                }
                vTaskDelay(1);
            }
            // close the connection:
            client.stop();
            debugPrint("Client Disconnected.");
        }
    }
};


static WifiManager WifiManagerInstance = WifiManager();
#endif //VISORV3_WIFIMANAGER_H
