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


    private:
    // GUI elements
    lv_obj_t*** imgs = nullptr;
    void *** imgsrcs = nullptr;

    // Pointer to lv task
    lv_task_t* task = nullptr;

    /**
     * Called every 1000ms from task_cb to
     * display current time.
     */
    void loop();

   

};

