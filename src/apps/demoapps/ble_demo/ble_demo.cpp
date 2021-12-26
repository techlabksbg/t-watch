#include "ble_demo.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>


// Generate you own at https://www.uuidgenerator.net/
#define SERVICE_UUID "2e8dd61f-2b81-4932-8de4-490349074256"
#define CHARACTERISTIC_UUID "2e8dd61f-2b81-4932-8de4-490349074255"

#define BLE_NAME "BLE_DEMO"


bool BLE_Demo::create() {         
    return true;
}

void BLE_Demo::buttonClickedStatic(lv_obj_t* obj, lv_event_t ev) {
    if (ev == LV_EVENT_SHORT_CLICKED) {
        ((BLE_Demo*)(obj->user_data))->buttonClicked(obj);
    }
}

bool BLE_Demo::show() { 
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    btn1 = styles.stdButton(myScr, "A", buttonClickedStatic, this);
    btn2 = styles.stdButton(myScr, "B", buttonClickedStatic, this);
    label = styles.stdLabel(myScr, "Not connected");
    lv_obj_align(btn1, myScr, LV_ALIGN_IN_TOP_MID, 0, 20);
    lv_obj_align(btn2, btn1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_align(label, btn2, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    
    bleLink = BLE_Link::getInstance();
    bleLink->begin(BLE_NAME, SERVICE_UUID, CHARACTERISTIC_UUID, this);

    return true;
}

bool BLE_Demo::hide() { 
    lv_obj_del(label);
    lv_obj_del(btn2);
    lv_obj_del(btn1);
    label = nullptr;
    btn2 = nullptr;
    btn1 = nullptr;
    return true;
}

bool BLE_Demo::destroy() { return true;}

void BLE_Demo::buttonClicked(lv_obj_t* obj) {
    int b = obj==btn1 ? 0 : 1;
    Serial.printf("Button %d clickec\n", b);
    lv_label_set_text(label,  b==0 ? "Pressed 0" : "Pressed 1");

    std::string msg = "button ";
    msg += b;
    if (bleLink->state==BLE_Link::CONNECTED_AS_SERVER) {
        msg += " on server";
    } else if (bleLink->state==BLE_Link::CONNECTED_AS_CLIENT) {
        msg += " on client";
    }
    bleLink->write(msg);
}

void BLE_Demo::bleLinkNotify() {
    Serial.println("BLE_Demo got notified...");
    Serial.print("read is now: ");
    Serial.println(bleLink->read().c_str());
}

void BLE_Demo::loop() {

}
