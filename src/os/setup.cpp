#include "os.h"

#include "apps/launcher/launcher.h"


void os_setup() {
    Serial.begin(115200);
    


    //Create a program that allows the required message objects and group flags
    g_event_queue_handle = xQueueCreate(20, sizeof(uint8_t));
    g_event_group = xEventGroupCreate();

    ttgo = TTGOClass::getWatch();

    //Initialize TWatch
    ttgo->begin();

    // Turn on the IRQ used
    ttgo->power->adc1Enable(AXP202_BATT_VOL_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1, AXP202_ON);
    ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_FINISHED_IRQ, AXP202_ON);
    ttgo->power->clearIRQ();

    //Initialize lvgl
    ttgo->lvgl_begin();

    // Enable BMA423 interrupt ，
    // The default interrupt configuration,
    // you need to set the acceleration parameters, please refer to the BMA423_Accel example
    ttgo->bma->attachInterrupt();

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

    //Check if the RTC clock matches, if not, use compile time
    ttgo->rtc->check();

    //Synchronize time to system time
    ttgo->rtc->syncToSystem();

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

    //Setting up the network
    setupNetwork();


    //Clear lvgl counter
    lv_disp_trig_activity(NULL);

#ifdef LILYGO_WATCH_HAS_BUTTON
    //In lvgl we call the button processing regularly
    lv_task_create([](lv_task_t *args) {
        ttgo->button->loop();
    }, 30, 1, nullptr);
#endif

    //When the initialization is complete, turn on the backlight
    ttgo->openBL();
    loadJsonConfig();

    //Execute your own GUI interface
    styles.setup();
    Launcher::setup();
    Launcher::showApp(&launcher);
}
