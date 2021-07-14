/**
 * @author Ivo Blöchliger
 */


#pragma once

#include "../../app.h"

LV_IMG_DECLARE(sysinfoicon);


class SysInfo : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &sysinfoicon; }
    const char * getName() { return "SysInfo"; }

    lv_obj_t* page;
    lv_obj_t* label;
    lv_obj_t* quitButton;
    lv_obj_t* quitLabel;
    lv_task_t* task;  

    private:
    static SysInfo* self;

   

    static void update(struct _lv_task_t *data) {
        int batLevel = ttgo->power->getBattPercentage();
        bool batCharging = ttgo->power->isChargeing();
        float batTemp = ttgo->power->getTemp();
        int freeHeap =  ESP.getFreeHeap();
        //int ramTotal = ESP.
        uint8_t mac[6];
        esp_read_mac(mac, ESP_MAC_WIFI_STA);
        //esp_chip_info_t chipInfo;
        //esp_chip_info(&chipInfo);
        
        int spiffsTotal = SPIFFS.totalBytes();
        int spiffsUsed = SPIFFS.usedBytes();

        Accel accel;
        ttgo->bma->getAccel(accel);
        
        
        char buf[200];
        sprintf(buf, "%s%s %d%% %.1fC\n%dkB RAM free\n%02x:%02x:%02x:%02x:%02x:%02x\n%s %dkB/%dkB\ng: %d, %d, %d\nSteps: %d", 
            batLevel<5 ? LV_SYMBOL_BATTERY_EMPTY : 
            (batLevel<35 ? LV_SYMBOL_BATTERY_1 : 
            (batLevel<65 ? LV_SYMBOL_BATTERY_2 :
                (batLevel<95 ? LV_SYMBOL_BATTERY_3 :LV_SYMBOL_BATTERY_FULL))),
            batCharging ? LV_SYMBOL_CHARGE : "",
            batLevel,
            batTemp,
            freeHeap/1024,
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
            LV_SYMBOL_SD_CARD, spiffsUsed/1024, spiffsTotal/1024,
            accel.x, accel.y, accel.z,
            ttgo->bma->getCounter()
        );
        lv_label_set_text(SysInfo::self->label, buf);
    }
};
