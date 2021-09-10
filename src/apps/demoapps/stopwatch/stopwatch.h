#pragma once

#include "../../app.h"

LV_IMG_DECLARE(stopwatchicon);

class Stopwatch : public App
{

    virtual bool create();
    virtual bool show();
    virtual bool hide() { return true; };
    virtual bool destroy() { return true; }

    void *getIcon() { return (void *)&stopwatchicon; }
    const char *getName() { return "Stopwatch"; }
    lv_obj_t *stopwatch;
};
