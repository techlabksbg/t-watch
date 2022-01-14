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

    private:

    static int percent2pwm(int p) {
        return p*p*254/10000+1;
    }
    static int pwm2percent(int p) {
        return (int)(sqrt(p/255.0)*100);
    }

    void setLabels(int percent) {
        static char buf[12];
        snprintf(buf, 12, "%u%%", percent);
        lv_label_set_text(percentLabel, buf);
        snprintf(buf, 12, "%.3f PWM", pwm/255.0);
        lv_label_set_text(pwmLabel, buf);
        lv_obj_align(percentLabel, slider, LV_ALIGN_OUT_TOP_MID, 0, -30);
        lv_obj_align(pwmLabel, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
    }

    static void slider_cb(lv_obj_t *obj, lv_event_t event) {
        if(event == LV_EVENT_VALUE_CHANGED) {
            int percent = lv_slider_get_value(obj);
            Brightness* brightness = (Brightness *) lv_obj_get_user_data(obj);
            brightness->pwm = percent2pwm(percent);
            brightness->setLabels(percent);
            ttgo->bl->adjust(brightness->pwm);
        }
    }
};
