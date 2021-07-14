#include "services.h"
#include "../os/os.h"

#define STREAMING_PORT 8080
#define LINES 1

void streamToIPTask(lv_task_t* task) {
    static bool streaming = false;
    static WiFiClient* client = nullptr;
    static uint16_t* buf=nullptr;
    char* host = (char*)task->user_data;
    if (host==nullptr) {
        if (client!=nullptr) {
            client->stop();
            delete client;
            client = nullptr;
        }
        if (buf!=nullptr) {
            delete[] buf;
            buf = nullptr;
        }
        streaming = false;
        return;
    }
    
    if (!streaming) {
        if (WiFi.isConnected()) {
            client = new WiFiClient;
            if (client->connect(host, STREAMING_PORT)) {
                buf = new uint16_t[LINES*240];
                streaming = true;
            } else {
                Serial.printf("Could not connect to host %s on port %d\n", host, STREAMING_PORT);
            }
        } else {
            Serial.println("Not connected to wifi!");
        }
    }
    if (streaming) {
        if (!client->connected()) {
            task->user_data = nullptr; // Clean up in next call
            return;
        }
        client->print("TechLabRocks"); // 12 Bytes marker
        for (int y=0; y<240; y+=LINES) {
            ttgo->tft->readRect(0,y,240,LINES,buf);
            client->write((const char*) buf, LINES*2*240);
        }
    }
}


void streamToIP(const char* host) {
    static char _host[81];
    static lv_task_t* task = nullptr;
    if (host==nullptr) {
        if (task!=nullptr) {
            task->user_data = nullptr;
            task = nullptr;
        }
    } else {
        strncpy(_host, host, 80);
        task = lv_task_create(streamToIPTask, 50, LV_TASK_PRIO_LOWEST, _host);
    }
}