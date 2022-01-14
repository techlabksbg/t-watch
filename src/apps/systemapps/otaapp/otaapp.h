/**
 * @author Ivo Blöchliger
 */


#pragma once

#include "../../app.h"

LV_IMG_DECLARE(otaappicon);


class OTAApp : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();
    virtual void loop();
    virtual void lv_event_callback(lv_obj_t* obj, lv_event_t event);


    
    void* getIcon() { return (void*) &otaappicon; }
    const char * getName() { return "OTA"; }

    private:

    lv_obj_t* webUpdateButton;
    lv_obj_t* arduinoButton;
    lv_obj_t* infoLabel;

    void showInfo(const char * msg);
    char* buffer =  nullptr;
    bool running = false;
    enum {NONE, SERVER, CLIENT} status = NONE;
};
