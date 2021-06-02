#include "ConstantsAndSettings.h"
#include "ArduinoJson.h"
#include "SD_MMC.h"

#define JSON_DOC_CAPA 1024

String WIFI_SSID = "ZRoom";
String WIFI_PASSWORD = "OculusQuest2";
String WIFI_HOSTNAME = "ZVIS";
int WIFI_PORT = 48999;

#ifdef SERIAL_DEBUG

void debugPrint(const String &text) {

    Serial.println(text);
}

void debugPrint(const int &text)
{
    Serial.println(text);
}
#else

void debugPrint(const String &text) {

}

void debugPrint(const int &text) {
}

#endif


DynamicJsonDocument getDocumentToSave() {
    DynamicJsonDocument doc(JSON_DOC_CAPA);
    auto obj = doc.createNestedObject("network_connection");
    obj["ssid"] = WIFI_SSID;
    obj["pass"] = WIFI_PASSWORD;
    obj["port"] = WIFI_PORT;
    obj["hostname"] = WIFI_HOSTNAME;
    return doc;
}

void saveConfiguration() {
    auto doc = getDocumentToSave();
    auto file = SD_MMC.open(CONFIGURATION_FILE, "w");
    serializeJson(doc, file);
    file.close();
}


void readConfiguration() {
    if (!SD_MMC.exists(CONFIGURATION_FILE)) {
        saveConfiguration();
        return;
    } else {
        DynamicJsonDocument doc(JSON_DOC_CAPA);
        auto file = SD_MMC.open(CONFIGURATION_FILE, "r");
        deserializeJson(doc, file);
        file.close();

        const char *ssid = doc["network_connection"]["ssid"];
        WIFI_SSID = ssid;
        const char *password = doc["network_connection"]["pass"];
        WIFI_PASSWORD = password;
        WIFI_PORT = doc["network_connection"]["port"];
        const char *hostname = doc["network_connection"]["hostname"];
        WIFI_HOSTNAME = hostname;
    }


}