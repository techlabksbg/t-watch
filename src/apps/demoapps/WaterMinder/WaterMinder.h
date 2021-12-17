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

    std::string Drank = "Drank";
    std::string Bottles = "Bottles";
    std::string OutputText;
    int AmountOfBottles = 0;
};