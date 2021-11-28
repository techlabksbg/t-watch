#include "hello_world.h"

bool Hello_World::create()
{
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    bg = styles.stdBgImage(myScr);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);
    hello_world = lv_label_create(myScr, NULL);
    lv_obj_add_style(hello_world, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_label_set_text(hello_world, "Hello World");

    Serial.println("Creating Hello World");
    return true;
}

bool Hello_World::show()
{
    Serial.println("Showing Hello World");
    return true;
}
