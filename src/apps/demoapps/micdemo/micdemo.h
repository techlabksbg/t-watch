#pragma once

#include "../../app.h"

LV_IMG_DECLARE(micdemoicon);


class MicDemo : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();

    void setup();
    void tearDown();
    size_t readData();
    
    void* getIcon() { return (void*) &micdemoicon; }
    const char * getName() { return "Microphone"; }

    static void micLoop(struct _lv_task_t *data) {
        self->readData();
    }

    static void motor_button_cb(lv_obj_t *obj, lv_event_t event) {        
        if (event != LV_EVENT_SHORT_CLICKED) return;
        ttgo->motor->onec();
    }

    private:
    int16_t* micBuffer = nullptr;
    bool clearIt = false;
    lv_task_t* micTask = nullptr;

    static MicDemo* self;
};

