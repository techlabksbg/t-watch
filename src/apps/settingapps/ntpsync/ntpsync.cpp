#include "ntpsync.h"


bool NTP_Sync::create() {
    self = this;
    Serial.println("NTPSYNC::create() start");
    lv_obj_t* bg = styles.stdBgImage(myScr);
    title = styles.stdTitle(bg, "NTP Sync\nNot connected");
    lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0,15);

    Serial.println("Title done");
    connect = styles.stdButton(bg, "Connect");
    lv_obj_align(connect, title, LV_ALIGN_OUT_BOTTOM_MID, 0,20);
    lv_obj_set_event_cb(connect, connect_cb);

    Serial.println("Connect button done");
    quit = styles.stdButton(bg, "Quit");
    lv_obj_align(quit, connect, LV_ALIGN_OUT_BOTTOM_MID, 0,20);
    App::register_for_hide_on_click(quit);

    Serial.println("NTPSYNC::create() end");

    return true;
}

bool NTP_Sync::show() {
    if (WiFi.isConnected()) {
        sntp_set_time_sync_notification_cb(ntp_cb);
        configTzTime((*configJson)["tz"], "pool.ntp.org");
        lv_label_set_text(title,"NTP Sync\nDone!");
    } else {
        lv_label_set_text(title,"NTP Sync\nNot connected!");
    }
    return true;
}

NTP_Sync* NTP_Sync::self = nullptr;