/**
 * @author Ivo Blöchliger
 */

#include "rpslsgame.h"



bool RpslsGame::create() {
    return true;
}
bool RpslsGame::show() {
    bg = lv_img_create(myScr, NULL);  /*Create an image object*/
    lv_img_set_src(bg, (void *) &rpslsgamebg);
    lv_obj_align(bg, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_user_data(bg, this);
    // Setup callback to click.
    lv_obj_set_event_cb(bg, [](lv_obj_t* bg, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            uint16_t x,y;
            if (ttgo->touch->getPoint(x,y)) {
                ((RpslsGame*)(lv_obj_get_user_data(bg)))->click(x,y);
            }
        }
    });

    return true;
}

void RpslsGame::click(int x, int y) {
    if (active!=nullptr) {
        delete active;
        active = nullptr;
    }
    active = new Overlay(bg, x,y,40, LV_COLOR_MAKE(200,100,50));
}

RpslsGame::Overlay::~Overlay() {
    lv_obj_del(overlay);
    delete style;
}

RpslsGame::Overlay::Overlay(lv_obj_t* bg, int cx, int cy, int r, lv_color_t color) {
    overlay = lv_obj_create(bg, nullptr);
    style = new lv_style_t;
    lv_style_init(style);
    lv_style_set_radius(style, LV_STATE_DEFAULT, r);
    // set transparency to 25%
    lv_style_set_bg_opa(style, LV_STATE_DEFAULT, 64);
    lv_style_set_bg_color(style, LV_STATE_DEFAULT, color);
    lv_obj_set_size(overlay, 2*r, 2*r);
    lv_obj_align(overlay, nullptr, LV_ALIGN_IN_TOP_LEFT, cx+r, cy+r);
    lv_obj_set_click(overlay, false);
}


bool RpslsGame::hide() {
    lv_obj_del(bg);
    if (active!=nullptr) {
        delete active;
        active = nullptr;
    }
    return true;
}
bool RpslsGame::destroy() {
    return true;
}