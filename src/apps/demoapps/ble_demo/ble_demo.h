/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(ble_demoicon);



class BLE_Demo : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();
    
    void* getIcon() { return (void*) &ble_demoicon; }
    const char * getName() { return "BLE Link"; }

    
    virtual void loop();

    private:
    lv_obj_t* btn1=nullptr;
    lv_obj_t* btn2=nullptr;
    lv_obj_t* label=nullptr;

    void buttonClicked(lv_obj_t* obj);
    static void buttonClickedStatic(lv_obj_t* obj, lv_event_t event);

    void setup_BLE_Client();
    void setup_BLE_Server();

    BLECharacteristic* bleCharacteristic;
    BLEDescriptor* bleDescriptor;
    BLEServer* bleServer;
    BLEService* bleService;
    
};
