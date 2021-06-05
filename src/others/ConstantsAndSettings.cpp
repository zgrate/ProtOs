#include "ConstantsAndSettings.h"
#include "ArduinoJson.h"
#include <SD_MMC.h>

#define JSON_DOC_CAPA 1024

String wifiSsid = "ZRoom";
String wifiPassword = "OculusQuest2";
String wifiHostname = "ZVIS";
String ipAddress = "";
bool isRunning = true;
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

std::vector<CapabilitiesEntry> capabilities;

void constructCapabilitiesList() {

#ifdef PX_MATRIX_SCREEN
    auto px = CapabilitiesEntry(PX_MATRIX_SCREEN, CapabilitiesType::SCREEN_CAPA, ScreenType::PXMATRIX_SCREEN);
    px.addDetail("sizeX", PxMATRIX_WIDTH);
    px.addDetail("sizeY", PxMATRIX_HEIGHT);
    px.addDetail("color_depth", PxMATRIX_COLOR_DEPTH);
    capabilities.push_back(px);
#ifdef PX_BRIGHTNESS_CONTROL
    auto pxBrightness = CapabilitiesEntry(PX_BRIGHTNESS_CONTROL, CapabilitiesType::CONTROL_CAPA,
                                          ControlType::PX_BRIGHTNESS);
    capabilities.push_back(pxBrightness);
#endif
#endif
#ifdef MAX_MATRIX_SCREEN
    auto max = CapabilitiesEntry(MAX_MATRIX_SCREEN, CapabilitiesType::SCREEN_CAPA, ScreenType::MAX_SCREEN);
    max.addDetail("bulk_mode", MAX_BULK_MODE);
    max.addDetail("matrices_number", MAX_MATRICES_NUMBER);
    max.addDetail("sizeX", MAX_WIDTH);
    max.addDetail("sizeY", MAX_HEIGHT);
    capabilities.push_back(max);
#ifdef MAX_BRIGTHNESS_CONTROL
    auto maxBrightness = CapabilitiesEntry(MAX_BRIGTHNESS_CONTROL, CapabilitiesType::CONTROL_CAPA,
                                           ControlType::MAX_BRIGHTNESS_CONTROL);
    capabilities.push_back(maxBrightness);
#endif
#endif
#ifdef FAN_QC_SUPPORT
    auto fan = CapabilitiesEntry(FAN_QC_SUPPORT, CapabilitiesType::CONTROL_CAPA, ControlType::FAN_CONTROL);
    fan.addDetail("shutdown_support", 0);
    capabilities.push_back(fan);
#endif
#ifdef OLED_SUPPORT
    auto oled = CapabilitiesEntry(OLED_SUPPORT, CapabilitiesType::CONTROL_CAPA, ControlType::OLED_CONTROL);
    oled.addDetail("lines", OLED_LINES);
    oled.addDetail("length", OLED_MAX_LENGTH);
    capabilities.push_back(oled);
#endif
#ifdef THERMOMETER_HYDROMETER_SENSOR
    auto thermoHydro = CapabilitiesEntry(THERMOMETER_HYDROMETER_SENSOR, CapabilitiesType::SENSOR_CAPA,
                                         SensorType::TEMPERATURE_HUMIDITY);
    capabilities.push_back(thermoHydro);
#endif
#ifdef CURRENT_VOLTAGE_SENSOR
    auto currentVol = CapabilitiesEntry(CURRENT_VOLTAGE_SENSOR, CapabilitiesType::SENSOR_CAPA,
                                        SensorType::CURRENT_VOLTAGE);
    capabilities.push_back(currentVol);
#endif
#ifdef SD_SUPPORT
    auto sdCard = CapabilitiesEntry(0, CapabilitiesType::SD_CARD_CAPA, 0);
    sdCard.addDetail("size", SD_MMC.cardSize());
    capabilities.push_back(sdCard);
#endif
#ifdef TIME_SUPPORT
    auto time = CapabilitiesEntry(0, CapabilitiesType::TIME_CAPA, 0);
    capabilities.push_back(time);
#endif
#ifdef WIFI_SUPPORT
    auto wifi = CapabilitiesEntry(0, CapabilitiesType::WIFI_CAPA, 0);
    capabilities.push_back(wifi);
#endif
#ifdef BLUETOOTH_SUPPORT
    auto bt = CapabilitiesEntry(0, CapabilitiesType::BT_CAPA, 0);
    capabilities.push_back(bt);
#endif
}

SensorType getSensorType(const uint8_t &id) {
    for (const auto &e : capabilities) {
        if (e.type == CapabilitiesType::SENSOR_CAPA && e.id == id) {
            return (SensorType) e.detailType;
        }
    }
    return SensorType::UNKNOWN_SENSOR;
}

ControlType getControlType(const uint8_t &id) {
    for (const auto &e : capabilities) {
        if (e.type == CapabilitiesType::CONTROL_CAPA && e.id == id) {
            return (ControlType) e.detailType;
        }
    }
    return ControlType::UNKNOWN_CONTROL;
}

ScreenType getScreenType(const uint8_t &id) {
    for (const auto &e : capabilities) {
        if (e.type == CapabilitiesType::SCREEN_CAPA && e.id == id) {
            return (ScreenType) e.detailType;
        }
    }
    return ScreenType::UNKNOWN_SCREEN;
}


String getCapabilitiesJson() {
    DynamicJsonDocument doc(10000);
    for (auto e : capabilities) {
        doc.add(e.toJson());
    }
    serializeJson(doc, Serial);
    return "";
}


DynamicJsonDocument getDocumentToSave() {
    DynamicJsonDocument doc(JSON_DOC_CAPA);
    auto obj = doc.createNestedObject("network_connection");
    obj["ssid"] = wifiSsid;
    obj["pass"] = wifiPassword;
    obj["port"] = WIFI_PORT;
    obj["hostname"] = wifiHostname;
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
        wifiSsid = ssid;
        const char *password = doc["network_connection"]["pass"];
        wifiPassword = password;
        WIFI_PORT = doc["network_connection"]["port"];
        const char *hostname = doc["network_connection"]["hostname"];
        wifiHostname = hostname;
    }


}