#include "services.h"
#include "../os/os.h"

#define STREAMING_PORT 8080
#define LINES 1

bool takeSnapshot = false;
WiFiClient* out = nullptr;
bool allOK = true;

// Copied from TTGO.h:1510
void disp_flush_snapshot(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) ;
    ttgo->tft->setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
    ttgo->tft->pushColors(( uint16_t *)color_p, size, false);
    if (takeSnapshot && out!=nullptr && allOK) {
        Serial.printf("Sending %d bytes\n", 2*size);
        allOK = (2*size == out->write((const char *)color_p, 2*size));
    }
    lv_disp_flush_ready(disp_drv);
}

void setupSnapshot() {   
    static bool done = false;
    if (!done) {
        done = true;
        static lv_disp_drv_t drv = lv_disp_get_default()->driver;
        drv.flush_cb = disp_flush_snapshot;
        lv_disp_drv_update(lv_disp_get_default(), &drv);
        //->driver.flush_cb = disp_flush_snapshot;
    }
}

void snapshot() {
    lv_obj_invalidate(lv_scr_act());
    takeSnapshot = true;
    lv_refr_now(nullptr);
    takeSnapshot = false;
}


void streamToIPTask(lv_task_t* task) {
    static bool streaming = false;
    static WiFiClient* client = nullptr;
    static uint16_t* buf=nullptr;
    char* host = (char*)task->user_data;
    if (host==nullptr) {
        Serial.println("Stopping screen cast");
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
        out = nullptr;
        allOK = false;
        lv_task_del(task);
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
        long start = millis();
        Serial.print("Sending frame... ");
        out = client;
        allOK = true;
        snapshot();
        if (!allOK) {
                task->user_data = nullptr; // Clean up in next call
                return;
        }
        start = millis()-start;
        Serial.printf("done after %d ms\n", (int)start);
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
        setupSnapshot();
        strncpy(_host, host, 80);
        task = lv_task_create(streamToIPTask, 1000, LV_TASK_PRIO_LOWEST, _host);
    }
}