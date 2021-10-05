#include "drawing.h"

bool Drawing::create() {
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    current = new char[2];
    current[0] = 'A';
    current[1] = 0;
    label = styles.stdLabel(myScr, current);
    lv_obj_align(label, myScr, LV_ALIGN_IN_TOP_LEFT, 5,5);
    return true;
}

bool Drawing::show() {
    start_loop(20);
    drawing = false;
    return true;
}

void Drawing::loop() {
    uint16_t a,b;
    if (ttgo->touch->getPoint(a,b)) {
        if (drawing) {
            ttgo->tft->drawLine(x,y,a,b,0xffff);
        } 
        ttgo->tft->fillCircle(a,b,2,0xfc00);
        x = a;
        y = b;
        drawing = true;
    } else {
        if (drawing) {
            last = millis();
        }
        drawing = false;
    }
    if (!drawing && millis()-last>500) {
        last = millis()+1000000L;
        lv_obj_invalidate(myScr);
    }
}

bool Drawing::hide() {
    stop_loop();
    return true;
}
