/**
 * @author Ivo Blöchliger
 */

#include "motorapp.h"

bool MotorApp::create() {
    lv_obj_t* bg = styles.stdBgImage(myScr);

    register_for_swipe_up(bg);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);
    
    lv_obj_t* motorButton = styles.stdButton(bg, "Brr...", motor_button_cb);
    lv_obj_align(motorButton, myScr, LV_ALIGN_CENTER, 0,0);


    return true;
}