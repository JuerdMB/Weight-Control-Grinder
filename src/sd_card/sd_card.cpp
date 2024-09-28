#include "sd_card.h"
#include "config.h"
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "definitions.h"

File logFile;

void initSDCard() {
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD Card initialization failed!");
        // Handle error appropriately
    } else {
        Serial.println("SD Card initialized.");
    }
}

bool logData(const SessionData &data) {
    logFile = SD.open("/grind_log.txt", FILE_APPEND);
    if (logFile) {
        logFile.print("Desired Weight: ");
        logFile.println(data.desiredWeight);
        logFile.print("Measured Weight at Stop: ");
        logFile.println(data.measuredWeightAtStop);
        logFile.print("Final Weight: ");
        logFile.println(data.finalWeight);
        logFile.print("Over-Grind Offset: ");
        logFile.println(data.overGrindOffset);
        logFile.println("-------------------------");
        logFile.close();
        return true;
    } else {
        Serial.println("Failed to open log file");
        return false;
    }
}

bool loadOverGrindOffset(float &ogo) {
    File configFile = SD.open("/config.json", FILE_READ);
    if (configFile) {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, configFile);
        configFile.close();
        if (!error) {
            ogo = doc["overGrindOffset"] | INITIAL_OGO;
            return true;
        } else {
            Serial.println("Failed to parse config file");
            return false;
        }
    } else {
        Serial.println("Config file not found");
        return false;
    }
}

bool saveOverGrindOffset(float ogo) {
    StaticJsonDocument<200> doc;
    doc["overGrindOffset"] = ogo;

    File configFile = SD.open("/config.json", FILE_WRITE);
    if (configFile) {
        serializeJson(doc, configFile);
        configFile.close();
        return true;
    } else {
        Serial.println("Failed to open config file for writing");
        return false;
    }
}