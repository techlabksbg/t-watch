/**
 * @author Ivo Blöchliger
 */

#include "otaapp.h"
#include "../../../os/ota.h"
#include "../../settingapps/wifimanager/wifimanager.h"

void OTAApp::loop() {
    //Serial.println("OTAApp::loop()");
    if (running) {
        if (WiFi.isConnected()) {
            watchOTA->loop();
        } else {
            watchOTA->stop();
            running = false;
            stop_loop();
            showInfo("");
        }
    } else {
        stop_loop();
    }
}

void OTAApp::showInfo(const char * msg) {
    lv_obj_t * label = lv_obj_get_child(arduinoButton, NULL);
    if (running) {
        if (status == SERVER) {
            lv_label_set_text(label, "Stop OTA");
            sprintf(buffer, "%s\n%s", WiFi.getHostname(), WiFi.localIP().toString().c_str());
            lv_label_set_text(infoLabel, buffer);
        } else if (status == CLIENT) {
            Serial.print(msg);
            lv_label_set_text(infoLabel, msg);
        }
    } else {
        lv_label_set_text(label, "Start OTA");
        lv_label_set_text(infoLabel, "not running");
    }
}

bool OTAApp::create() {
    if (buffer==nullptr) {
        buffer = new char[100];
    }
    styles.stdBgImage(myScr);
    infoLabel = styles.stdLabel(myScr, "not running");
    lv_label_set_align(infoLabel, LV_ALIGN_CENTER);

    arduinoButton = styles.stdButton(myScr, "Start OTA");
    register_lv_event_callback(arduinoButton);

    webUpdateButton = styles.stdButton(myScr, "Web Update");
    register_lv_event_callback(webUpdateButton);

    lv_obj_align(webUpdateButton,myScr, LV_ALIGN_CENTER, 0,-50);
    lv_obj_align(arduinoButton,webUpdateButton, LV_ALIGN_OUT_BOTTOM_MID, 0,0);
    lv_obj_align(infoLabel,arduinoButton, LV_ALIGN_OUT_BOTTOM_MID, 0,0);
    return true;
}


void OTAApp::lv_event_callback(lv_obj_t* obj, lv_event_t event) {
    if (event==LV_EVENT_SHORT_CLICKED) {
        if (obj==arduinoButton) {
            if (WiFi.isConnected()) {
                if (running) {
                    if (watchOTA->start(WatchOTA::ARDUINO_SERVER, nullptr)) {
                        running = true;
                        showInfo("Starting Update...");
                        start_loop(20);
                    }
                } else {
                    running = false;
                    showInfo("Aborting Update...");
                    stop_loop();
                    watchOTA->stop();
                }
            } else {
                wifiManager.connect(this);
            }
        }
        if (obj == webUpdateButton) {
            if (WiFi.isConnected()) {
                if (!running) {
                    if (watchOTA->start(WatchOTA::WEB_DOWNLOAD, [this](const char * msg){                    
                        showInfo(msg);
                    })) {
                        status = CLIENT;
                        running = true;
                        showInfo("Web Update");
                        start_loop(50);
                    }
                }
            } else {
                wifiManager.connect(this);
            }
        }
    }
}

bool OTAApp::show() {
    return true;
}
bool OTAApp::hide() {
    return true;
}
bool OTAApp::destroy() {
    if (buffer!=nullptr) {
        delete[] buffer;
        buffer = nullptr;
    }
    return true;
}