/**
 * @author Ivo Blöchliger
 */


#include "bhfsg.h"

LV_IMG_DECLARE(bhfsgdigit00);
LV_IMG_DECLARE(bhfsgdigit01);
LV_IMG_DECLARE(bhfsgdigit10);
LV_IMG_DECLARE(bhfsgdigit11);
LV_IMG_DECLARE(bhfsgdigit20);
LV_IMG_DECLARE(bhfsgdigit21);


bool BhfSGWatch::create() {
    imgs = new lv_obj_t**[3];
    imgsrcs = new void**[3];
    for (int i=0; i<3; i++) {
        imgs[i] = new lv_obj_t*[6];
        imgsrcs[i] = new void*[2];
        for (int j=0; j<6; j++) {
            imgs[i][j] = nullptr;
        }
    }
    imgsrcs[0][0] = (void*) &bhfsgdigit00;
    imgsrcs[0][1] = (void*) &bhfsgdigit01;
    imgsrcs[1][0] = (void*) &bhfsgdigit10;
    imgsrcs[1][1] = (void*) &bhfsgdigit11;
    imgsrcs[2][0] = (void*) &bhfsgdigit20;
    imgsrcs[2][1] = (void*) &bhfsgdigit21;

    //Serial.println("set bg to black");
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0,0,0));

    for (int i=0; i<3; i++) {
        for (int j=0; j<(i==0?5:6); j++) {
            //Serial.println("img create");
            imgs[i][j] = lv_img_create(myScr, nullptr);
            //Serial.println("Set src");
            lv_img_set_src(imgs[i][j], imgsrcs[i][0]);
            //Serial.println("align");
            lv_obj_align(imgs[i][j],nullptr,LV_ALIGN_CENTER,j*40-(i==0?80:100),i*50-50);
        }
    }
    loop();
    return true;
}

void BhfSGWatch::loop() {
    time_t now;
    //Serial.println("loop");
    static struct tm last {0,0,0,0};
    struct tm  info;
    time(&now);
    localtime_r(&now, &info);
    int diff = info.tm_hour ^ last.tm_hour;
    //Serial.printf("hour diff=%d\n", diff);
    if (diff) {
        for (int j=0; j<5; j++) {
            if ((diff >> j) & 1) {
                lv_img_set_src(imgs[0][4-j], imgsrcs[0][(info.tm_hour >> j)&1]);
            }
        }
    }
    for (int i=1; i<3; i++) {
        diff = (i==1) ? info.tm_min ^ last.tm_min : info.tm_sec ^ last.tm_sec;
        int v = (i==1) ? info.tm_min : info.tm_sec;
        //Serial.printf("t=%d diff=%d (for v=%d)\n",i,diff,v);
        if(diff) {
            for (int j=0; j<6; j++) {
                if ((diff>>j) & 1) {
                    lv_img_set_src(imgs[i][5-j], imgsrcs[i][(v >> j)&1]);
                }
            }
        }
    }
    last = info;
}

bool BhfSGWatch::show() {
    start_loop(1000);
    return true;
}


bool BhfSGWatch::hide() {
    stop_loop();
    return true;
}


bool BhfSGWatch::destroy() {
    for (int i=0; i<3; i++) {
        delete[] imgs[i];
    }
    delete[] imgs;
    imgs = nullptr;

    return true;
}

