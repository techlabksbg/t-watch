/**
 * @author JirR02
 */

#include "WaterMinder.h"

bool WaterMinder::create()
{

    lv_obj_t* bg = styles.stdBgImage(myScr);

    register_for_swipe_up(bg);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);
    
    lv_obj_t* WaterbottleImage = lv_img_create(myScr, NULL);
    lv_img_set_src(WaterbottleImage, &WaterBottle);
    lv_obj_align(WaterbottleImage, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_obj_t* Counter = lv_label_create(myScr, NULL);
    lv_label_set_recolor(Counter, true);
    lv_label_set_text(Counter, "#ffffff Drank 0 Bottles");
    lv_obj_align(Counter, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -40);

    /**
    Button in Progress
    */

    lv_obj_t* LabelCounterButton = lv_label_create(myScr, NULL);
    lv_label_set_text(LabelCounterButton, "+ 1 Bottle");
    lv_obj_align(LabelCounterButton, NULL, LV_ALIGN_CENTER, 0, 0);

    return true;
}