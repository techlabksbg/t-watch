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

    virtual void loop();

    private:
    uint16_t x,y;
    bool drawing = false;
    unsigned long last=0;

    lv_obj_t* label;
    char * current;

};
