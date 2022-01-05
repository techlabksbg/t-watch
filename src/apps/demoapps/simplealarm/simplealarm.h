/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(simplealarmicon);


class SimpleAlarm : public App {
    public:

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() {return true;}

    virtual void* getIcon() { return (void*)&simplealarmicon; }
    virtual const char * getName() {return "Simple Alarm";}

    virtual void processAlarm();
    virtual void loop();
    virtual void lv_event_callback(lv_obj_t* obj, lv_event_t event);

    private:

    lv_obj_t* hoursSlider = nullptr;
    lv_obj_t* minutesSlider = nullptr;
    lv_obj_t* timeLabel = nullptr;
    lv_obj_t* setButton = nullptr;
    lv_task_t* alarmLoopTask = nullptr;
    int alarmCounter = 0;
    lv_color_t defaultBGColor = LV_COLOR_MAKE(0,0,127);

    int hours = 0;
    int minutes = 0;

};

