/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(techlabwatchicon);


class TechLabWatch : public App {
    public:

    TechLabWatch() {
        appType = TYPE_WATCH;
    }

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();

    virtual void* getIcon() { return (void*) &techlabwatchicon; }
    virtual const char * getName() { return "TechLab Watch"; }

    /**
     * Called every 1000ms from task_cb to
     * display current time.
     */
    virtual void loop();

    private:
    // GUI elements
    lv_obj_t* bg = nullptr;
    lv_obj_t* timeLabel = nullptr;
    lv_obj_t* dateLabel = nullptr;

    
    

};

