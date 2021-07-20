/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(drawingicon);


class Drawing : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &drawingicon; }
    const char * getName() { return "Draw Demo"; }

    virtual bool processDoubleTap(){ return true; } // Do not hide app on double tap

    static void task_cb(struct _lv_task_t *data) {
         ((Drawing*)data->user_data)->loop();
    }

    private:
    lv_task_t* task = nullptr;
    void loop();
    uint16_t x,y;
    bool drawing = false;
    unsigned long last=0;

};
