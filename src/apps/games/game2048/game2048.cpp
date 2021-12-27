/**
 * @author Ivo Blöchliger
 */

#include "game2048.h"

const char* Game2048::powers[] = {"", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096", "8192"};
lv_color_t Game2048::colors[] = {
    LV_COLOR_MAKE(0,0,0),
    LV_COLOR_MAKE(50,50,50),
    LV_COLOR_MAKE(100,80,60),
    LV_COLOR_MAKE(200,120,80),
    LV_COLOR_MAKE(220,150,40),
    LV_COLOR_MAKE(200,200,40),  // 32
    LV_COLOR_MAKE(150,250,50),
    LV_COLOR_MAKE(50,250,50),
    LV_COLOR_MAKE(20,200,150),
    LV_COLOR_MAKE(20,150,200),     // 512
    LV_COLOR_MAKE(0,50,250),
    LV_COLOR_MAKE(200,200,0),     // 2028
    LV_COLOR_MAKE(255, 0,255),     
    LV_COLOR_MAKE(20,20,20),     // 8192
};

bool Game2048::create() {
    // General background to black
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    // Set up and register callback
    lv_obj_set_click(myScr, true);
    lv_obj_set_user_data(myScr, this);
    lv_obj_set_event_cb(myScr, [](lv_obj_t *obj, lv_event_t event) {
        static uint16_t x,y;
        if (event==LV_EVENT_PRESSING) { // This is a race condition... 
            ttgo->touch->getPoint(x,y);
        }
        if (event != LV_EVENT_SHORT_CLICKED) return;
        ((Game2048*)(obj->user_data))->click(x,y);
    });

    feld = new byte[16];
    for (int i=0; i<16; i++) {
        feld[i] = 0;
    }
    feld[rand()%16] = 1;
    feld[rand()%16] = 1;
    return true;
}

void Game2048::createObjs() {
    static lv_style_t tile_style;
    static lv_style_t label_style;
    // Style for tiles
    lv_style_init(&tile_style);
    lv_style_set_border_color(&tile_style, LV_STATE_DEFAULT, LV_COLOR_MAKE(100,100,100));
    lv_style_set_border_width(&tile_style, LV_STATE_DEFAULT, 1);
    lv_style_set_bg_color(&tile_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    // Style for tiles
    lv_style_init(&label_style);
    lv_style_set_text_color(&label_style, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &ubuntu_r_22);

    objs = new lv_obj_t*[16];
    labels = new lv_obj_t*[16];
    for (int i=0; i<16; i++) {
        objs[i] = lv_obj_create(myScr, nullptr);
        lv_obj_set_click(objs[i], false);
        lv_obj_set_size(objs[i], 60,60);
        lv_obj_add_style(objs[i], LV_STATE_DEFAULT, &tile_style);
        lv_obj_align(objs[i], nullptr, LV_ALIGN_IN_TOP_LEFT, (i%4)*60, (i/4)*60);

        labels[i] = lv_label_create(objs[i],NULL);
        lv_label_set_text(labels[i], "");
        lv_obj_add_style(labels[i], LV_OBJ_PART_MAIN, &label_style);
        lv_obj_align(labels[i], nullptr, LV_LABEL_ALIGN_CENTER, 0, 0);
    }
}

bool Game2048::show() {
    createObjs();
    showFeld();
    return true;
}
bool Game2048::hide() {
    delete[] objs;
    delete[] labels;
    objs = nullptr;
    return true;
}

bool Game2048::destroy() {
    delete[] feld;
    feld = nullptr;
    return true;
}



void Game2048::setFeld(int x, int y, byte value) {
    int i = x+y*4;
    if (i<0 || i>15) return;
    feld[i] = value;
}
byte Game2048::getFeld(int x, int y) {
    int i = x+y*4;
    if (i<0 || i>15) return 0;
    return feld[i];
}
void Game2048::showFeld() {
    for (int i=0; i<16; i++) {
        lv_obj_set_style_local_bg_color(objs[i], LV_BTN_PART_MAIN, LV_STATE_DEFAULT, colors[feld[i]]);
        lv_label_set_text(labels[i], powers[feld[i]]);
        lv_obj_align(labels[i], nullptr, LV_ALIGN_CENTER, 0, 0);
    }
}

bool on(int x, int y) {
    return x>=0 && x<4 && y>=0 && y<4;
}

void Game2048::click(int x, int y) {
    Serial.printf("Touch at %d, %d\n", x,y);
    int vx, vy; // Richtung in der geschoben wird
    int px, py; // Richtung rechtwinklig dazu
    int ax, ay; // Startpunkt 
    bool moved = false;
    // perp: (-vy, vx)
    if (x>y) { // top right
        if (x<240-y) { // click top (move up)
            Serial.println("top");
            vx = 0;
            vy = 1;
            px = 1;
            py = 0;
            ax = 0;
            ay = 0;
        } else { // click right 
            Serial.println("right");
            vx = -1;
            vy = 0;
            px = 0;
            py = 1;
            ax = 3;
            ay = 0;
        }
    } else { // bottom left
        if (x<240-y) { // click left
            Serial.println("left");
            vx = 1;
            vy = 0;
            px = 0;
            py = 1;
            ax = 0;
            ay = 0;
        } else { // blick bottom
            Serial.println("bottom");
            vx = 0;
            vy = -1;
            px = 1;
            py = 0;
            ax = 0;
            ay = 3;
        }
    }
    
    for (int row=0; row<4; row++) {
        int bx = ax+row*px;
        int by = ay+row*py;
        // Compact everything   
        int cx = bx;
        int cy = by;
        for (int j=0; j<4; j++) {
            if (getFeld(bx,by)!=0) {
                bx+=vx;
                by+=vy;
            } else if (getFeld(cx,cy)!=0) { // non-empty case
                if (cx!=bx || cy!=by) { // move it to start
                    moved = true;
                    setFeld(bx,by, getFeld(cx,cy));
                    setFeld(cx,cy, 0);
                    bx+=vx;
                    by+=vy;
                }
            }
            cx+=vx;
            cy+=vy;
        }
        // Combine if any
        bx = ax+row*px;
        by = ay+row*py;
        for (int j=0; j<3; j++) {
            if (getFeld(bx,by)>0 && getFeld(bx,by)==getFeld(bx+vx, by+vy)) {
                moved = true;
                Serial.printf("  Join on %d,%d and %d,%d\n", bx,by, bx+vx, by+vy);
                setFeld(bx,by, getFeld(bx,by)+1);
                int cx = bx+vx;
                int cy = by+vy;
                //Serial.printf("  Shifting on %d,%d\n",cx,cy);
                for (int k=j+1; k<3; k++) {
                    setFeld(cx,cy, getFeld(cx+vx, cy+vy));
                    cx += vx;
                    cy += vy;
                }
                Serial.printf("    Setting 0 on %d,%d\n",cx,cy);
                setFeld(cx,cy, 0);
            }
            bx += vx;
            by += vy;
        }
    }
    // count empty tiles
    int c = 0;
    for (int i=0; i<16; i++) {
        if (feld[i]==0) c++;
    }
    // No more tiles? Restart!
    if (c==0) {
        for (int i=0; i<16; i++) {
            feld[i] = 0;
        }
        feld[0] = 1;
    } else if (moved) {  // only if something actually moved, add new tiles
        int n = rand()%2+1;
        if (n>c) n = c;
        while (n>0) {
            int cc = 0;
            int r = rand()%c+1;
            for (int i=0;i<16; i++) {
                if (feld[i]==0) cc++;
                if (cc==r) {
                    feld[i] = rand()%2+1;
                    c--;
                    n--;
                    break;
                }
            }
        }
    }
    showFeld();
}