/**
 * @author Ivo Blöchliger
 */

#include "launcher.h"


bool Launcher::create() {
    Serial.printf("buildTileView for %s\n", getName());
    static lv_style_t mainStyle;
    lv_style_init(&mainStyle);
    lv_style_set_radius(&mainStyle, LV_OBJ_PART_MAIN, 0);
    //lv_style_set_bg_color(&mainStyle, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
    lv_style_set_bg_opa(&mainStyle, LV_OBJ_PART_MAIN, LV_OPA_0);
    lv_style_set_border_width(&mainStyle, LV_OBJ_PART_MAIN, 0);
    lv_style_set_text_color(&mainStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
    //lv_style_set_image_recolor(&mainStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);

    Serial.printf("Number of tiles in %s is %d\n", getName(), numberOfApps);
    validPoints = new lv_point_t[numberOfApps];
    for (int i=0; i<numberOfApps; i++) {
        validPoints[i].x = i/2;
        validPoints[i].y = i%2;
    }

    tileView = lv_tileview_create(myScr, NULL);
    Serial.println("set_valid_positions");
    lv_tileview_set_valid_positions(tileView, validPoints, numberOfApps);

    static lv_style_t tvStyle;
    lv_style_copy(&tvStyle, &mainStyle);
    lv_style_set_bg_opa(&tvStyle, LV_OBJ_PART_MAIN, LV_OPA_COVER);
    lv_style_set_bg_color(&mainStyle, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0x04, 0x08, 0x20));
    lv_style_set_bg_grad_color(&tvStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x20, 0x08, 0x04));
    lv_style_set_bg_grad_dir(&tvStyle, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    Serial.println("add style");
    lv_obj_add_style(tileView, LV_OBJ_PART_MAIN, &tvStyle);
    lv_tileview_set_edge_flash(tileView, true);
    lv_obj_align(tileView, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_page_set_scrlbar_mode(tileView, LV_SCRLBAR_MODE_OFF);



    tiles = new lv_obj_t*[numberOfApps];
    for (int i=0; i<numberOfApps; i++) {
        Serial.printf("Creating tile %d\n", i);
        tiles[i] =  lv_cont_create(tileView, NULL);
        lv_obj_add_style(tiles[i], LV_OBJ_PART_MAIN, &mainStyle);
        lv_obj_set_size(tiles[i],120, 120);

        lv_obj_t *img = lv_img_create(tiles[i], NULL);
        lv_obj_add_style(img, LV_OBJ_PART_MAIN, &mainStyle);
        lv_img_set_src(img, apps[i]->getIcon());
        lv_obj_align(img, tiles[i], LV_ALIGN_CENTER, 0, 0);

        lv_obj_t *label = lv_label_create(tiles[i],  NULL);
        lv_obj_add_style(label, LV_OBJ_PART_MAIN, &mainStyle);
        lv_label_set_text(label, apps[i]->getName());
        lv_obj_align(label, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

        if (i%2==1) {
            lv_obj_align(tiles[i], tiles[i-1], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        } else if (i>0) {
            lv_obj_align(tiles[i], tiles[i-2], LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        }

        lv_tileview_add_element(tileView, tiles[i]);
        lv_obj_set_click(tiles[i], true);
        lv_obj_set_user_data(tiles[i],this);
        lv_obj_set_event_cb(tiles[i], tile_event_cb);
    }
    lv_obj_align(tileView, NULL, LV_ALIGN_CENTER, 0, 0);
    return true;
}

bool Launcher::show() {
    return true;
}
bool Launcher::hide() {
    return true;
}

// Normally, this app does not get destroyed...
bool Launcher::destroy() {
    if (tileView!=nullptr) { 
        lv_obj_del(tileView);
        tileView = nullptr;
        delete[] tiles;
        delete[] validPoints;
    }
    return true;
}


Launcher launcher("Launcher", (Launcher*) nullptr);
Launcher* Launcher::rootLauncher = &launcher;
App* Launcher::activeApp = nullptr;
App* Launcher::lastLauncher = nullptr;
App* Launcher::lastWatch = nullptr;
App* Launcher::lastApp = nullptr;
App* Launcher::sleepyApp = nullptr;
App* Launcher::alarmApp = nullptr;
std::function<void(void)> Launcher::rtcCallback = nullptr;