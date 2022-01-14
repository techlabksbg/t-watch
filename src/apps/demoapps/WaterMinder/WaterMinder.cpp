/**
 * @author JirR02
 */

#include "WaterMinder.h"
#include <string.h>

bool WaterMinder::create()
{

    lv_obj_t *bg = styles.stdBgImage(myScr);

    register_for_swipe_up(bg);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);

    lv_obj_t *WaterbottleImage = lv_img_create(myScr, NULL);
    lv_img_set_src(WaterbottleImage, &WaterBottle);
    lv_obj_align(WaterbottleImage, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

    lv_obj_t *AddButton = styles.stdButton(
        myScr, "+ 1 Bottle", [](lv_obj_t *button, lv_event_t event)
        {
        if (event != LV_EVENT_SHORT_CLICKED)
            return;
        ((WaterMinder *)(button->user_data))->Add_Water_Bottle();
        },
        this);

    lv_obj_t *RemoveButton = styles.stdButton(
        myScr, "- 1 Bottle", [](lv_obj_t *button, lv_event_t event)
        {
        if (event != LV_EVENT_SHORT_CLICKED)
            return;
        ((WaterMinder *)(button->user_data))->Remove_Water_Bottle();
        },
        this);

    Counter = lv_label_create(myScr, NULL);
    lv_obj_add_style(Counter, LV_OBJ_PART_MAIN, &styles.textLabel);
    lv_label_set_text(Counter, "Drank 0 Bottle(s)");
    lv_obj_align(Counter, NULL, LV_ALIGN_IN_BOTTOM_MID, 0,-30);

    Goal = lv_label_create(myScr, NULL);
    lv_obj_add_style(Goal, LV_OBJ_PART_MAIN, &styles.textLabel);
    lv_label_set_text(Goal, "Goal: Drink 4 Bottles");
    lv_obj_align(Goal, NULL, LV_ALIGN_IN_BOTTOM_MID, 0,-5);
    
    lv_obj_align(AddButton, myScr, LV_ALIGN_CENTER, 0,-15);
    lv_obj_align(RemoveButton, myScr, LV_ALIGN_CENTER, 0,35);

    return true;
}

void WaterMinder::Add_Water_Bottle()
{
    String Drank = "Drank ";
    String Bottles = " Bottle(s) ";

    AmountOfBottlesInt = AmountOfBottlesInt + 1;

    OutputText = Drank + AmountOfBottlesInt + Bottles;
    lv_label_set_text(Counter, OutputText.c_str());

    if(AmountOfBottlesInt == AmountOfBottlesGoal)
    {
       lv_label_set_text(Goal, "Goal reached!");
    }
}

void WaterMinder::Remove_Water_Bottle()
{
    String Drank = "Drank ";
    String Bottles = " Bottle(s) ";

    AmountOfBottlesInt = AmountOfBottlesInt - 1;

    if(AmountOfBottlesInt < 0)
    {
        AmountOfBottlesInt = 0;
    }

    OutputText = Drank + AmountOfBottlesInt + Bottles;
    lv_label_set_text(Counter, OutputText.c_str());

    if(AmountOfBottlesInt < AmountOfBottlesGoal)
    {
        lv_label_set_text(Goal, "Goal: Drink 4 Bottles");
    }   
}