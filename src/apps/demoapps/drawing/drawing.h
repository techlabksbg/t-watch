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
    virtual bool destroy();
    
    void* getIcon() { return (void*) &drawingicon; }
    const char * getName() { return "Draw Demo"; }

    virtual bool processDoubleTap(){ return true; } // Do not hide app on double tap

    virtual void loop();

    private:
    uint16_t x,y;
    bool drawing = false;
    bool finished = true;
    unsigned long last=0;

    lv_obj_t* label;
    char * current;
    struct pts_t {
        unsigned long time;
        int x;
        int y;
    };
    pts_t* pts=nullptr;
    int num_pts = 0;
    unsigned long startms = 0;

    char * buffer= nullptr;

    void output_and_increment();

};
