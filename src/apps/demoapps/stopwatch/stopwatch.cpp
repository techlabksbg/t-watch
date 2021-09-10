#include "stopwatch.h"

bool Stopwatch::create()
{
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    stopwatch = lv_label_create(myScr, NULL);
    lv_obj_add_style(stopwatch, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_label_set_text(stopwatch, "Stopwatch");

    Serial.println("Creating Stopwatch");
    return true;
}

bool Stopwatch::show()
{
    Serial.println("Showing Stopwatch");
    return true;
}
