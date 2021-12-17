/**
 * @author JirR02
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(WaterMindericon);
LV_IMG_DECLARE(WaterBottle)


class WaterMinder : public App {

    virtual bool create();
    virtual bool show() { return true;};
    virtual bool hide() { return true;};
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &WaterMindericon; }
    const char * getName() { return "WaterMinder"; }

private:
    std::string Drank = "#ffffff Drank ";
    std::string Bottles = "#ffffff Bottle(s) ";
    std::string OutputText;
    std::string AmountOfBottles = "#ffffff 0";

    void Add_Water_Bottle();

    lv_obj_t *Counter;
};