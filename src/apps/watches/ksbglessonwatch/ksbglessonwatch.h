/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(ksbglessonwatchicon);


class KSBGLessonWatch : public App {
    public:

    KSBGLessonWatch() {
        appType = TYPE_WATCH;
    }

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();

    virtual void* getIcon() { return (void*) &ksbglessonwatchicon; }
    virtual const char * getName() { return "KSBG Watch"; }

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
    lv_obj_t* remainLabel = nullptr;

    struct lessonTime {
        int hours;
        int minutes;
    };

    lessonTime remaining(RTC_Date r);



    

};

