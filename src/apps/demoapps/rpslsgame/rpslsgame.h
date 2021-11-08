/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(rpslsgameicon);
LV_IMG_DECLARE(rpslsgamebg);



class RpslsGame : public App {
    public:

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();

    virtual void* getIcon() { return (void*)&rpslsgameicon; }
    virtual const char * getName() {return "Rock, Scissors,...";}

    class Overlay {
        public:
        Overlay(lv_obj_t* bg, int cx, int cy, int r, lv_color_t color);
        ~Overlay();
        private:
        lv_obj_t* overlay;
        lv_style_t* style;
    };

    private:
    // image background
    lv_obj_t* bg; 

    Overlay* active = nullptr;

    void click(int x, int y);

};

