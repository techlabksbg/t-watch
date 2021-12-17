/**
 * @author JirR02
 */

#include "WaterMinder.h"

bool WaterMinder::create()
{

    lv_obj_t *bg = styles.stdBgImage(myScr);

    register_for_swipe_up(bg);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);

    lv_obj_t *WaterbottleImage = lv_img_create(myScr, NULL);
    lv_img_set_src(WaterbottleImage, &WaterBottle);
    lv_obj_align(WaterbottleImage, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_obj_t* LabelCounterButton = lv_label_create(myScr, NULL);
    lv_label_set_text(LabelCounterButton, "+ 1 Bottle");
    lv_obj_align(LabelCounterButton, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *motorButton = styles.stdButton(
        myScr, "+ 1 Bottle", [](lv_obj_t *button, lv_event_t event)
        {
        if (event != LV_EVENT_SHORT_CLICKED)
            return;
        ((WaterMinder *)(button->user_data))->Add_Water_Bottle();
        },
        this);
    Counter = lv_label_create(myScr, NULL);
    lv_label_set_recolor(Counter, true);
    lv_label_set_text(Counter, "#ffffff Drank 0 Bottles");
    lv_obj_align(Counter, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -40);
    lv_obj_set_sty
    
    lv_obj_align(motorButton, myScr, LV_ALIGN_CENTER, 0,0);

    return true;
}

void WaterMinder::Add_Water_Bottle()
{
    std::string Drank = "#ffffff Drank ";
    std::string Bottles = "#ffffff Bottle(s)";
    std::string OutputText;
    std::string AmountOfBottles = "#ffffff 0";

    OutputText = Drank + AmountOfBottles + Bottles;
    lv_label_set_text(Counter, OutputText.c_str());
}