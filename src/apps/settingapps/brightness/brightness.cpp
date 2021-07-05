#include "brightness.h"

bool Brightness::create() {
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    

    slider = lv_slider_create(myScr, NULL);
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, myScr, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(slider, 0, 100);

    pwm = ttgo->bl->getLevel();
    uint16_t percent = pwm2percent(pwm);
    lv_slider_set_value(slider, percent, LV_ANIM_OFF);
    lv_obj_set_user_data(slider, this);

    percentLabel = styles.stdTitle(myScr, "");
    pwmLabel = styles.stdLabel(myScr, "");

    setLabels(percent);

    return true;
}

bool Brightness::show() {
    // Enable gui-callbacks
    register_for_swipe_up(myScr);
    lv_obj_set_event_cb(slider, slider_cb);
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

