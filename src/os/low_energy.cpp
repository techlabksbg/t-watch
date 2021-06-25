#include "os.h"

#include "apps/launcher/launcher.h"
#include "apps/settingapps/wifimanager/wifimanager.h"


void low_energy()
{
    if (ttgo->bl->isOn()) { // go to sleep
        
        ttgo->closeBL();
        ttgo->stopLvglTick();
        ttgo->bma->enableStepCountInterrupt(false);
        ttgo->displaySleep();
        if (WiFi.isConnected()) {
            wifiManager.disconnect();
        }
        Launcher::goToSleep();
        lenergy = true;
        Serial.println("ENTER IN LIGHT SLEEEP MODE");
        gpio_wakeup_enable ((gpio_num_t)AXP202_INT, GPIO_INTR_LOW_LEVEL);
        gpio_wakeup_enable ((gpio_num_t)BMA423_INT1, GPIO_INTR_HIGH_LEVEL);
        esp_sleep_enable_gpio_wakeup ();
        esp_light_sleep_start();
    } else {
        ttgo->startLvglTick();
        ttgo->displayWakeup();
        ttgo->rtc->syncToSystem();
        /*
        updateStepCounter(ttgo->bma->getCounter());
        updateBatteryLevel();
        updateBatteryIcon(LV_ICON_CALCULATION);*/
        lv_disp_trig_activity(NULL);
        ttgo->openBL();
        ttgo->bma->enableStepCountInterrupt();
        Launcher::wakeUp();
        lenergy=false;
    }
}
