/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(wifiicon);

#define WIFI_MAX_SAVED 50
#define WIFI_MAX_SCANNED 50
#define PW_MAX_LENGTH 64

class WifiManager : public App {

    public:

    WifiManager();

    virtual bool create();
    virtual bool show() {Serial.println("WifiManager::show()"); return true;}
    virtual bool hide(){Serial.println("WifiManager::hide()");closeOnConnect = nullptr; return true;}
    virtual bool destroy();

    void* getIcon() { return (void*) &wifiicon; }
    const char * getName() { return "WIFI"; }

    void scanDone();
    void connectionEstablished();
    void connectionLost();
    void disconnect();
    void connect(App* callback=nullptr);



    private:
    lv_obj_t* page=nullptr;
    lv_obj_t* pwPage=nullptr;
    lv_obj_t* textArea=nullptr;
    lv_obj_t* stateLabel=nullptr;  
    lv_obj_t* toggle=nullptr;  
    lv_obj_t** savedCont=nullptr;
    lv_obj_t** scannedCont=nullptr;
    char* password=nullptr;
    char* ssid=nullptr;
    bool isOff=true;
    bool isConnecting=false;
    App* closeOnConnect = nullptr;
    int savedSSIDs = 0;
    lv_task_t* serial_listener_task = nullptr;


    void clearScanned();
    void enterPw();
    void addToggle();
    void removeToggle();
    void addNewKnown();
    void addConnectionButtons(const char* name);
    void clearCheckedButtons();
    void removeSSID(lv_obj_t* delButton);

    

    static WifiManager* self;

    static void connect_cb(lv_obj_t *button, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        Serial.println("WifiManager::connect_cb");
        if (!(lv_btn_get_state(button)==LV_BTN_STATE_CHECKED_PRESSED || 
            lv_btn_get_state(button)==LV_BTN_STATE_CHECKED_RELEASED)) {
            Serial.println("Button is selected, turing wifi off");
            self->disconnect();
            return;
        }

        lv_obj_t* label = lv_obj_get_child(button,NULL);
        Serial.printf("Got label %p\n", label);
        strlcpy(self->ssid, lv_label_get_text(label), PW_MAX_LENGTH+1);
        Serial.printf("Got ssid=%s\n", self->ssid);
        strlcpy(self->password, (*configJson)["wifi"]["known"][self->ssid], PW_MAX_LENGTH+1);
        Serial.printf("Got pw %s\n", self->password);
        // Connect
        Serial.printf("Should connect to %s with pw %s\n",self->ssid, self->password);
        self->connect();
    }

    static void connect_new_cb(lv_obj_t *button, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        Serial.println("WifiManager::connect_new_cb");
        lv_obj_t* label = lv_obj_get_child(button,NULL);
        Serial.printf("Got label %p\n", label);
        strlcpy(self->ssid, lv_label_get_text(label), PW_MAX_LENGTH+1);
        Serial.printf("Got ssid=%s\n", self->ssid);
        if ((*configJson)["wifi"]["known"].containsKey(self->ssid)) {
            strlcpy(self->password, (*configJson)["wifi"]["known"][self->ssid], PW_MAX_LENGTH);
            Serial.printf("Got pw %s\n", self->password);
            // Connect
            Serial.printf("Should connect to %s with pw %s\n",self->ssid, self->password);
            self->connect();
        } else {
            self->enterPw();
        }
    }

    static void delete_cb(lv_obj_t *delButton, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        lv_obj_t* button = (lv_obj_t*) lv_obj_get_user_data(delButton);
        lv_obj_t* label = lv_obj_get_child(button,NULL);
        char buf[120];
        sprintf(buf, "Really remove\n%s?", lv_label_get_text(label));
        Serial.printf("MsgBox: %s\n", buf);
        static const char* btns[] = {"Cancel", "Ok", ""};  // Last string must be empty!
        styles.messageBox.show(buf, btns, [delButton](uint16_t btnNr)->void{
            Serial.printf("messageBox callback called with btn=%d\n", btnNr);
            if (btnNr==1) { // Only when OK
                self->removeSSID(delButton);
            }
        });
        Serial.println("delete_cb finished.");
    }

    static void scan_cb(lv_obj_t *scanButton, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        Serial.println("WifiManager::scan_cb");
        //WifiManager* self = (WifiManager*)lv_obj_get_user_data(scanButton);
        self->clearScanned();
        WiFi.disconnect();
        Serial.println("Start scan...");
        styles.showSpinner(self->myScr, "Scanning...");
        WiFi.scanNetworks(true);
    }

    static void kb_event_cb(lv_obj_t * keyboard, lv_event_t e) {  
        // default handler      
        lv_keyboard_def_event_cb(keyboard, e);
        // special events
        if (e == LV_EVENT_CANCEL || e==LV_EVENT_APPLY) {
            lv_task_del(self->serial_listener_task);
            self->serial_listener_task = nullptr;
            Serial.print("KB event");
            if (e==LV_EVENT_APPLY) {
                strlcpy(self->password, lv_textarea_get_text(self->textArea), PW_MAX_LENGTH+1);
                Serial.printf("  Have pw=%s\n", self->password);
            } else {
                Serial.println("  kb aborted");
                self->password[0]=0;
            }
            lv_keyboard_set_textarea(keyboard, NULL);
            lv_obj_del(keyboard);
            lv_obj_del(self->pwPage);
            self->pwPage = nullptr;
            if (self->password[0]!=0) {
                self->connect();
            }
        }
    }

    static void toggle_cb(lv_obj_t * toggle, lv_event_t e) {
        if(e == LV_EVENT_VALUE_CHANGED) {
            WiFi.mode(WIFI_OFF);
            self->removeToggle();
            self->closeOnConnect = nullptr;
        }
    }

    static void serial_listener(struct _lv_task_t *data) {
        while (Serial.available()) {
            char c = Serial.read();
            lv_textarea_add_char(self->textArea,c);
            Serial.write(c);
            Serial.flush();
        }
    }

};

extern WifiManager wifiManager;