/**
 * @author Ivo Blöchliger
 */

#include "techlabwatch.h"

LV_IMG_DECLARE(techlabwatchbg);
LV_IMG_DECLARE(techlabwatchicon);

bool TechLabWatch::create() {
    static lv_style_t labelStyle;
    lv_style_init(&labelStyle);
    lv_style_set_text_color(&labelStyle, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xa0, 0xc0, 0xff));
    lv_style_set_text_font(&labelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_28);
    bg = lv_img_create(myScr, NULL);  /*Create an image object*/
    lv_img_set_src(bg, (void *) &techlabwatchbg);
    
    register_for_swipe_up(bg);

    timeLabel = lv_label_create(myScr, NULL);
    lv_obj_add_style(timeLabel, LV_OBJ_PART_MAIN, &labelStyle);
    dateLabel = lv_label_create(myScr, NULL);
    lv_obj_add_style(dateLabel, LV_OBJ_PART_MAIN, &labelStyle);

    lv_label_set_align(timeLabel, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(dateLabel, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(bg, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(timeLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
    lv_obj_align(dateLabel, NULL, LV_ALIGN_IN_TOP_RIGHT, -20, 20);

    return true;
}

bool TechLabWatch::show() {
    //Serial.println("TechLabWatch::show About to call loop");
    loop(nullptr);
    if (task==nullptr) {
        Serial.println("TechLabWatch::show Creating task");
        task = lv_task_create(loop, 1000, LV_TASK_PRIO_LOWEST, NULL);
    }
    return true;
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



const char* TechLabWatch::getName() {
    return "Tech-Lab";
}

void* TechLabWatch::getIcon() {
    return (void*) &techlabwatchicon;
}


lv_obj_t* TechLabWatch::bg = nullptr;
lv_obj_t* TechLabWatch::timeLabel = nullptr;
lv_obj_t* TechLabWatch::dateLabel = nullptr;
lv_task_t* TechLabWatch::task = nullptr;
