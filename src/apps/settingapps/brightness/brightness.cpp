#include "brightness.h"

bool Brightness::create() {
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    register_for_swipe_up(myScr);

    slider = lv_slider_create(myScr, NULL);
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, myScr, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(slider, 0, 100);

    pwm = ttgo->bl->getLevel();
    uint16_t percent = pwm2percent(pwm);
    lv_slider_set_value(slider, percent, LV_ANIM_OFF);
    lv_obj_set_user_data(slider, this);
    lv_obj_set_event_cb(slider, slider_cb);

    percentLabel = styles.stdTitle(myScr, "");
    pwmLabel = styles.stdLabel(myScr, "");

    setLabels(percent);

    return true;
}

bool Brightness::show() {
    return true;
}

bool Brightness::hide() {
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

