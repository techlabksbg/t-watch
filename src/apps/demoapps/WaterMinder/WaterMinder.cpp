/**
 * @author Jirayu Ruh
 */

#include "WaterMinder.h"

bool WaterMinder::create() {
    lv_obj_t* bg = styles.stdBgImage(myScr);

    register_for_swipe_up(bg);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);
    
    lv_obj_t* WaterbottleImage = lv_img_create(myScr, NULL);
    lv_img_set_src(WaterbottleImage, &WaterBottle);
    lv_obj_align(WaterbottleImage, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_obj_t* CounterButton = lv_btn_create(myScr, NULL);
    lv_obj_set_event_cb(CounterButton, event_handler);
    lv_obj_align(CounterButton, NULL, LV_ALIGN_CENTER,0,0);

    return true;
}