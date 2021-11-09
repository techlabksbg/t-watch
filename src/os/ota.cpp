#include "ota.h"

#include "ArduinoOTA.h"
#include "HttpsOTAUpdate.h"

#include "Update.h"
#include <SPIFFS.h>
#include "jsonconfig.h"
#include "loop.h"
#include "certs/certs.h"

void WatchOTA::loop() {
    if (type == ARDUINO_SERVER) {
        loopArduinoOTA();
    } else if (type == WEB_DOWNLOAD) {
        loopWebOTA();
    }
}

void default_status_cb(const char* s) {
    if (ttgo->tft->cursor_y>220) {
        ttgo->tft->setCursor(0,0);
        ttgo->tft->fillRect(0,0,240,240,0);
    }
    ttgo->tft->println(s);
}

bool WatchOTA::start(otaType _type, status_cb_t _status_cb) {
    if (!WiFi.isConnected()) return false;
    type = _type;
    status_cb = _status_cb == nullptr ? default_status_cb : _status_cb;
    if (type==ARDUINO_SERVER) {
        return startArduinoOTA();
    }
    if (type==WEB_DOWNLOAD) {
        return startWebOTA();
    }
    return false;
}
bool WatchOTA::stop() {
    if (type==ARDUINO_SERVER) {
        stopArduinoOTA();
    } else if (type==WEB_DOWNLOAD) {
        stopWebOTA();
    }
    return true;
}


void WatchOTA::setupArduinoOTA() {
    Serial.println("WatchOTA::setupArduinoOTA() set port");
    ArduinoOTA.setPort(3232);
    Serial.println("WatchOTA::setupArduinoOTA() set callbacks");
    ArduinoOTA
        .onStart([this]() {
            Serial.println("Going exclusive");
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
    Serial.println("WatchOTA::setupArduinoOTA() done");
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
    Serial.println("OTA handle");
    ArduinoOTA.handle();
}

// See https://github.com/husarnet/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/OTAWebUpdater/OTAWebUpdater.ino
// And services/downloadtospiffs.cpp

#include "HTTPClient.h"


#define DOWNLOAD_BUF (512)

typedef struct {
    const char* filename;
    const char* url;
    const char* root_ca; 
    HTTPClient* client=nullptr;
    WiFiClient* stream=nullptr;
    char* buf=nullptr;
    int bytesSent;
    int bytesReceived;
    bool start = true;
} DownloadTaskData;

void downloadTask2(lv_task_t *data) {
    bool error = false;
    DownloadTaskData* d = (DownloadTaskData*) data->user_data;
    if (d->start) {
        d->start = false;
        d->buf = new char[DOWNLOAD_BUF];
        d->client = new HTTPClient;
        Serial.println("OTA from url: Starting web request");
        if (d->url[4]=='s') {
            if (d->root_ca==nullptr) {
                Serial.println("ERROR: Missing root certificate for https!");
                delete d->client;
                error = true;
            } else {
                d->client->begin(d->url, d->root_ca);
            }
        } else { // http
            d->client->begin(d->url);
        }
        if (!error) {
            d->client->GET();
            d->bytesSent = d->client->getSize();
        }
        if (d->bytesSent<0) {
            error = true;
        }
        if (!error) {
            Serial.printf("About to get %d bytes\n", d->bytesSent);
            d->stream = &(d->client->getStream());
            d->bytesReceived = 0;
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
                error = true;
            }
        }
    }
    if (!error && d->bytesReceived < d->bytesSent) {
        if (d->stream->available()>0) {
            auto num = d->stream->readBytes(d->buf,DOWNLOAD_BUF);
            if (Update.write((uint8_t*) d->buf, num) != num) {
                Update.printError(Serial);
                error=true;
            }
            d->bytesReceived+=num;
            Serial.printf("Downloaded %d of %d bytes\n", d->bytesReceived, d->bytesSent);
        }
    }
    // Finished, so clean up
    if (error || d->bytesReceived == d->bytesSent) {
        if (d->client!=nullptr) {
            d->client->end();
            delete d->client;
        }
        delete[] d->buf;
        delete d;
        lv_task_del(data);
    }
}


void HttpEvent(HttpEvent_t *event) {
    switch(event->event_id) {
        case HTTP_EVENT_ERROR:
            Serial.println("Http Event Error");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            Serial.println("Http Event On Connected");
            break;
        case HTTP_EVENT_HEADER_SENT:
            Serial.println("Http Event Header Sent");
            break;
        case HTTP_EVENT_ON_HEADER:
            Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            break;
        case HTTP_EVENT_ON_FINISH:
            Serial.println("Http Event On Finish");
            break;
        case HTTP_EVENT_DISCONNECTED:
            Serial.println("Http Event Disconnected");
            break;
    }
}


bool WatchOTA::startWebOTA() {
    disableCore0WDT();
    HttpsOTA.onHttpEvent(HttpEvent);
    Serial.println("Starting HttpsOTA...");
    HttpsOTA.begin("https://tech-lab.ch/twatch/firmware.bin", ISRG_ROOT_X1);
    Serial.println("   HttpsOTA started...");
    return true;
}

void WatchOTA::loopWebOTA() {
    HttpsOTAStatus_t otastatus = HttpsOTA.status();
    if(otastatus == HTTPS_OTA_SUCCESS) { 
        Serial.println("Firmware written successfully.");
        sleep(5);
        ESP.restart();
    } else if (otastatus == HTTPS_OTA_FAIL) {
        Serial.println("Firmware Upgrade Fail");
    } else if (otastatus == HTTPS_OTA_UPDATING) {
        Serial.print(".");
    }
}
void WatchOTA::stopWebOTA() {
    enableCore0WDT();
}




WatchOTA* WatchOTA::self = nullptr;
WatchOTA* watchOTA = nullptr;