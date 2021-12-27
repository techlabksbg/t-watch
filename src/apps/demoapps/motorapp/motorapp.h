/**
 * @author Ivo Blöchliger
 */

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

    virtual void lv_event_callback(lv_obj_t* obj, lv_event_t event);

    private:
    lv_obj_t* motorButton = nullptr;
    
};
