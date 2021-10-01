#pragma once

#include "../../app.h"

LV_IMG_DECLARE(stopwatchicon);

class Stopwatch : public App
{

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true; }

    virtual void loop();

    void *getIcon() { return (void *)&stopwatchicon; }
    const char *getName() { return "Stopwatch"; }

private:
    lv_obj_t *stopwatch, *time_passed, *last_time, *start_pause_resume_button, *start_pause_resume_label, *stop_reset_button, *stop_reset_label;
    time_t start_stop_t;
    void start_pause_resume_time(), stop_reset_time();
    int state = 0, last_state = -1;
};
