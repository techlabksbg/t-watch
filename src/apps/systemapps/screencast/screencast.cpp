/**
 * @author Ivo Blöchliger
 */
#include "screencast.h"
#include "../../../services/services.h"

bool ScreenCast::create() {
    // Add Background
    styles.stdBgImage(myScr);

    lv_obj_t* button = styles.stdButton(myScr, "Switch", button_cb);
    lv_obj_set_user_data(button, this); // So the callback can get the instance context.
    lv_obj_align(button, nullptr, LV_ALIGN_CENTER, 0,0);
}

void ScreenCast::click() {
    static bool on = false;
    if (!on) {
        streamToIP("192.168.1.177");
    } else {
        streamToIP(nullptr);
    }
}