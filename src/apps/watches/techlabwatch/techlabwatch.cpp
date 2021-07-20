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
    // display correct time
    loop();
    // create task, if not already running
    if (task==nullptr) {
        /** 
         * Call task_cb every 1000ms
         * Pass the pointer to this instance as user-data
         * So we can call the loop method in the context of this instance.
         */
        task = lv_task_create(task_cb, 1000, LV_TASK_PRIO_LOWEST, this);
    }
    // All done.
    return true;
}


void TechLabWatch::loop() {
    time_t now;
    struct tm  info;
    char buf[64];
    // Getting time
    time(&now);
    localtime_r(&now, &info);
    strftime(buf, sizeof(buf), "%H\n%M\n%S", &info);
    //Serial.printf("About to set text to %s\n", buf);
    lv_label_set_text(timeLabel, buf);
    strftime(buf, sizeof(buf), "%Y\n%m\n%d", &info);
    lv_label_set_text(dateLabel, buf);
    //Serial.println("TechLabWatch::loop done.");
}

bool TechLabWatch::hide() {
    if (task!=nullptr) {
        Serial.println("TechLabWatch::hide  delete taks");
        lv_task_del(task);
        task = nullptr;
    }
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
