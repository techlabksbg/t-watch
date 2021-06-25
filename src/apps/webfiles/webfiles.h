#pragma once

#include "../app.h"
#include "ESPAsyncWebServer.h"
#include "../settingapps/wifimanager/wifimanager.h"


LV_IMG_DECLARE(webfilesicon);



class WebFiles : public App {

    public:


    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}

    void* getIcon() { return (void*) &webfilesicon; }
    const char * getName() { return "WebFiles"; }

    private:
    static WebFiles* self;
    lv_obj_t* title=nullptr;
    lv_obj_t* quit=nullptr;
    lv_obj_t* connect=nullptr;
    AsyncWebServer* server=nullptr;

    void registerHandlers();

    static void connect_cb(lv_obj_t *button, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        wifiManager.connect(self);
    }

};

