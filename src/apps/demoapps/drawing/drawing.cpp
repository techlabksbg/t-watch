#include "drawing.h"
#include "../../../services/services.h"
#include "../../settingapps/wifimanager/wifimanager.h"


#define MAX_PTS 200

bool Drawing::create() {
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    current = new char[2];
    current[0] = 'A';
    current[1] = 0;
    label = styles.stdLabel(myScr, current);
    lv_obj_align(label, myScr, LV_ALIGN_IN_TOP_LEFT, 5,5);
    // This is not optimal, it should be freed, when the app is not in use...
    // ... this is however not trivial, since the upload is asynchronous.
    buffer  = new char[20+MAX_PTS*(5+1+3+1+3+1)];
    return true;
}

bool Drawing::destroy() {
    delete[] current;
    delete[] buffer;
    current = nullptr;
    return true;
}

bool Drawing::show() {
    if (WiFi.isConnected()) {
        pts = new pts_t[MAX_PTS];
        start_loop(20);
        drawing = false;   
    } else {
        // connect to wifi
        lv_async_call([](void *userdata) {
            Serial.println("Async connect to WiFi...");
            wifiManager.connect((App*)userdata);
        }, this);
        Serial.println("no connection... async call scheduled");
    }
    return true;
}

void Drawing::output_and_increment() {
    char * bufp = buffer;
    Serial.println(current);
    sprintf(bufp, "%llx %s\n", ESP.getEfuseMac(), current);
    while(*bufp) bufp++;
    for (int i=0; i<num_pts; i++) {
        sprintf(bufp, "%lu %d %d\n", pts[i].time, pts[i].x, pts[i].y);
        while(*bufp) bufp++;
    }

    httpPost(this, buffer, "http://www.tech-lab.ch/twatch/drawing/upload.php",nullptr);
    num_pts = 0;
    if (current[0]=='Z') {
        current[0]='0';
    } else if (current[0]=='9') {
        current[0] = 'a';
    } else if (current[0]=='z') {
        current[0] =  'A';
    } else {
        current[0]++;
    }
    delete[] buffer;
    lv_label_set_text(label, current);
}

void Drawing::loop() {
    uint16_t a,b;
    if (ttgo->touch->getPoint(a,b)) {
        finished = false;
        if (drawing) {
            ttgo->tft->drawLine(x,y,a,b,0xffff);
        } 
        ttgo->tft->fillCircle(a,b,2,0xfc00);
        x = a;
        y = b;
        drawing = true;
        if (startms==0) {
            startms = millis();
        }
        if (num_pts<MAX_PTS-1) {
            pts[num_pts++] = {millis()-startms,a,b};
        }
    } else {
        if (drawing) {
            last = millis();
            if (num_pts<MAX_PTS) {
                pts[num_pts++] = {millis()-startms, -1,-1};
            }
        }
        drawing = false;
    }
    if (!drawing && !finished && millis()-last>500) {
        finished = true;
        output_and_increment();
        lv_obj_invalidate(myScr);
        startms = 0;

    }
}

bool Drawing::hide() {
    stop_loop();
    delete[] pts;
    pts = nullptr;
    return true;
}
