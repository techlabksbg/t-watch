#include "motorapp.h"

bool MotorApp::create() {
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    lv_obj_t* motorButton = styles.stdButton(myScr, "Brr...");
    lv_obj_align(motorButton, myScr, LV_ALIGN_CENTER, 0,0);
    lv_obj_set_event_cb(motorButton, motor_button_cb);


    return true;
}