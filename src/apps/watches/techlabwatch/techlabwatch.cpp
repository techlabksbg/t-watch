/**
 * @author Ivo Blöchliger
 */

#include "techlabwatch.h"

LV_IMG_DECLARE(techlabwatchbg);

bool TechLabWatch::create() {
    // Define a style for the labels (text)
    static lv_style_t labelStyle;
    lv_style_init(&labelStyle);
    lv_style_set_text_color(&labelStyle, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xa0, 0xc0, 0xff));
    lv_style_set_text_font(&labelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_28);

    // Define the background
    bg = lv_img_create(myScr, NULL);  /*Create an image object*/
    lv_img_set_src(bg, (void *) &techlabwatchbg);
    
    // Make app terminate, when swiping up
    register_for_swipe_up(bg);

    // Craeat time an date labels
    timeLabel = lv_label_create(myScr, NULL);
    dateLabel = lv_label_create(myScr, NULL);
    // Apply style
    lv_obj_add_style(timeLabel, LV_OBJ_PART_MAIN, &labelStyle);
    lv_obj_add_style(dateLabel, LV_OBJ_PART_MAIN, &labelStyle);
    // Set text align
    lv_label_set_align(timeLabel, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(dateLabel, LV_LABEL_ALIGN_CENTER);

    // Align Background on screen
    lv_obj_align(bg, NULL, LV_ALIGN_CENTER, 0, 0);
    // Align labels on screen
    lv_obj_align(timeLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
    lv_obj_align(dateLabel, NULL, LV_ALIGN_IN_TOP_RIGHT, -20, 20);

    // All done!
    return true;
}

bool TechLabWatch::show() {
    // display correct time now
    loop();

    // call loop every 1000 ms
    start_loop(1000);

    return true;
}


void TechLabWatch::loop() {
    RTC_Date r = ttgo->rtc->getDateTime();
    char buf[64];
    snprintf(buf, sizeof(buf), "%02d\n%02d\n%02d", r.hour, r.minute, r.second);
    lv_label_set_text(timeLabel, buf);
    snprintf(buf, sizeof(buf), "%4d\n%02d\n%02d", r.year, r.month, r.day);
    lv_label_set_text(dateLabel, buf);
}

bool TechLabWatch::hide() {
    stop_loop();
    return true;
}

bool TechLabWatch::destroy() {
    if (bg!=nullptr) {
        lv_obj_del(timeLabel);
        lv_obj_del(dateLabel);
        lv_obj_del(bg);
        bg = nullptr;
        timeLabel = nullptr;
        dateLabel = nullptr;
    }
    return true;
}
