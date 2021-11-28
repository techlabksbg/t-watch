/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(timezoneicon);


class TimeZone : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();
    
    void* getIcon() { return (void*) &timezoneicon; }
    const char * getName() { return "Set Time Zone"; }

    private:

    lv_obj_t* page=nullptr;
    lv_obj_t* btn_matrix = nullptr;
    char** btn_map=nullptr;
    char* buf=nullptr;
    bool makeRegionButtons();
    void makeCitiesButtons(const char* region);
    void setTimeZone(const char* city);
    bool getMemory();
    void freeMemory();

};
