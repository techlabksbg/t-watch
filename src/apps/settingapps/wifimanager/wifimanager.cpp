/**
 * @author Ivo Blöchliger
 */

#include "wifimanager.h"

WifiManager::WifiManager() {
    self = this;
    password = new char[PW_MAX_LENGTH+1];
    ssid = new char[PW_MAX_LENGTH+1];
    password[0] = 0;
    ssid[0] = 0;
}

void WifiManager::addToggle() {
    Serial.println("WifiManager::addToggle()");
    if (toggle==nullptr && stateLabel!=nullptr) {
        toggle = lv_switch_create(page, NULL);
        lv_obj_align(toggle, stateLabel, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_switch_on(toggle, LV_ANIM_OFF);
        lv_obj_set_event_cb(toggle, toggle_cb);
        lv_label_set_text(stateLabel, "WiFi ON");
    }
    Serial.println("WifiManager::addToggle() done");
}
void WifiManager::removeToggle() {
    Serial.println("WifiManager::removeToggle()");
    if (toggle!=nullptr) {
        lv_obj_del(toggle);
        toggle=nullptr;
    }
    if (stateLabel!=nullptr) {
        lv_label_set_text(stateLabel, "WiFi off");
    }
}

void WifiManager::clearCheckedButtons() {
    for (int i=1; i<savedSSIDs; i++) {
        if (lv_btn_get_state(savedCont[i])==LV_BTN_STATE_CHECKED_PRESSED || lv_btn_get_state(savedCont[i])==LV_BTN_STATE_CHECKED_RELEASED) {
            lv_btn_toggle(savedCont[i]);
        }
    }
}
void WifiManager::addConnectionButtons(const char* name) {
    Serial.printf("WifiManager::addConnectionButtons(%s)\n", name);
    if (savedSSIDs>=WIFI_MAX_SAVED) return;
    lv_obj_t* button = styles.stdButton(page, name, connect_cb);
    savedCont[savedSSIDs++] = button;
    lv_obj_set_size(button, 195, 40);
    lv_page_glue_obj(button, true);
    lv_obj_align(button, savedCont[savedSSIDs-2], LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_btn_set_checkable(button, true); // Make it checkable
    Serial.println("Added button, about to add deletebutton");
    lv_obj_t* delButton = styles.stdButton(page, LV_SYMBOL_CLOSE, delete_cb);
    lv_obj_set_size(delButton, 40, 40);
    lv_obj_align(delButton, button, LV_ALIGN_OUT_RIGHT_MID,5,0);
    lv_obj_set_user_data(delButton, button); // To get from the delButton to the button and the ssid
    lv_obj_set_user_data(button, delButton); // To get from the button to the delButton for realignment
    lv_page_glue_obj(delButton, true);
    Serial.println("WifiManager::addConnectionButtons done.");
}

bool WifiManager::create() {
    // Get latest ssid;
    if ((*configJson)["wifi"].containsKey("last")) {
        strlcpy(ssid, (*configJson)["wifi"]["last"], PW_MAX_LENGTH);
        if ((*configJson)["wifi"].containsKey("known") && (*configJson)["wifi"]["known"].containsKey(ssid)) {
            strlcpy(password, (*configJson)["wifi"]["known"][ssid], PW_MAX_LENGTH);
        } else {
            ssid[0] = 0;
        }
    }

    
    savedCont = new lv_obj_t*[WIFI_MAX_SAVED];
    scannedCont = new lv_obj_t*[WIFI_MAX_SCANNED];
    
    page = styles.stdPage(myScr);
    lv_obj_t* quitButton = styles.stdButton(page, "Quit");
    register_for_hide_on_click(quitButton);
    lv_page_glue_obj(quitButton, true);
    lv_obj_align(quitButton, page, LV_ALIGN_IN_TOP_MID, 0, 5);


    bool conn = WiFi.isConnected();
    isOff = !conn;
    stateLabel = styles.stdTitle(page, "WiFi off");
    lv_obj_align(stateLabel, page, LV_ALIGN_IN_TOP_LEFT, 0, 55);
    if (conn) {
        addToggle();
    }
    savedCont[0] = styles.stdTitle(page, "Saved SSIDs");
    lv_obj_align(savedCont[0], stateLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    savedSSIDs=1;
    for (JsonPair kv : (JsonObject)((*configJson)["wifi"]["known"])) {     
        if (savedSSIDs>=WIFI_MAX_SAVED) break;
        addConnectionButtons(kv.key().c_str());
    }
    if (savedSSIDs<WIFI_MAX_SAVED) savedCont[savedSSIDs]=nullptr;

    scannedCont[0] = styles.stdTitle(page, "Scanned");
    lv_obj_align(scannedCont[0], savedCont[savedSSIDs-1], LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);

    scannedCont[1] = styles.stdButton(page, "Scan!");
    lv_page_glue_obj(scannedCont[1], true);
    lv_obj_align(scannedCont[1], scannedCont[0], LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    lv_obj_set_event_cb(scannedCont[1], scan_cb);
    scannedCont[2] = nullptr;

    return true;
}

bool WifiManager::destroy() {
    delete[] savedCont;
    savedCont = nullptr;
    delete[] scannedCont;
    scannedCont = nullptr;
    delete[] password;
    password = nullptr;
    delete[] ssid;
    ssid = nullptr;
    return true;
}

void WifiManager::clearScanned() {
    Serial.println("WifiManager::clearScanned()");
    int scanned = 2;
    while (scanned<WIFI_MAX_SCANNED && scannedCont[scanned]!=nullptr) {
        Serial.printf("Trying to delete scan result %d\n", scanned);
        lv_obj_del(scannedCont[scanned]);
        scannedCont[scanned] = nullptr;
        scanned++;
    }
    Serial.println("WifiManager::clearScanned() done");
}

void WifiManager::scanDone() {
    int scanned = 2;
    int16_t len =  WiFi.scanComplete();
    Serial.printf("WifiManager::scanDone() with len=%d\n", len);
    styles.hideSpinner();
    for (int i = 0; i < len; ++i) {
        lv_obj_t* button = styles.stdButton(page, WiFi.SSID(i).c_str());
        
        scannedCont[scanned++] = button;
        if (scanned<WIFI_MAX_SCANNED) {
            scannedCont[scanned]=nullptr;
        }
        lv_obj_set_size(button,240,40);
        lv_obj_align(button, scannedCont[scanned-2], LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
        lv_page_glue_obj(button, true);
        lv_obj_set_event_cb(scannedCont[scanned-1], connect_new_cb);
    }
}

void WifiManager::enterPw() {
    pwPage = styles.stdPage(myScr);

    /*lv_cont_create(myScr,NULL);
    lv_obj_set_size(pwPage, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_style(pwPage, LV_OBJ_PART_MAIN, &styles.background);
    lv_obj_align(pwPage, NULL, LV_ALIGN_CENTER, 0, 0); */

    textArea  = lv_textarea_create(pwPage, NULL);
    lv_obj_set_size(textArea, LV_HOR_RES/2, 40);
    lv_textarea_set_text(textArea, "");
    lv_obj_align(textArea, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);


    lv_obj_t* kb = lv_keyboard_create(pwPage, NULL);
    lv_obj_set_size(kb, LV_HOR_RES*3/2, LV_VER_RES-40);
    lv_obj_align(kb, textArea, LV_ALIGN_OUT_BOTTOM_MID,0,0);
    lv_keyboard_set_cursor_manage(kb, true);
    lv_obj_set_event_cb(kb, kb_event_cb);
    lv_keyboard_set_textarea(kb, textArea);
    Serial.println("You can use your Serial connection to type in the password!");
    serial_listener_task = lv_task_create(serial_listener, 50, LV_TASK_PRIO_LOWEST, NULL);

}

void WifiManager::connect(App* callback) {
    // Show app wifi app (to hide other app)
    Serial.println("Wifi::connect()...");
    closeOnConnect = nullptr;
    show_app(this);
    if (ssid==nullptr || ssid[0]==0) {
        closeOnConnect = callback;
        Serial.println("No ssid defined, just using app, but with callback defined");
    } else {
        if ((!WiFi.isConnected()) || WiFi.SSID()!=String(ssid)) {
            styles.showSpinner(myScr, ssid);
            Serial.printf("Connecting to ssid=%s with pw=%s\n",ssid, password);
            isConnecting = true;
            if (WiFi.getMode()!=WIFI_STA) {
                WiFi.mode(WIFI_STA);
            }
            closeOnConnect = callback;
            WiFi.begin(ssid, password);
        } else {
            Serial.println("Connection already stablished, call calling app if any");
            closeOnConnect = nullptr;
            if (callback!=nullptr) {
                show_app(callback);
            } 
        }
    }
}

void WifiManager::disconnect() {
    if (!isOff || WiFi.isConnected() || WiFi.getMode()!=WIFI_STA) {
        Serial.println("WifiManager::disconnect() Turning WiFi off...");
        
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        removeToggle();
    } else {
        Serial.println("WifiManager::disconnect() WiFi was already off, not turning it off again...");
    }
    clearCheckedButtons();
    isOff = true;
}

void WifiManager::addNewKnown() {
    addConnectionButtons(ssid);
    Serial.println("WifiManager::addNewKnown() About to clear scanned...");
    clearScanned();
    lv_obj_align(scannedCont[0], savedCont[savedSSIDs-1], LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_obj_align(scannedCont[1], scannedCont[0], LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    Serial.println("WifiManager::addNewKnown() done.");
}


void WifiManager::removeSSID(lv_obj_t* delButton) {
    Serial.println("WifiManager::delete_cb");
    lv_obj_t* button = (lv_obj_t*) lv_obj_get_user_data(delButton);
    lv_obj_t* label = lv_obj_get_child(button,NULL);
    char* SSIDtoBeRemoved = lv_label_get_text(label);
    Serial.printf("About to delete %s\n", SSIDtoBeRemoved);
    (*configJson)["wifi"]["known"].remove(SSIDtoBeRemoved);
    if (strcmp((*configJson)["wifi"]["last"], SSIDtoBeRemoved)==0) {
        (*configJson)["wifi"]["last"] = "";
    }
    saveJsonConfig();
    // Rearrange GUI
    bool removed = false;
    for (int i=1; i<savedSSIDs; i++) {
        Serial.printf("Testing entry %d\n",i);
        if (savedCont[i]==button) {
            Serial.println("  Removing this one");
            lv_obj_del(button);
            lv_obj_del(delButton);
            removed = true;
            savedSSIDs--;
        }
        if (removed) {
            savedCont[i] = (i+1==WIFI_MAX_SAVED) ? nullptr : savedCont[i+1];
            if (savedCont[i]!=nullptr) {
                lv_obj_align(savedCont[i], savedCont[i-1], LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
                lv_obj_t* delB = (lv_obj_t*) lv_obj_get_user_data(savedCont[i]);
                lv_obj_align(delB, savedCont[i], LV_ALIGN_OUT_RIGHT_MID,5,0);
            }
        }
    }
    clearScanned();
    lv_obj_align(scannedCont[0], savedCont[savedSSIDs-1], LV_ALIGN_OUT_BOTTOM_LEFT, 0, 15);
    lv_obj_align(scannedCont[1], scannedCont[0], LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
}


void WifiManager::connectionEstablished() {
    isOff = false;
    isConnecting = false;
    Serial.println("Wifi connected!");
    const char* hostname = (*configJson)["hostname"];
    WiFi.setHostname(hostname);
    bool save = false;
    Serial.println("checking for wifi[known] entry...");
    if (!(*configJson)["wifi"].containsKey("known")) {
        Serial.println("  ... adding entry");
        (*configJson)["wifi"]["known"] = JsonObject();
        save = true;
    }
    Serial.println("Checking if ssid exists...");
    if (!(*configJson)["wifi"]["known"].containsKey(ssid)) {
        (*configJson)["wifi"]["known"][ssid]=password;
        Serial.println("new ssid added to json, calling addNewKnown()");
        addNewKnown();
        Serial.println("Toggle last button");
        lv_btn_toggle(savedCont[savedSSIDs-1]);
        save = true;
    }
    if (!(*configJson)["wifi"].containsKey("last") || strncmp(ssid, (*configJson)["wifi"]["last"],PW_MAX_LENGTH)!=0) {
        Serial.println("Setting last connection");
        (*configJson)["wifi"]["last"]=ssid;
        save = true;
    } 
    if (save) {
        saveJsonConfig();
    }
    styles.hideSpinner();
    addToggle();
    if (closeOnConnect!=nullptr) {
        Serial.println("WifiManager::connectionEstablished() opening calling app..");
        show_app(closeOnConnect);
        closeOnConnect = nullptr;
        Serial.println("WifiManager::connectionEstablished() done calling app..");
    }
    // NTP sync
    if (WiFi.isConnected()) {
        sntp_set_time_sync_notification_cb([](timeval *tv) {
            Serial.println("ntp callback, setting rtc to sys");
            rtcHandler->sys2rtc();
        });
        Serial.println("NTP sync start.");
        configTzTime((*configJson)["tz"], "pool.ntp.org");
    }
}

void WifiManager::connectionLost() {
    Serial.println("WifiManager::connectionLost()");
    if (isConnecting) {
        styles.hideSpinner();
        isConnecting=false;
        WiFi.mode(WIFI_OFF);
    } else {
        if (!isOff) disconnect();
    }
    clearCheckedButtons();
}

WifiManager wifiManager;
WifiManager* WifiManager::self = &wifiManager;


// TODO: when connected an switching to another wlan, wlan stopps completely first.