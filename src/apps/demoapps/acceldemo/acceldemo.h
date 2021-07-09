#pragma once

#include "../../app.h"

LV_IMG_DECLARE(acceldemoicon);


class AccelDemo : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &acceldemoicon; }
    const char * getName() { return "AccelDemo"; }

     static void _demoLoop(struct _lv_task_t *data) {
        self->demoLoop();
    }

    private:

    void demoLoop();
    static AccelDemo* self;
    lv_task_t* demoTask = nullptr;
    double x = 120;
    double y = 120;
    double fx = 0;
    double fy = 0;

};
