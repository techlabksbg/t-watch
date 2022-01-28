/**
 * The Code in the 'os'-Folder derives from the SimpleWatch Example by Lewis He, see
 * https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/tree/master/examples/LVGL/SimpleWatch
 * @author Lewis He
 * @author Ivo Blöchliger
 */

#include "os.h"

#include "apps/launcher/launcher.h"
#include "ota.h"

// See https://docs.lvgl.io/latest/en/html/porting/log.html
// Set Variables in library TTWatch/src/lvgl.conf
#if LV_USE_LOG
void my_log_cb(lv_log_level_t level, const char * file, uint32_t line, const char * fn_name, const char * dsc)
{
    if (line==129 && strcmp("_lv_indev_read_task", fn_name)==0) return;
    if (line==192 && strcmp("lv_task_handler", fn_name)==0) return;
    if (line==74 && strcmp("lv_task_handler", fn_name)==0) return;
    if (line==77 && strcmp("_lv_indev_read_task", fn_name)==0) return;
    if (line==153 && strcmp("_lv_indev_read", fn_name)==0) return;
    if (line==155 && strcmp("_lv_indev_read", fn_name)==0) return;



  /*Send the logs via serial port*/
  if(level == LV_LOG_LEVEL_ERROR) Serial.print("ERROR: ");
  if(level == LV_LOG_LEVEL_WARN)  Serial.print("WARNING: ");
  if(level == LV_LOG_LEVEL_INFO)  Serial.print("INFO: ");
  if(level == LV_LOG_LEVEL_TRACE) Serial.print("TRACE: ");

  Serial.print("File: ");
  Serial.print(file);

  Serial.printf(":%d %s: %s\n", line, fn_name, dsc);
}
#endif

void os_setup() {
    Serial.begin(115200);
    

    //Create a program that allows the required message objects and group flags
    g_event_queue_handle = xQueueCreate(20, sizeof(uint8_t));
    g_event_group = xEventGroupCreate();

    ttgo = TTGOClass::getWatch();

    //Initialize TWatch
    ttgo->begin();

    // Turn on backlight
    ttgo->openBL();
    ttgo->tft->setTextColor(0xffff);
    ttgo->tft->println("Enabling IRQs");

    ttgo->tft->println("  -> power");
    // Turn on the IRQ used
    ttgo->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
    ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
    ttgo->power->clearIRQ();


    ttgo->tft->println("  -> accelerometer");
    // Enable BMA423 interrupt ，
    // The default interrupt configuration,
    // you need to set the acceleration parameters, please refer to the BMA423_Accel example
    ttgo->bma->attachInterrupt();
    ttgo->bma->enableActivityInterrupt();
    ttgo->bma->enableAnyNoMotionInterrupt();
    ttgo->bma->enableTiltInterrupt(false);
    ttgo->bma->enableWakeupInterrupt();
    ttgo->bma->enableStepCountInterrupt();
    

    //Connection interrupted to the specified pin
    pinMode(BMA423_INT1, INPUT);
    attachInterrupt(BMA423_INT1, [] {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        uint8_t data = Q_EVENT_BMA_INT;
        xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR ();
        }
    }, RISING);

    // Connection interrupted to the specified pin
    pinMode(AXP202_INT, INPUT);
    attachInterrupt(AXP202_INT, [] {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        uint8_t data = Q_EVENT_AXP_INT;
        xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR ();
        }
    }, FALLING);

    ttgo->tft->println("rtc to system");
    //Check if the RTC clock matches, if not, use compile time
    ttgo->rtc->check();
    //Synchronize time to system time
    ttgo->rtc->syncToSystem();
    // Set up interrupt for rtc (see https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/examples/BasicUnit/RTC/RTC.ino)
    pinMode(RTC_INT_PIN, INPUT_PULLUP);
    attachInterrupt(RTC_INT_PIN, [] {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        uint8_t data = Q_EVENT_RTC_INT;
        xQueueSendFromISR(g_event_queue_handle, &data, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR ();
        }
    }, FALLING);

    ttgo->tft->println("init motor");
    // Initialize vibration Motor
    ttgo->motor_begin();

#ifdef LILYGO_WATCH_HAS_BUTTON

    /*
        ttgo->button->setClickHandler([]() {
            Serial.println("Button2 Pressed");
        });
    */

    //Set the user button long press to restart
    ttgo->button->setLongClickHandler([]() {
        Serial.println("Pressed Restart Button,Restart now ...");
        delay(1000);
        esp_restart();
    });
#endif

    ttgo->tft->println("set wifi callbacks");
    //Setting up the network
    setupNetwork();

  

#if LV_USE_LOG
    lv_log_register_print_cb(my_log_cb);
#endif

#ifdef LILYGO_WATCH_HAS_BUTTON
    //In lvgl we call the button processing regularly
    lv_task_create([](lv_task_t *args) {
        ttgo->button->loop();
    }, 30, 1, nullptr);
#endif

    ttgo->tft->println("set up SPIFFS");
    if (!(spiffs_initialized = SPIFFS.begin())) {
        ttgo->tft->println("Formating SPIFFS");
        Serial.println("Formating SPIFFS");
        if (!SPIFFS.format()) {
            Serial.println("SPIFFS format failed!");
        } else {
            if (!SPIFFS.begin()) {
                Serial.println("SPIFFS mount failed!");
            }
        }
    }

    ttgo->tft->println("load config");
    loadJsonConfig();

    //ttgo->tft->println("starting rtc handler");
    //rtcHandler = new RTCHandler();



    // watchOTA instance.
    watchOTA = new WatchOTA();

    ttgo->tft->println("start lvgl");
    //Initialize lvgl
    ttgo->lvgl_begin();

    //Clear lvgl counter
    lv_disp_trig_activity(NULL);

    //Execute your own GUI interface
    styles.setup();
    Launcher::setup();
    Launcher::showApp(&launcher);
}
