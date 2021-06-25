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
    lv_obj_t* label;
    uint8_t pwm;

    private:

    static int percent2pwm(int p) {
        return p*p*254/10000+1;
    }
    static int pwm2percent(int p) {
        return (int)(sqrt(p/255.0)*100);
    }


    static void slider_cb(lv_obj_t *obj, lv_event_t event) {
        static char buf[12];
        if(event == LV_EVENT_VALUE_CHANGED) {
            Serial.println("Brightness::slider_cb");
            int p = lv_slider_get_value(obj);
            Brightness* brightness = (Brightness *) lv_obj_get_user_data(obj);
            brightness->pwm = percent2pwm(p);
            snprintf(buf, 12, "%u%% (%u)", p, brightness->pwm);
            Serial.println(buf);
            lv_label_set_text(brightness->label, buf);
            
            Serial.printf("pwm = %d\n", brightness->pwm);
            ttgo->bl->adjust(brightness->pwm);
        }
    }
};
