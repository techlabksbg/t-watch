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

    
    void* getIcon() { return (void*) &otaappicon; }
    const char * getName() { return "OTA"; }

    private:

    lv_obj_t* webUpdateButton;
    lv_obj_t* arduinoButton;
    lv_obj_t* infoLabel;

    void showInfo();
    char* buffer =  nullptr;
    bool running = false;
};
