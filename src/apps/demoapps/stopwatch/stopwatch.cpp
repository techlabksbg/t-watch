#include "stopwatch.h"

bool Stopwatch::create()
{
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);

    stopwatch = lv_label_create(myScr, NULL);
    lv_obj_add_style(stopwatch, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_label_set_text(stopwatch, "Stopwatch");
    lv_label_set_align(stopwatch, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(stopwatch, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);

    time_passed = lv_label_create(myScr, NULL);
    lv_obj_add_style(time_passed, LV_OBJ_PART_MAIN, &styles.textLabel);
    lv_label_set_text(time_passed, "0:00");
    lv_label_set_align(time_passed, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(time_passed, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 50);

    start_stop_button = styles.stdButton(
        myScr, "Start", [](lv_obj_t *button, lv_event_t event)
        {
            if (event != LV_EVENT_SHORT_CLICKED)
                return;
            ((Stopwatch *)(button->user_data))->start_stop_time();
        },
        this);
    start_stop_label = lv_obj_get_child(start_stop_button, NULL);
    lv_label_set_align(start_stop_label, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(start_stop_button, myScr, LV_ALIGN_IN_TOP_LEFT, 20, 75);

    Serial.println("Creating Stopwatch");
    return true;
}

bool Stopwatch::show()
{
    Serial.println("Showing Stopwatch");
    loop();
    start_loop(10);
    return true;
}

void Stopwatch::loop()
{
    time_t time_now;
    struct tm info;
    char buf[64];
    time(&time_now);
    time_now -= start_stop_t;
    localtime_r(&time_now, &info);
    strftime(buf, sizeof(buf), "%M:%S", &info);
    if (state == 1)
    {
        lv_label_set_text(time_passed, buf);
    }
}

bool Stopwatch::hide()
{
    stop_loop();
    return true;
}

void Stopwatch::start_stop_time()
{
    struct tm info;
    switch (state)
    {
    case 0: // resetted (0)
        time(&start_stop_t);
        localtime_r(&start_stop_t, &info);
        lv_label_set_text(start_stop_label, "Stop/Pause");
        state = 1;
        break;
    case 1: // running
        lv_label_set_text(start_stop_label, "Reset");
        state = 2;
        break;
    case 2: // paused
        lv_label_set_text(start_stop_label, "Start");
        time_t reset_time = 0;
        char buf[64];
        localtime_r(&reset_time, &info);
        strftime(buf, sizeof(buf), "%M:%S", &info);

        lv_label_set_text(time_passed, buf);
        state = 0;
        break;
    }
}