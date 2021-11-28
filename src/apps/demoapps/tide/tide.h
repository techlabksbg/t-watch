/**
 * @author Sebastian Gfeller
 */

#pragma once

#include "../../app.h"
#include "HTTPClient.h"

LV_IMG_DECLARE(tideicon);


class Tide : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &tideicon; }
    const char * getName() { return "Tide"; }

    lv_obj_t* highTitle;
    lv_obj_t* highLabel;
    lv_obj_t* lowTitle;
    lv_obj_t* lowLabel;
};
