/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"
#include "../wifimanager/wifimanager.h"


LV_IMG_DECLARE(ntpsyncicon);



class NTP_Sync : public App {

    public:


    virtual bool create();
    virtual bool show();
    virtual bool hide() { return true;}
    virtual bool destroy() { return true;}

    void* getIcon() { return (void*) &ntpsyncicon; }
    const char * getName() { return "NTP Sync"; }

    private:
    lv_obj_t* title=nullptr;
    lv_obj_t* quit=nullptr;
    lv_obj_t* connect=nullptr;
    static NTP_Sync* self;

    static void connect_cb(lv_obj_t *button, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        wifiManager.connect(self);
    }

    static void ntp_cb(timeval *tv) {
        Serial.println("NTP_Sync::ntp_cb() called, syncing to rtc");
        ttgo->rtc->syncToRtc();
    }


};

