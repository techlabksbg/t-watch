/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(bhfsgicon);


class BhfSGWatch : public App {
    public:

    BhfSGWatch() {
        appType = TYPE_WATCH;
    }

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();

    virtual void* getIcon() { return (void*) &bhfsgicon; }
    virtual const char * getName() { return "Bahnhof SG"; }

    /**
     * Implement my own loop method
     */
    virtual void loop();


    private:
    // GUI elements
    lv_obj_t*** imgs = nullptr;
    void *** imgsrcs = nullptr;
   

};

