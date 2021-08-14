#include "ota.h"

#include "ArduinoOTA.h"
#include <SPIFFS.h>
#include "jsonconfig.h"
#include "loop.h"

void WatchOTA::loop() {
    if (type == ARDUINO_SERVER) {
        loopArduinoOTA();
    }
}

bool WatchOTA::start(otaType _type, status_cb_t _status_cb) {
    type = _type;
    status_cb = _status_cb;
    if (type==ARDUINO_SERVER) {
        startArduinoOTA();
    }
}
bool WatchOTA::stop() {
    if (type==ARDUINO_SERVER) {
        stopArduinoOTA();
    }
}


void WatchOTA::setupArduinoOTA() {
    ArduinoOTA.setPort(3232);

    ArduinoOTA
        .onStart([this]() {
            register_exclusive_loop(ota_loop);
            state = UPDATING;
            if (ArduinoOTA.getCommand() == U_FLASH) {
                status_cb("OTA sketch Update");
                Serial.println("OTA sketch Update");
            } else {// U_SPIFFS
                status_cb("OTA SPIFFS Update");
                Serial.println("OTA SPIFFS Update");
                SPIFFS.end();
            }
        })
        .onEnd([this]() {
            status_cb("OTA End");
            Serial.println("OTA End");
            release_exclusive_loop(ota_loop);
            state = DONE;
        })
        .onProgress([this](unsigned int progress, unsigned int total) {
            static unsigned int percentage = -1;
            unsigned int p = (progress / (total / 100));
            if (percentage != p) {
                percentage = p;
                char buffer[30];
                Serial.printf("Progress: %u%%\r", p);
                sprintf(buffer, "%u%%", p);
                status_cb(buffer);
            }
        })
        .onError([](ota_error_t error) {
            
            if (error == OTA_AUTH_ERROR) { Serial.println("Auth Failed"); }
            else if (error == OTA_BEGIN_ERROR) { Serial.println("Begin Failed"); }
            else if (error == OTA_CONNECT_ERROR) { Serial.println("Connect Failed"); }
            else if (error == OTA_RECEIVE_ERROR) { Serial.println("Receive Failed"); }
            else if (error == OTA_END_ERROR) { Serial.println("End Failed"); }
            release_exclusive_loop(ota_loop);
        });
    setupDone = true;
}

bool WatchOTA::startArduinoOTA() {
    if (!WiFi.isConnected()) return false;
    if (!setupDone) {
        setupArduinoOTA();
    }
    ArduinoOTA.setHostname((*configJson)["hostname"]);
    ArduinoOTA.setMdnsEnabled(true);
    ArduinoOTA.begin();
    state = WAITING;
    Serial.println("Arduino OTA up!");
    status_cb("Arduino OTA up!");
    return true;
}  

void WatchOTA::stopArduinoOTA() {
    ArduinoOTA.end();
    release_exclusive_loop(ota_loop);
    Serial.println("Arduino OTA stop!");
    status_cb("Arduino OTA stop!");
}  

void WatchOTA::loopArduinoOTA() {
    //if (!WiFi.isConnected()) return;
    ArduinoOTA.handle();
}

WatchOTA* WatchOTA::self = nullptr;