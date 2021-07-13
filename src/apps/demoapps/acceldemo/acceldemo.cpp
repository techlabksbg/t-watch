/**
 * @author Ivo Blöchliger
 */

#include "acceldemo.h"

#define R 8


bool AccelDemo::create() {
    self = this;
    register_for_swipe_up(myScr);
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    return true;
}





bool AccelDemo::show() {
    if (demoTask==nullptr) {
        Serial.println("AccelDemo::show() create task");
        demoTask = lv_task_create(_demoLoop, 20, LV_TASK_PRIO_LOWEST, NULL);
    }
    return true;
}

bool AccelDemo::hide() {
    if (demoTask!=nullptr) {
        lv_task_del(demoTask);
        demoTask = nullptr;
    }
    return true;
}


void AccelDemo::demoLoop() {
    ttgo->tft->fillEllipse(x,y,R,R, 0);
    Accel accel;
    ttgo->bma->getAccel(accel);
    fy -= accel.x/1000.0;
    fx += accel.y/1000.0;
    x+=fx;
    y+=fy;
    if (x<R) {
        x = R;
        fx = -fx*0.9;
    }
    if (x>239-R) {
        x = 239-R;
        fx = -fx*0.9;
    }
    if (y<R) {
        y = R;
        fy = -fy*0.9;
    }
    if (y>239-R) {
        y=239-R;
        fy = -fy*0.9;
    }
    ttgo->tft->fillEllipse(x,y,R,R, 0xfff);
}


AccelDemo* AccelDemo::self = nullptr;