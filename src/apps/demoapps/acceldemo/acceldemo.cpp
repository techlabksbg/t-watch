/**
 * @author Ivo Blöchliger
 */

#include "acceldemo.h"

#define R 8


bool AccelDemo::create() {
    register_for_swipe_up(myScr);
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    return true;
}


bool AccelDemo::show() {
    start_loop(20);
    return true;
}

bool AccelDemo::hide() {
    stop_loop();
    return true;
}


void AccelDemo::loop() {
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