#pragma once

#include "../../app.h"

LV_IMG_DECLARE(motorappicon);


class MotorApp : public App {

    virtual bool create();
    virtual bool show() { return true;};
    virtual bool hide() { return true;};
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &motorappicon; }
    const char * getName() { return "MotorTest"; }

    static void motor_button_cb(lv_obj_t *obj, lv_event_t event) {        
        if (event != LV_EVENT_SHORT_CLICKED) return;
        ttgo->motor->onec();
    }
};
