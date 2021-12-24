#include "ble_demo.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>


// Generate you own at https://www.uuidgenerator.net/
#define SERVICE_UUID_1ST "2e8dd61f-2b81-4932-8de4-490349074256"
#define SERVICE_UUID_2ND "2e8dd61f-2b81-4932-8de4-490349074257"
#define BLE_CHARACTERISTIC_UUID "2e8dd61f-2b81-4932-8de4-490349074255"

#define BLE_SERVER_NAME_1ST "BLE_DEMO_1ST"
#define BLE_SERVER_NAME_2ND "BLE_DEMO_2ND"

void BLE_Demo::setup_BLE_Client() {
    BLEDevice::init("");
    BLEClient* pClient = BLEDevice::createClient();
    
    
}
void BLE_Demo::setup_BLE_Server(bool first) {
    BLEDevice::init(first ? BLE_SERVER_NAME_1ST : BLE_SERVER_NAME_2ND);
    bleServer = BLEDevice::createServer();
    bleService = bleServer->createService(first ? SERVICE_UUID_1ST : SERVICE_UUID_2ND);
    bleCharacteristic = bleService->createCharacteristic(
                                         BLE_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

}



bool BLE_Demo::create() {         
    bleCharacteristic = new BLECharacteristic(BLE_CHARACTERISTIC_UUID);
    bleDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x1585)); // Random value?
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
}

void BLE_Demo::loop() {

}
