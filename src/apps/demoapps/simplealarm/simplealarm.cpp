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
    register_lv_event_callback(hoursSlider);

    minutesSlider = lv_slider_create(myScr, NULL);
    lv_obj_set_width(minutesSlider, 200);
    lv_obj_align(minutesSlider, hoursSlider, LV_ALIGN_OUT_BOTTOM_MID, 0, 40);
    lv_slider_set_range(minutesSlider, 0, 59);
    lv_slider_set_value(minutesSlider, minutes, LV_ANIM_OFF);
    register_lv_event_callback(minutesSlider);

    char buf[10];
    sprintf(buf, "%02d:%02d", hours, minutes);
    timeLabel = styles.stdTitle(myScr, buf);
    lv_obj_align(timeLabel, minutesSlider, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t* quit = styles.stdButton(myScr, LV_SYMBOL_CLOSE);
    lv_obj_set_width(quit, 80);
    register_for_hide_on_click(quit);

    setButton = styles.stdButton(myScr, LV_SYMBOL_OK);
    lv_obj_set_width(setButton, 80);
    register_lv_event_callback(setButton);

    lv_obj_align(setButton, myScr, LV_ALIGN_IN_BOTTOM_LEFT, 20, -20);
    lv_obj_align(quit, myScr, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -20);

    return true;
}

bool SimpleAlarm::show() {
    if (ttgo->rtc->alarmActive()) {
        RTC_Alarm alarm = ttgo->rtc->getAlarm();
        hours = alarm.hour;
        minutes = alarm.minute;
    } else {
        RTC_Date r = ttgo->rtc->getDateTime();
        minutes = r.minute+1;
        hours = (r.hour+(minutes>59 ? 1 :0)) % 24;
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
    stop_loop();
    return true;
}

void SimpleAlarm::processAlarm() {
    Serial.printf("SimpleAlarm::processAlarm()");
    start_loop(1000);
}

void SimpleAlarm::loop() {
    if (alarmCounter % 2 == 1) {
        lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, defaultBGColor);
    } else {
        lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(255,255,0));
        if (alarmCounter % 4 == 0) {
            ttgo->motor->onec();
        }
    }
    if (alarmCounter==31) {
        hide_myself();
    }
    alarmCounter++;
}

void SimpleAlarm::lv_event_callback(lv_obj_t* obj, lv_event_t event) {
    static char buf[10];
    if ((obj == hoursSlider || obj==minutesSlider) && event == LV_EVENT_VALUE_CHANGED) {
        hours = lv_slider_get_value(hoursSlider);
        minutes = lv_slider_get_value(minutesSlider);
        sprintf(buf, "%02d:%02d", hours, minutes);
        lv_label_set_text(timeLabel, buf);
        return;
    }
    if (obj == setButton && event==LV_EVENT_CLICKED) {
        Serial.printf("Setting alarm to %02d:%02d\n", hours, minutes);
        setAlarm(hours, minutes);
        Serial.println("I'm outa here...");
        hide_myself();
    }

}