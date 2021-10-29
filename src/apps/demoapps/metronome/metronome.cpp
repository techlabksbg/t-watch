#include "metronome.h"

bool Metronome::create()
{
    register_for_swipe_up(myScr);
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    stopwatch = lv_label_create(myScr, NULL);
    lv_obj_add_style(stopwatch, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_label_set_text(stopwatch, "Stopwatch");
    lv_label_set_align(stopwatch, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(stopwatch, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);

    time_passed = lv_label_create(myScr, NULL);
    lv_obj_add_style(time_passed, LV_OBJ_PART_MAIN, &styles.textLabel);
    lv_label_set_text(time_passed, "00:00");
    lv_label_set_align(time_passed, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(time_passed, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 50);

    last_time = lv_label_create(myScr, NULL);
    lv_obj_add_style(last_time, LV_OBJ_PART_MAIN, &styles.textLabel);
    lv_label_set_text(last_time, "00:00");
    lv_label_set_align(last_time, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(last_time, NULL, LV_ALIGN_IN_TOP_LEFT, 150, 50);

    start_pause_resume_button = styles.stdButton(
        myScr, "Start", [](lv_obj_t *button, lv_event_t event)
        {
            if (event != LV_EVENT_SHORT_CLICKED)
                return;
            ((Metronome *)(button->user_data))->start_pause_resume_time();
        },
        this);
    start_pause_resume_label = lv_obj_get_child(start_pause_resume_button, NULL);
    lv_label_set_align(start_pause_resume_label, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(start_pause_resume_button, myScr, LV_ALIGN_IN_TOP_LEFT, 20, 75);

    stop_reset_button = styles.stdButton(
        myScr, "Start", [](lv_obj_t *button, lv_event_t event)
        {
            if (event != LV_EVENT_SHORT_CLICKED)
                return;
            ((Metronome *)(button->user_data))->stop_reset_time();
        },
        this);
    stop_reset_label = lv_obj_get_child(stop_reset_button, NULL);
    lv_label_set_align(stop_reset_label, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(stop_reset_button, myScr, LV_ALIGN_IN_TOP_LEFT, 20, 125);

    Serial.println("Creating Stopwatch");
    return true;
}

bool Metronome::show()
{
    Serial.println("Showing Stopwatch");
    loop();
    start_loop(10);
    return true;
}

void Metronome::loop()
{
    time_t time_now;
    struct tm info;
    char buf[64];
    time(&time_now);
    time_now -= start_stop_t;
    localtime_r(&time_now, &info);
    strftime(buf, sizeof(buf), "%M:%S", &info);
    if (state == 0)
    {
        lv_label_set_text(time_passed, buf);
    }
    if (state != last_state)
    {
        switch (state)
        {
        case 0: // Running
            if (last_state == 3)
            {
                time(&start_stop_t);               // reset time
                localtime_r(&start_stop_t, &info); // reset time
            }
            lv_label_set_text(start_pause_resume_label, "Pause");
            lv_label_set_text(stop_reset_label, "Stop");
            break;
        case 1: // Paused
            lv_label_set_text(last_time, buf);
            lv_label_set_text(start_pause_resume_label, "Resume");
            lv_label_set_text(stop_reset_label, "Reset");
            break;
        case 2: // Stopped
            lv_label_set_text(start_pause_resume_label, "Resume");
            lv_label_set_text(stop_reset_label, "Reset");
            break;
        case 3: // Resetted
            time_t reset_time = 0;
            localtime_r(&reset_time, &info);
            strftime(buf, sizeof(buf), "%M:%S", &info);
            lv_label_set_text(time_passed, buf);
            lv_label_set_text(last_time, buf);
            lv_label_set_text(start_pause_resume_label, "Start");
            lv_label_set_text(stop_reset_label, "----");
            break;
        }
    }
    last_state = state;
}

bool Metronome::hide()
{
    stop_loop();
    return true;
}

void Metronome::start_pause_resume_time()
{
    switch (state)
    {
    case 0: // resetted
        state = 1;
        break;
    case 1: // running
        state = 0;
        break;
    case 2: // paused
        state = 0;
        break;
    case 3:
        state = 0;
        break;
    }
}

void Metronome::stop_reset_time()
{
    switch (state)
    {
    case 0: // resetted (0)
        state = 2;
        break;
    case 1: // running
        state = 3;
        break;
    case 2: // paused
        state = 3;
        break;
    case 3: // paused
        break;
    }
}
