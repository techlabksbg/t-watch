/**
 * @author Jirayu Ruh
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(WaterMindericon);
LV_IMG_DECLARE(WaterBottle)


class WaterMinder : public App {

    virtual bool create();
    virtual bool show() { return true;};
    virtual bool hide() { return true;};
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &WaterMindericon; }
    const char * getName() { return "WaterMinder"; }

    static void event_handler(lv_obj_t * obj, lv_event_t event) {        
        if (event != LV_EVENT_SHORT_CLICKED) return;
        ttgo->motor->onec();
    }
};