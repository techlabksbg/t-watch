/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(screencasticon);


class ScreenCast : public App {

    virtual bool create();
    virtual bool show() {return true;}
    virtual bool hide() {return true;}
    virtual bool destroy() {return true;}
    
    void* getIcon() { return (void*) &screencasticon; }
    const char * getName() { return "Screencast"; }

    static void button_cb(lv_obj_t* obj, lv_event_t e) {
        if (e!=LV_EVENT_CLICKED) return;
        ((ScreenCast*)(obj->user_data))->click();
    }

    private:
    void click();

};
