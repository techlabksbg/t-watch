/**
 * @author Philipp Kündig
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(metronomeicon);

class Metronome : public App
{

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true; }

    virtual void loop();

    void *getIcon() { return (void *)&metronomeicon; }
    const char *getName() { return "Metronome"; }

private:
    lv_obj_t *metronome, *slider, *metronome_speed_label;
    // time_t beg_t;
    void set_labels(int speed)
    {
        static char buf[12];
        snprintf(buf, 12, "%ubpm", speed);
        lv_label_set_text(metronome_speed_label, buf);
        stop_loop();
        int wait_time = (60000/(float)speed)+0.5;
        start_loop(wait_time);
        // time(&beg_t);
        lv_obj_align(metronome_speed_label, slider, LV_ALIGN_OUT_TOP_MID, 0, -30);
    }
    static void slider_cb(lv_obj_t *obj, lv_event_t event)
    {
        if (event == LV_EVENT_VALUE_CHANGED)
        {
            int speed_label = lv_slider_get_value(obj);
            Metronome *speed = (Metronome *)lv_obj_get_user_data(obj);
            speed->set_labels(speed_label);
        }
    }
};
