/**
 * @author Ivo Blöchliger
 */

#include "app.h"

App::app_cb_t* App::hide_cb = nullptr;
App::app_cb_t* App::show_cb = nullptr;
App::rtc_cb_t* App::setAlarm_cb = nullptr;

void App::start_loop(uint32_t period) {
    if (loop_task==nullptr) {
        loop_task = lv_task_create([](lv_task_t * taskptr){
            ((App*)(taskptr->user_data))->loop();
        },period, LV_TASK_PRIO_LOWEST, this);
    }
}

void App::stop_loop() {
    if (loop_task) {
        lv_task_del(loop_task);
        loop_task = nullptr;
    }
}