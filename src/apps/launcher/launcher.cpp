/**
 * @author Ivo Blöchliger
 */

#include "launcher.h"


bool Launcher::create() {
    page = styles.stdPage(myScr);

    Serial.printf("Number of tiles in %s is %d\n", getName(), numberOfApps);
    



    tiles = new lv_obj_t*[numberOfApps];
    for (int i=0; i<numberOfApps; i++) {
        Serial.printf("Creating tile %d\n", i);
        tiles[i] =  lv_cont_create(page, NULL);
        lv_obj_add_style(tiles[i], LV_OBJ_PART_MAIN, &styles.appTileStyle);
        lv_obj_set_size(tiles[i],120, 120);

        lv_obj_t *img = lv_img_create(tiles[i], NULL);
        lv_obj_add_style(img, LV_OBJ_PART_MAIN, &styles.appTileStyle);
        lv_img_set_src(img, apps[i]->getIcon());
        lv_obj_align(img, tiles[i], LV_ALIGN_CENTER, 0, 0);

        lv_obj_t *label = lv_label_create(tiles[i],  NULL);
        lv_obj_add_style(label, LV_OBJ_PART_MAIN, &styles.appTileStyle);
        lv_label_set_text(label, apps[i]->getName());
        lv_obj_align(label, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

        if (i%2==1) {
            lv_obj_align(tiles[i], tiles[i-1], LV_ALIGN_OUT_RIGHT_MID, 0, 0);
        } else if (i>0) {
            lv_obj_align(tiles[i], tiles[i-2], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        } else {
            lv_obj_align(tiles[0],NULL, LV_ALIGN_IN_TOP_LEFT, 0,0);
        }
        lv_page_glue_obj(tiles[i], true);
        lv_obj_set_click(tiles[i], true);
        lv_obj_set_user_data(tiles[i],this);
        lv_obj_set_event_cb(tiles[i], tile_event_cb);
    }
    lv_obj_align(page, NULL, LV_ALIGN_CENTER, 0, 0);
    return true;
}

bool Launcher::show() {
    Serial.printf("Launcher::show() of %s\n", getName());
    return true;
}
bool Launcher::hide() {
    Serial.printf("Launcher::hide() of %s\n", getName());
    return true;
}

// Normally, this app does not get destroyed...
bool Launcher::destroy() {
    if (page!=nullptr) { 
        lv_obj_del(page);
        page = nullptr;
        delete[] tiles;
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