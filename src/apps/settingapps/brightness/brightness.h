/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(brightnessicon);


class Brightness : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();
    
    void* getIcon() { return (void*) &brightnessicon; }
    const char * getName() { return "Brightness"; }

    lv_obj_t* bg;
    lv_obj_t* slider;
    lv_obj_t* percentLabel;
    lv_obj_t* pwmLabel;
    uint8_t pwm;

    virtual void lv_event_callback(lv_obj_t* obj, lv_event_t event);

    private:

   
    void setLabels(int percent); 

 
};
