#include "drawing.h"

bool Drawing::create() {
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    return true;
}

bool Drawing::show() {
    if (task==nullptr) {
        task = lv_task_create(task_cb, 20, LV_TASK_PRIO_LOWEST, this);
    }
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
    if (task!=nullptr) {
        lv_task_del(task);
        task = nullptr;
    }
    return true;
}
