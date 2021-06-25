#include "brightness.h"

bool Brightness::create() {
    Serial.println("create bg");
    bg = lv_cont_create(myScr, NULL);
    lv_cont_set_fit(bg, LV_FIT_PARENT);
    Serial.println("style");
    static lv_style_t bgStyle;
    lv_style_set_bg_color(&bgStyle, LV_STATE_DEFAULT,  LV_COLOR_BLACK);
    Serial.println("grad color");
    lv_style_set_bg_grad_color(&bgStyle, LV_STATE_DEFAULT,LV_COLOR_WHITE);
    lv_style_set_bg_grad_dir(&bgStyle, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    Serial.println("Apply style");
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &bgStyle);
    lv_obj_align(bg, myScr,LV_ALIGN_CENTER, 0,0);

    register_for_swipe_up(bg);


    Serial.println("make slider");
    slider = lv_slider_create(bg, NULL);
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, bg, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(slider, 0, 100);
    Serial.println("make label");
    label = lv_label_create(bg, NULL);
    lv_label_set_text(label, "");
    lv_obj_align(label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    Serial.println("Getting current value");
    uint16_t p = pwm2percent(ttgo->bl->getLevel());
    Serial.printf("p=%d, Level=%d\n",p, ttgo->bl->getLevel());
    lv_slider_set_value(slider, p, LV_ANIM_OFF);
    Serial.println("set_user_data");
    lv_obj_set_user_data(slider, this);
    lv_obj_set_event_cb(slider, slider_cb);
    return true;
}

bool Brightness::show() {
    return true;
}

bool Brightness::hide() {
    (*configJson)["bl"] = (int)pwm;
    int test = (*configJson)["bl"];
    Serial.printf("test=configJson['bl']=%d\n", test);
    saveJsonConfig();
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

