/**
 * The Code in the 'os'-Folder derives from the SimpleWatch Example by Lewis He, see
 * https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/tree/master/examples/LVGL/SimpleWatch
 * @author Lewis He
 * @author Ivo Blöchliger
 */


#include "os.h"

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
                Serial.println("loop.cpp: ttgo->bma->isStepCounter() == true");
                //updateStepCounter(ttgo->bma->getCounter());
            }
            if (ttgo->bma->isDoubleClick()) {
                Serial.println("loop.cpp: ttgo->bma->isDoubleClick() == true");
                Launcher::doubleTap();
            }
            if (ttgo->bma->isTilt()) {
                Serial.println("loop.cpp: ttgo->bma->isTilt() == true");
            }
            if (ttgo->bma->isActivity()) {
                Serial.println("loop.cpp: ttgo->bma->isActivity() == true");
            }
            if (ttgo->bma->isAnyNoMotion()) {
                Serial.println("loop.cpp: ttgo->bma->isAnyNoMotion() == true");
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
                    Serial.println("loop.cpp::waking up from short button pressed");
                    low_energy();
                    //Serial.println("done waking up");
                } else {
                    Launcher::buttonPressed();
                }
            }
            if (ttgo->power->isPEKLongtPressIRQ() && ttgo->bl->isOn()) {
                ttgo->power->clearIRQ();
                Serial.println("loop.cpp::leeping from long button pressed");
                low_energy();
            }
            ttgo->power->clearIRQ();
            break;
        case Q_EVENT_WIFI_SCAN_DONE: {
            wifiManager.scanDone();
            break;
        }
        case Q_EVENT_RTC_INT:
            Serial.println("rtc alarm has fired");
            ttgo->rtc->resetAlarm();
            ttgo->rtc->disableAlarm();
            launcher.rtcAlarmFired();
            break;
        
        default:
            break;
        }
    }
    // No screen Timeout when plugged in.
    if (ttgo->bl->isOn() && (lv_disp_get_inactive_time(NULL) < DEFAULT_SCREEN_TIMEOUT || 
        (ttgo->power->isVBUSPlug() && lv_disp_get_inactive_time(NULL) < (DEFAULT_SCREEN_TIMEOUT)*10))) {
        lv_task_handler();
    } else if (ttgo->bl->isOn()) {
        low_energy();
    }
}
