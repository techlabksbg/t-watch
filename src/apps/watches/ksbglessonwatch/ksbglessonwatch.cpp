/**
 * @author Ivo Blöchliger
 */

#include "ksbglessonwatch.h"

LV_IMG_DECLARE(ksbglessonwatchbg);

bool KSBGLessonWatch::create() {
    // Black background on myScr:
    static lv_style_t blackBackground;
    lv_style_init(&blackBackground);
    lv_style_set_bg_color(&blackBackground, LV_STATE_DEFAULT,  LV_COLOR_MAKE(0,0,0));
    lv_obj_add_style(myScr,LV_OBJ_PART_MAIN, &blackBackground);

    // Define a style for the labels (text)
    static lv_style_t labelStyle;
    lv_style_init(&labelStyle);
    lv_style_set_text_color(&labelStyle, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xa0, 0xc0, 0xff));
    lv_style_set_text_font(&labelStyle, LV_STATE_DEFAULT, &ubuntumonob32);

    // Define a style for the labels (text)
    static lv_style_t dateStyle;
    lv_style_init(&dateStyle);
    lv_style_set_text_color(&dateStyle, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xa0, 0xc0, 0xff));
    lv_style_set_text_font(&dateStyle, LV_STATE_DEFAULT, &ubuntumonor32);

    // Define a style for the remaining time
    static lv_style_t remainStyle;
    lv_style_init(&remainStyle);
    lv_style_set_text_color(&remainStyle, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xa0, 0xc0, 0xff));
    lv_style_set_text_font(&remainStyle, LV_STATE_DEFAULT, &ubuntumonob64);

    // Define the background
    bg = lv_img_create(myScr, NULL);  /*Create an image object*/
    lv_img_set_src(bg, (void *) &ksbglessonwatchbg);
    
    // Make app terminate, when swiping up
    register_for_swipe_up(myScr);
    register_for_swipe_up(bg);

    // Craeat time an date labels
    timeLabel = lv_label_create(myScr, NULL);
    dateLabel = lv_label_create(myScr, NULL);
    remainLabel = lv_label_create(myScr, NULL);
    // Apply style
    lv_obj_add_style(timeLabel, LV_OBJ_PART_MAIN, &labelStyle);
    lv_obj_add_style(dateLabel, LV_OBJ_PART_MAIN, &dateStyle);
    lv_obj_add_style(remainLabel, LV_OBJ_PART_MAIN, &remainStyle);

    // Set text align
    lv_label_set_align(timeLabel, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(dateLabel, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(remainLabel, LV_LABEL_ALIGN_CENTER);

    // Align Background on screen
    lv_obj_align(bg, NULL, LV_ALIGN_IN_TOP_MID, 0, 65);

    // Set default labels for alignement
    lv_label_set_text(remainLabel, "00:00");
    lv_label_set_text(timeLabel, "00:00:00");
    lv_label_set_text(dateLabel, "2020-09-09");
    // Align labels on screen
    lv_obj_align(timeLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_align(dateLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 40);
    lv_obj_align(remainLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 170);

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

KSBGLessonWatch::lessonTime KSBGLessonWatch::remaining(RTC_Date r) {
    struct lessonTime lessons[] = {
        {7,40}, {8,25}, {8,34}, {9,19}, {9,28}, {10,13}, 
        {10,30}, {11,15}, {11,24}, {12,9}, {12,14}, {12,59}, {13,4}, {13,49}, 
        {13,55}, {14,40}, {14,49}, {15,34}, {15,43}, {16,28}, {16,33}, {17,18}, {17,23}, {18,8}
    };
    int seconds = r.hour*3600+r.minute*60+r.second;
    for (int i=0; i<sizeof(lessons)/sizeof(lessonTime); i++) {
        int secs = lessons[i].hours*3600+lessons[i].minutes*60;
        if (secs>seconds) {
            secs -= seconds;
            return {secs/60,secs%60};
        }
    }
    return {-1,-1}; // Gang go schlofe!
}

void KSBGLessonWatch::loop() {
    RTC_Date r = ttgo->rtc->getDateTime();
    char buf[64];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", r.hour, r.minute, r.second);
    lv_label_set_text(timeLabel, buf);
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d", r.year, r.month, r.day);
    lv_label_set_text(dateLabel, buf);
    lessonTime t = remaining(r);
    if (t.hours!=-1) {
        sprintf(buf, "%02d:%02d", t.hours, t.minutes);
        lv_label_set_text(remainLabel, buf);
    } else {
        lv_label_set_text(remainLabel, "--:--");
    }
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
