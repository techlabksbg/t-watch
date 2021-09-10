#include "First_App.h"

bool First_app::create()
{
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    hello_world = lv_label_create(myScr, NULL);
    lv_obj_add_style(hello_world, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_label_set_text(hello_world, "Hello World");

    Serial.println("Creating First_App");
    return true;
}

bool First_app::show()
{
    Serial.println("Showing First_App");
    return true;
}
