/**
 * @author Ivo Blöchliger
 */

#include "app.h"

App::app_cb_t* App::hide_cb = nullptr;
App::app_cb_t* App::show_cb = nullptr;
App::rtc_cb_t* App::setAlarm_cb = nullptr;