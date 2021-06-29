#include "motorapp.h"

bool MotorApp::create() {
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    
    lv_obj_t* motorButton = styles.stdButton(myScr, "Brr...", motor_button_cb);
    lv_obj_align(motorButton, myScr, LV_ALIGN_CENTER, 0,0);


    return true;
}