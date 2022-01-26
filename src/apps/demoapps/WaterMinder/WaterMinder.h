/**
 * @author JirR02
 */

#pragma once

#include "../../app.h"
#include <string.h>

LV_IMG_DECLARE(WaterMindericon);
LV_IMG_DECLARE(WaterBottle)


class WaterMinder : public App {

    virtual bool create() { return true;};
    virtual bool show();
    virtual bool hide(); 
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &WaterMindericon; }
    const char * getName() { return "WaterMinder"; }

private:
    String Drank;
    String Bottles;
    String OutputText;
    int AmountOfBottlesInt = 0;
    int AmountOfBottlesGoal = 4;

    void Add_Water_Bottle();
    void Remove_Water_Bottle();
    void displayInfo();

    lv_obj_t *Counter;
    lv_obj_t *Goal;
    lv_obj_t *AddButton;
    lv_obj_t *RemoveButton;
};