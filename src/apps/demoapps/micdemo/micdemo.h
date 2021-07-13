/**
 * @author Ivo Blöchliger
 */

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


    private:
    int16_t* micBuffer = nullptr;
    bool clearIt = false;
    lv_task_t* micTask = nullptr;

    static MicDemo* self;
};

