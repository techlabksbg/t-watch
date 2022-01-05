/**
 * @author Ivo Blöchliger
 */

#include "motorapp.h"

bool MotorApp::create() {
    lv_obj_t* bg = styles.stdBgImage(myScr);

    register_for_swipe_up(bg);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);
    
    motorButton = styles.stdButton(bg, "Brr...");
    lv_obj_align(motorButton, myScr, LV_ALIGN_CENTER, 0,0);
    register_lv_event_callback(motorButton);
    return true;
}

void MotorApp::lv_event_callback(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_SHORT_CLICKED && obj==motorButton) {
        ttgo->motor->onec();
    }
}
