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

    private:

    lv_obj_t* hoursSlider = nullptr;
    lv_obj_t* minutesSlider = nullptr;
    lv_obj_t* timeLabel = nullptr;
    lv_task_t* alarmLoopTask = nullptr;
    int alarmCounter = 0;
    lv_color_t defaultBGColor = LV_COLOR_MAKE(0,0,127);


    int hours = 0;
    int minutes = 0;

    static void slider_cb(lv_obj_t *obj, lv_event_t event) {
        static char buf[10];
        if(event == LV_EVENT_VALUE_CHANGED) {
            SimpleAlarm* that = (SimpleAlarm*) lv_obj_get_user_data(obj);
            that->hours = lv_slider_get_value(that->hoursSlider);
            that->minutes = lv_slider_get_value(that->minutesSlider);
            sprintf(buf, "%02d:%02d", that->hours, that->minutes);
            lv_label_set_text(that->timeLabel, buf);
        }
    }

    static void setAlarm_cb(lv_obj_t *obj, lv_event_t event) {
        if (event!=LV_EVENT_CLICKED) return;
        Serial.println("Getting instance");
        SimpleAlarm* that = (SimpleAlarm*) lv_obj_get_user_data(obj);
        Serial.printf("that = %p\n",that);
        Serial.printf("Setting alarm to %02d:%02d with that=%p\n", that->hours, that->minutes, that);
        that->setAlarm(that->hours, that->minutes);
        Serial.println("I'm outa here...");
        that->hide_myself();
    }

    static void alarmLoop(lv_task_t * task) {
        SimpleAlarm* that = (SimpleAlarm*) task->user_data;
        if (that->alarmCounter % 2 == 1) {
            
            lv_obj_set_style_local_bg_color(that->myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, that->defaultBGColor);
        } else {
            lv_obj_set_style_local_bg_color(that->myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(255,255,0));
            if (that->alarmCounter % 4 == 0) {
                ttgo->motor->onec();
            }
        }
        if (that->alarmCounter==31) {
            that->hide_myself();
        }
        that->alarmCounter++;
    }

};

