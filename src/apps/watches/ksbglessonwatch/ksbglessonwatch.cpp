/**
 * @author Ivo Blöchliger
 */

#include "ksbglessonwatch.h"

LV_IMG_DECLARE(ksbglessonwatchbg);

bool KSBGLessonWatch::create() {
    // Define a style for the labels (text)
    static lv_style_t labelStyle;
    lv_style_init(&labelStyle);
    lv_style_set_text_color(&labelStyle, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xa0, 0xc0, 0xff));
    lv_style_set_text_font(&labelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_32);

    // Define the background
    bg = lv_img_create(myScr, NULL);  /*Create an image object*/
    lv_img_set_src(bg, (void *) &ksbglessonwatchbg);
    
    // Make app terminate, when swiping up
    register_for_swipe_up(bg);

    // Craeat time an date labels
    timeLabel = lv_label_create(myScr, NULL);
    remainLabel = lv_label_create(myScr, NULL);
    // Apply style
    lv_obj_add_style(timeLabel, LV_OBJ_PART_MAIN, &labelStyle);
    lv_obj_add_style(remainLabel, LV_OBJ_PART_MAIN, &labelStyle);
    // Set text align
    lv_label_set_align(timeLabel, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(remainLabel, LV_LABEL_ALIGN_CENTER);

    // Align Background on screen
    lv_obj_align(bg, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    // Align labels on screen
    lv_obj_align(timeLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
    lv_obj_align(remainLabel, NULL, LV_ALIGN_IN_TOP_RIGHT, -20, 20);

    // All done!
    return true;
}

bool KSBGLessonWatch::show() {
    // display correct time now
    loop();

    // call loop every 1000 ms
    start_loop(1000);

    return true;
}


void KSBGLessonWatch::loop() {
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
    lv_label_set_text(remainLabel, buf);
    //Serial.println("KSBGLessonWatch::loop done.");
}

bool KSBGLessonWatch::hide() {
    stop_loop();
    return true;
}

bool KSBGLessonWatch::destroy() {
    if (bg!=nullptr) {
        lv_obj_del(timeLabel);
        lv_obj_del(remainLabel);
        lv_obj_del(bg);
        bg = nullptr;
        timeLabel = nullptr;
        remainLabel = nullptr;
    }
    return true;
}
