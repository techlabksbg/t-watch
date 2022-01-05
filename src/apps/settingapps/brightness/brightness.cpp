/**
 * @author Ivo Blöchliger
 */

#include "brightness.h"

int percent2pwm(int p) {
    return p*p*254/10000+1;
}
int pwm2percent(int p) {
    return (int)(sqrt(p/255.0)*100);
}


bool Brightness::create() {
    lv_obj_t* bg = styles.stdBgImage(myScr);

    slider = lv_slider_create(bg, NULL);
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, bg, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(slider, 0, 100);

    pwm = ttgo->bl->getLevel();
    uint16_t percent = pwm2percent(pwm);
    lv_slider_set_value(slider, percent, LV_ANIM_OFF);
    lv_obj_set_user_data(slider, this);

    percentLabel = styles.stdTitle(bg, "");
    pwmLabel = styles.stdLabel(bg, "");

    setLabels(percent);

    return true;
}

bool Brightness::show() {
    // Enable gui-callbacks
    register_for_swipe_up(myScr);
    register_lv_event_callback(slider);
    return true;
}

bool Brightness::hide() {
    // First disable gui-callbacks. May be this was the cause for corrupted SPIFFS.
    // Spoiler: It wasn't.
    lv_obj_set_event_cb(slider, nullptr);
    lv_obj_set_event_cb(myScr, nullptr);
    (*configJson)["bl"] = (int)pwm;
    Serial.printf("about to save configJson['bl']=%d\n", pwm);
    saveJsonConfig();
    Serial.println("saving done");
    return true;
}

bool Brightness::destroy() {
    if (bg!=nullptr) { 
        lv_obj_del(bg);
        bg = nullptr;
        slider = nullptr;
    }
    return true;
}


void Brightness::setLabels(int percent) {
    static char buf[12];
    snprintf(buf, 12, "%u%%", percent);
    lv_label_set_text(percentLabel, buf);
    snprintf(buf, 12, "%.3f PWM", pwm/255.0);
    lv_label_set_text(pwmLabel, buf);
    lv_obj_align(percentLabel, slider, LV_ALIGN_OUT_TOP_MID, 0, -30);
    lv_obj_align(pwmLabel, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
}


void Brightness::lv_event_callback(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_VALUE_CHANGED && obj==slider) {
        int percent = lv_slider_get_value(obj);
        pwm = percent2pwm(percent);
        setLabels(percent);
        ttgo->bl->adjust(pwm);
    }
}