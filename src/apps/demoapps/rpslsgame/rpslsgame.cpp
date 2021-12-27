/**
 * @author Ivo Blöchliger
 */

#include "rpslsgame.h"
#include "esp_bt_main.h"
#include "BluetoothSerial.h"

#include "esp_bt.h"


BluetoothSerial SerialBT;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

bool RpslsGame::create() {
    //ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));
//    log_e("esp_bluedroid_init");
//    esp_bluedroid_init();
    for (int i=0; i<2; i++) {
        scores[i] = 0;
    }
    return true;
}

void RpslsGame::lv_event_callback(lv_obj_t* obj, lv_event_t event) {
    static uint16_t x,y;
    if (obj==bg) {
        if (event == LV_EVENT_PRESSED) {
            ttgo->touch->getPoint(x,y);
            return;
        }
        if (event == LV_EVENT_CLICKED) {
            click(x,y,false);
        }
        if (event==LV_EVENT_LONG_PRESSED) {
            click(x,y,true);
        }
    }
    if (obj==serverBT || obj==clientBT || obj==soloBT) {
        if (event == LV_EVENT_CLICKED) {
            if (obj==serverBT) {
                SerialBT.begin("RpslsServer");
                zustand = WAITING_FOR_CLIENT;
                start_loop(100);
            }
            if (obj==clientBT) {
                SerialBT.begin("RpslsClient", true);
                if (SerialBT.connect("RpslsServer")) {
                    zustand = CONNECTED_AS_CLIENT;
                    Serial.println("Connected as client to Server");
                } else {
                    zustand = WAITING_FOR_SERVER;
                }
                start_loop(100);
            }
            if (obj==soloBT) {
                zustand = HAN_STYLE;
            }
            lv_obj_del(serverBT);
            lv_obj_del(clientBT);
            lv_obj_del(soloBT);
            serverBT = nullptr;
            clientBT = nullptr;
            soloBT = nullptr;
            startGame();
        }
    }
}

void RpslsGame::startGame() {
    // React on clicks for this image
    lv_obj_set_click(bg, true);
    // Setup callback to click.
    register_lv_event_callback(bg);
    for (int i=0; i<2; i++) {
        labels[i] = styles.stdTitle(bg, "0");
    }
    updateLabels();
    start_loop(100);
}

bool RpslsGame::show() {
    bg = lv_img_create(myScr, NULL);  /*Create an image object*/
    lv_img_set_src(bg, (void *) &rpslsgamebg);
    lv_obj_align(bg, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_user_data(bg, this);

    serverBT = styles.stdButton(bg, "Server");
    clientBT = styles.stdButton(bg, "Client");
    soloBT = styles.stdButton(bg, "Solo");

    lv_obj_align(serverBT, bg, LV_ALIGN_IN_TOP_MID, 0, 50);
    lv_obj_align(clientBT, bg, LV_ALIGN_IN_TOP_MID, 0, 100);
    lv_obj_align(soloBT, bg, LV_ALIGN_IN_TOP_MID, 0, 150);

    register_lv_event_callback(serverBT);
    register_lv_event_callback(clientBT);
    register_lv_event_callback(soloBT);
    
    return true;
}

void RpslsGame::loop() {
    switch (zustand) {
        case INIT:
        case HAN_STYLE:
            break;
        case WAITING_FOR_SERVER:
            if (SerialBT.connected(100)) {
                zustand = CONNECTED_AS_CLIENT;
            }
            break;
        case WAITING_FOR_CLIENT:
            if (SerialBT.hasClient()) {
                zustand = CONNECTED_AS_SERVER;
                Serial.println("Connected as Server");
            }
            break;
        case CONNECTED_AS_SERVER:
            if (!SerialBT.hasClient()) {
                hide_myself();
            }
            if (SerialBT.available()) {
                String msg  = SerialBT.readString();
                Serial.printf("Server got >>%s<< of len=%d\n", msg.c_str(), msg.length());
            }
            break;
        case CONNECTED_AS_CLIENT:
            if (!SerialBT.connected(10)) {
                hide_myself();
            }
            if (SerialBT.available()) {
                String msg  = SerialBT.readString();
                Serial.printf("Client got >>%s<< of len=%d\n", msg.c_str(), msg.length());
            }
            break;
    }
}

void RpslsGame::updateLabels() {
    for (int i=0; i<2; i++) {
        lv_label_set_text(labels[i], String(scores[i]).c_str());
        lv_obj_align(labels[i], nullptr, i==0 ? LV_ALIGN_IN_TOP_LEFT : LV_ALIGN_IN_TOP_RIGHT, 0,0);
    }
}

void RpslsGame::click(int x, int y, bool long_pressed) {
    int player_1 = 0;
    int cx [] = {119, 204, 170, 70, 36};
    int cy [] = {38, 99, 200, 200, 99};

    if (active!=nullptr) {
        delete active;
        active = nullptr;
    }
    if (long_pressed && abs (x-119) <20 && abs(y-119) <20 ){
       scores[0]=0;
       scores[1]=0;
    }
    else if (! long_pressed) {
        player_1=-1;
        for (int i=0; i<5; i++) {
            if ((x-cx[i])*(x-cx[i])+ (y-cy[i])*(y-cy[i])<23*23) {
                player_1 = i;
                break;
            }
        }
        if (player_1!=-1) {
            int computer = random(5);
            Serial.printf("computer=%d\n", computer);
            Serial.printf("player=%d\n", player_1);
            SerialBT.printf("%d\r\n", player_1);
            int diff = (computer + 5 - player_1)%5;
            if (diff == 0){
                Serial.println ("draw");
            } else {
                if (diff == 1|| diff ==3){
                    scores[1]++ ;        
                } else {
                    scores[0]++ ;
                }
            }
            active = new Overlay(bg,cx[computer],cy[computer],40, LV_COLOR_MAKE(250,100,50));
        }
    }
    
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
    SerialBT.end();
    zustand = INIT;
    stop_loop();
    return true;
    
}
bool RpslsGame::destroy() {
    return true;
}
