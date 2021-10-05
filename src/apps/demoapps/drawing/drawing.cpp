#include "drawing.h"

#define MAX_PTS 200

bool Drawing::create() {
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    current = new char[2];
    current[0] = 'A';
    current[1] = 0;
    label = styles.stdLabel(myScr, current);
    lv_obj_align(label, myScr, LV_ALIGN_IN_TOP_LEFT, 5,5);
    return true;
}

bool Drawing::destroy() {
    delete[] current;
    current = nullptr;
    return true;
}

bool Drawing::show() {
    pts = new pts_t[MAX_PTS];
    start_loop(20);
    drawing = false;
    return true;
}

void Drawing::output_and_increment() {
    Serial.println(current);
    for (int i=0; i<num_pts; i++) {
        Serial.printf("%lu %d %d\n", pts[i].time, pts[i].x, pts[i].y);    
    }
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
