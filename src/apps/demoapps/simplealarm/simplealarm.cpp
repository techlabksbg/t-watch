/**
 * @author Ivo Blöchliger
 */

#include "simplealarm.h"


bool SimpleAlarm::create() {

    

    _lv_style_get_color(&styles.background,  LV_STYLE_BG_COLOR | (LV_STATE_DEFAULT << LV_STYLE_STATE_POS), &defaultBGColor);


    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    

    hoursSlider = lv_slider_create(myScr, NULL);
    lv_obj_set_width(hoursSlider, 200);
    lv_obj_align(hoursSlider, myScr, LV_ALIGN_IN_TOP_MID, 0, 40);
    lv_slider_set_range(hoursSlider, 0, 24);
    lv_slider_set_value(hoursSlider, hours, LV_ANIM_OFF);
    lv_obj_set_user_data(hoursSlider, this);
    lv_obj_set_event_cb(hoursSlider, slider_cb);

    minutesSlider = lv_slider_create(myScr, NULL);
    lv_obj_set_width(minutesSlider, 200);
    lv_obj_align(minutesSlider, hoursSlider, LV_ALIGN_OUT_BOTTOM_MID, 0, 40);
    lv_slider_set_range(minutesSlider, 0, 59);
    lv_slider_set_value(minutesSlider, minutes, LV_ANIM_OFF);
    lv_obj_set_user_data(minutesSlider, this);
    lv_obj_set_event_cb(minutesSlider, slider_cb);

    char buf[10];
    sprintf(buf, "%02d:%02d", hours, minutes);
    timeLabel = styles.stdTitle(myScr, buf);
    lv_obj_align(timeLabel, minutesSlider, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t* quit = styles.stdButton(myScr, LV_SYMBOL_CLOSE);
    lv_obj_set_width(quit, 80);
    register_for_hide_on_click(quit);
    lv_obj_t* set = styles.stdButton(myScr, LV_SYMBOL_OK);
    lv_obj_set_width(set, 80);
    lv_obj_set_user_data(set, this);
    lv_obj_set_event_cb(set, setAlarm_cb);

    lv_obj_align(set, myScr, LV_ALIGN_IN_BOTTOM_LEFT, 20, -20);
    lv_obj_align(quit, myScr, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -20);

    return true;
}

bool SimpleAlarm::show() {
    if (ttgo->rtc->alarmActive()) {
        RTC_Alarm alarm = ttgo->rtc->getAlarm();
        hours = alarm.hour;
        minutes = alarm.minute;
    } else {
        time_t now;
        struct tm  info;
        time(&now);
        localtime_r(&now, &info);

        minutes = info.tm_min+1;
        hours = (info.tm_hour+(minutes>59 ? 1 :0)) % 24;
        minutes = minutes % 60;

    }
    lv_slider_set_value(hoursSlider, hours, LV_ANIM_OFF);
    lv_slider_set_value(minutesSlider, minutes, LV_ANIM_OFF);
    char buf[10];
    sprintf(buf, "%02d:%02d", hours, minutes);
    lv_label_set_text(timeLabel, buf);
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, defaultBGColor);
    return true;
}

bool SimpleAlarm::hide() {
    alarmCounter = 0;
    
    if (alarmLoopTask!=nullptr) {
        lv_task_del(alarmLoopTask);
        alarmLoopTask = nullptr;
    }

    return true;
}

void SimpleAlarm::processAlarm() {
    Serial.printf("SimpleAlarm::processAlarm()");
    if (alarmLoopTask == nullptr) {
        Serial.println("SimpleAlarm::processAlarm() creating Loop task");
        alarmLoopTask = lv_task_create(alarmLoop, 1000, LV_TASK_PRIO_LOWEST, this);
    }
}