#include "jsonconfig.h"

#include <FS.h>
#include <SPIFFS.h>


DynamicJsonDocument* configJson = nullptr;


void applyJsonConfig() {
    Serial.println("Applying config");    
    if (configJson->containsKey("bl")) {
        Serial.println("Key bl exists");
        if ((*configJson)["bl"].is<int>()) {
            Serial.println("is int");
            uint8_t pwm = (*configJson)["bl"];
            ttgo->bl->adjust(pwm);
        }
    }
}

void loadJsonConfig() {
    if (configJson==nullptr) {
        configJson = new DynamicJsonDocument(CONFIGJSONSIZE);
    }
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount SPIFFS");        
    } else {
        fs::File file = SPIFFS.open("/config.json", FILE_READ);
        if (!file) {
          Serial.println("There was an error opening the config.json file for reading");
        } else {
            Serial.println("File config.json opened for reading!");
            DeserializationError error = deserializeJson(*configJson, file);
            file.close();
            if (error) {
                Serial.println("error deserializing config.json");
            } else {
                serializeJsonPretty(*configJson, Serial);
            }
        }
    }
    bool saveIt = false;
    if (!configJson->containsKey("tz")) {
        (*configJson)["tz"]="CET-2";
        saveIt=true;
    }
    if (!configJson->containsKey("wifi")) {
        (*configJson)["wifi"] = JsonObject();        
        (*configJson)["wifi"]["last"]="";
        saveIt=true;
    } 
    if (!(*configJson)["wifi"].containsKey("known")) {
        (*configJson)["wifi"]["known"] = JsonObject();
        (*configJson)["wifi"]["known"]["tech-lab"] = "tech-lab";
        (*configJson)["wifi"]["known"]["stopbuepf"] = "stopbuepf";
        saveIt=true;
    }
    if (!(*configJson)["wifi"].containsKey("last")) {
        (*configJson)["wifi"]["last"]="";
        saveIt=true;
    }

    if (!configJson->containsKey("bl")) {
        (*configJson)["bl"] = 20;  // Backlight
        saveIt=true;
    }
    if (saveIt) {
        saveJsonConfig();
    } 
    applyJsonConfig();
}

void saveJsonConfig() {
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount SPIFFS");        
    } else {
        fs::File file = SPIFFS.open("/config.json", FILE_WRITE);
        if (!file) {
          Serial.println("There was an error opening the config.json file for writing");
        } else {
            Serial.println("File config.json opened writing!");
            (*configJson)["hello"] = "world";
            serializeJson(*configJson, file);
            file.close();
            serializeJson(*configJson, Serial);
        }
    }
}
