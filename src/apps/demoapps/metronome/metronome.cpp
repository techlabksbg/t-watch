/**
 * @author Philipp Kündig
 */

#include "metronome.h"

bool Metronome::create()
{
    register_for_swipe_up(myScr);
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    slider = lv_slider_create(myScr, NULL);
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, myScr, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(slider, 20, 250);
    lv_slider_set_value(slider, 80, LV_ANIM_ON);
    lv_obj_set_user_data(slider, this);

    metronome_speed_label = lv_label_create(myScr, NULL);
    lv_obj_add_style(metronome_speed_label, LV_OBJ_PART_MAIN, &styles.textLabel);
    set_labels(80);

    metronome = lv_label_create(myScr, NULL);
    lv_obj_add_style(metronome, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_label_set_text(metronome, "Metronome");
    lv_label_set_align(metronome, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(metronome, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);

    Serial.println("Creating Metronome");
    return true;
}

bool Metronome::show()
{
    Serial.println("Showing Metronome");
    // loop();
    // start_loop(6/(80*100));
    start_loop(60000/80);
    lv_obj_set_event_cb(slider, slider_cb);
    return true;
}

void Metronome::loop()
{
    Serial.println("beep");
    // time_t time_now;
    // struct tm info;
    // char buf[64];
    // time(&time_now);
    // time_now -= beg_t;
    // localtime_r(&time_now, &info);
    // strftime(buf, sizeof(buf), "%M:%S", &info);
    
    // if (state == 0)
    // {
    //     lv_label_set_text(time_passed, buf);
    // }
    // if (state != last_state)
    // {
    //     switch (state)
    //     {
    //     case 0: // Running
    //         if (last_state == 3)
    //         {
    //             time(&start_stop_t);               // reset time
    //             localtime_r(&start_stop_t, &info); // reset time
    //         }
    //         lv_label_set_text(start_pause_resume_label, "Pause");
    //         lv_label_set_text(stop_reset_label, "Stop");
    //         break;
    //     case 1: // Paused
    //         lv_label_set_text(last_time, buf);
    //         lv_label_set_text(start_pause_resume_label, "Resume");
    //         lv_label_set_text(stop_reset_label, "Reset");
    //         break;
    //     case 2: // Stopped
    //         lv_label_set_text(start_pause_resume_label, "Resume");
    //         lv_label_set_text(stop_reset_label, "Reset");
    //         break;
    //     case 3: // Resetted
    //         time_t reset_time = 0;
    //         localtime_r(&reset_time, &info);
    //         strftime(buf, sizeof(buf), "%M:%S", &info);
    //         lv_label_set_text(time_passed, buf);
    //         lv_label_set_text(last_time, buf);
    //         lv_label_set_text(start_pause_resume_label, "Start");
    //         lv_label_set_text(stop_reset_label, "----");
    //         break;
    //     }
    // }
    // last_state = state;
}

bool Metronome::hide()
{
    stop_loop();
    return true;
}
