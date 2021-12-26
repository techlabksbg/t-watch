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
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));
//    log_e("esp_bluedroid_init");
//    esp_bluedroid_init();
    for (int i=0; i<2; i++) {
        scores[i] = 0;
    }
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
            ((RpslsGame*)(lv_obj_get_user_data(bg)))->click(x,y,false);
        }
        if (event==LV_EVENT_LONG_PRESSED) {
             Serial.println("Long-pressed in CB");
            ((RpslsGame*)(lv_obj_get_user_data(bg)))->click(x,y,true);
        }
    });

    for (int i=0; i<2; i++) {
        labels[i] = styles.stdTitle(bg, "0");
   
    }
    updateLabels();
    /*Serial.println("bluedroid init...");
    
    Serial.println("BluetoothSerial init");
    SerialBT = new BluetoothSerial(); */
    start_loop(100);
    return true;
}

void RpslsGame::loop() {
    switch (zustand) {
        case INIT:
            Serial.println("SerialBT.begin()");
            SerialBT.begin("RpslsClient", true);
            if (SerialBT.connect("RpslsServer")) {
                zustand = CONNECTED_AS_CLIENT;
                Serial.println("Connected as client to Server");
            } else {
                SerialBT.end();
                SerialBT.begin("RpslsServer");
                zustand = WAITING_FOR_CLIENT;
                Serial.println("No server, serving myself and waiting for Client");
            }
            break;
        case WAITING_FOR_CLIENT:
            if (SerialBT.connected()) {
                zustand = CONNECTED_AS_SERVER;
                Serial.println("Connected as Server");
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
    int cx [] = {119, 199, 149, 89, 39};
    int cy [] = {30, 95,209,209, 85};

    if (active!=nullptr) {
        delete active;
        active = nullptr;
    }
    if (long_pressed && abs (x-119) <20 && abs(y-119) <20 ){
       scores[0]=0;
       scores[1]=0;
    }
    else if (! long_pressed) {
        if ((x-119)*(x-119)+(y-30)*(y-30)<(23*23)) {
            player_1 = 0;
        }
        if ((x-199)*(x-199)+(y-95)*(y-95)<(23*23)) {
            player_1 = 1;
        }
        if ((x-149)*(x-149)+(y-209)*(y-209)<(23*23)) {
            player_1 = 2;
        }
        if ((x-89)*(x-89)+(y-209)*(y-209)<(23*23)) {
            player_1 = 3;
        }
        if ((x-39)*(x-39)+(y-95)*(y-95)<(23*23)) {
            player_1 = 4;
        }
        int computer = random(5);
        Serial.println (computer);

        Serial.println (player_1);

        int dif = (computer + 5 - player_1)%5;

        if (dif == 0){
            Serial.println ("draw");
        }else {
            if (dif == 1|| dif ==3){
                scores[1] ++ ;        
            }else{
                scores[0] ++ ;
            }
        }
        active = new Overlay(bg,cx[computer],cy[computer],40, LV_COLOR_MAKE(250,100,50));
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
