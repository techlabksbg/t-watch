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
    // React on clicks for this image
    lv_obj_set_click(bg, true);
    // Setup callback to click.
    lv_obj_set_event_cb(bg, [](lv_obj_t* bg, lv_event_t event) {
        static uint16_t x,y;
        if (event == LV_EVENT_PRESSED) {
            ttgo->touch->getPoint(x,y);
            return;
        }
        if (event == LV_EVENT_CLICKED) {
            Serial.println("clicked in CB");
            ((RpslsGame*)(lv_obj_get_user_data(bg)))->click(x,y);
        }
    });

    for (int i=0; i<2; i++) {
        scores[i] = 0;
        labels[i] = styles.stdTitle(bg, "0");
   
    }
    updateLabels();
    return true;
}

void RpslsGame::updateLabels() {
    for (int i=0; i<2; i++) {
        lv_label_set_text(labels[i], String(scores[i]).c_str());
        lv_obj_align(labels[i], nullptr, i==0 ? LV_ALIGN_IN_TOP_LEFT : LV_ALIGN_IN_TOP_RIGHT, 0,0);
    }
}

void RpslsGame::click(int x, int y) {
    if (active!=nullptr) {
        delete active;
        active = nullptr;
    }
    Serial.printf("click at %d,%d\n", x,y);
    active = new Overlay(bg,x,y,40, LV_COLOR_MAKE(250,100,50));
    scores[random(2)]++;
    updateLabels();
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
    // set transparency to 50%
    lv_style_set_bg_opa(style, LV_STATE_DEFAULT, 128);
    lv_style_set_bg_color(style, LV_STATE_DEFAULT, color);
    lv_style_set_border_width(style, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(overlay, LV_OBJ_PART_MAIN, style);
    lv_obj_set_size(overlay, 2*r, 2*r);
    lv_obj_align(overlay, nullptr, LV_ALIGN_IN_TOP_LEFT, cx-r, cy-r);
    lv_obj_set_click(overlay, false);
}


bool RpslsGame::hide() {
    for (int i=0; i<2; i++) {
        lv_obj_del(labels[i]);
    }
    if (active!=nullptr) {
        delete active;
        active = nullptr;
    }
    lv_obj_del(bg);
    return true;
}
bool RpslsGame::destroy() {
    return true;
}