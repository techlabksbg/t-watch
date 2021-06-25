#include "os.h"
//#include "systemgui/wifigui.h"

#include "apps/launcher/launcher.h"
#include "apps/settingapps/wifimanager/wifimanager.h"


void os_loop()
{
    bool  rlst;
    uint8_t data;
    if (xQueueReceive(g_event_queue_handle, &data, 5 / portTICK_RATE_MS) == pdPASS) {
        switch (data) {
        case Q_EVENT_BMA_INT:
            do {
                rlst =  ttgo->bma->readInterrupt();
            } while (!rlst);
            //! setp counter
            if (ttgo->bma->isStepCounter()) {
                //updateStepCounter(ttgo->bma->getCounter());
            }
            break;
        case Q_EVENT_AXP_INT:
            ttgo->power->readIRQ();
            /*
            if (ttgo->power->isVbusPlugInIRQ()) {
                updateBatteryIcon(LV_ICON_CHARGE);
            }
            if (ttgo->power->isVbusRemoveIRQ()) {
                updateBatteryIcon(LV_ICON_CALCULATION);
            }
            if (ttgo->power->isChargingDoneIRQ()) {
                updateBatteryIcon(LV_ICON_CALCULATION);
            }
            */
            if (ttgo->power->isPEKShortPressIRQ()) {
                //Serial.println("loop.cpp::Button pressed  ");
                if (!ttgo->bl->isOn()) { // Sleeping? Then wake up
                    //Serial.println("waking up");
                    low_energy();
                    //Serial.println("done waking up");
                } else {
                    Launcher::buttonPressed();
                }
            }
            if (ttgo->power->isPEKLongtPressIRQ() && ttgo->bl->isOn()) {
                ttgo->power->clearIRQ();
                low_energy();
                return;
            }
            ttgo->power->clearIRQ();
            break;
        case Q_EVENT_WIFI_SCAN_DONE: {
            wifiManager.scanDone();
            break;
        }
        default:
            break;
        }
    }
    // No screen Timeout when plugged in.
    if (ttgo->bl->isOn() && (lv_disp_get_inactive_time(NULL) < DEFAULT_SCREEN_TIMEOUT || ttgo->power->isVBUSPlug())) {
        lv_task_handler();
    } else {
        low_energy();
    }
}
