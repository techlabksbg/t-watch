#pragma once

#include "../../app.h"

LV_IMG_DECLARE(stopwatchicon);

class Stopwatch : public App
{

    virtual bool create();
    virtual bool show();
    virtual bool hide(); // { return true; };
    virtual bool destroy() { return true; }

    virtual void loop();

    void *getIcon() { return (void *)&stopwatchicon; }
    const char *getName() { return "Stopwatch"; }

private:
    lv_obj_t *stopwatch, *time_passed, *start_stop_button, *start_stop_label;
    time_t start_stop_t;
    void start_stop_time();
    bool running;
};
