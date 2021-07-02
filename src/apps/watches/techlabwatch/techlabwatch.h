#pragma once

#include "../../app.h"


class TechLabWatch : public App {
    public:

    TechLabWatch() {
        appType = TYPE_WATCH;
    }

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();

    virtual void* getIcon();
    virtual const char * getName();


    private:
    static lv_obj_t* bg;
    static lv_obj_t* timeLabel;
    static lv_obj_t* dateLabel;
    static lv_task_t* task;

    static void loop(struct _lv_task_t *data) {
        time_t now;
        struct tm  info;
        char buf[64];
        //Serial.println("Getting time");
        time(&now);
        localtime_r(&now, &info);
        strftime(buf, sizeof(buf), "%H\n%M\n%S", &info);
        //Serial.printf("About to set text to %s\n", buf);
        lv_label_set_text(timeLabel, buf);
        strftime(buf, sizeof(buf), "%Y\n%m\n%d", &info);
        lv_label_set_text(dateLabel, buf);
        //Serial.println("TechLabWatch::loop done.");
    }

    

};

