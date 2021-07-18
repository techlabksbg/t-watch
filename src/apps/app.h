/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../os/os.h"
#include "styles/styles.h"



#define MAXAPPS 32

class App {

    public:

    typedef enum {
        STATE_UNINITALIZED = 0,
        STATE_CREATED = 1,
        STATE_SHOWN = 2,
        STATE_HIDDEN = 3,
        STATE_DESTROYED = 4,
        STATE_ERROR = 5
    } app_state_t;

    typedef enum {
        TYPE_NORMAL = 0,
        TYPE_WATCH = 1,
        TYPE_LAUNCHER = 2
    } app_type_t;

    typedef void app_cb_t(App*);
    typedef void (rtc_cb_t)(App*, int, int);


    virtual bool create() = 0;
    virtual bool show() = 0;
    virtual bool hide() = 0;
    virtual bool destroy() = 0;

    virtual void* getIcon() = 0;
    virtual const char * getName() = 0;

    app_state_t state = STATE_UNINITALIZED;
    app_type_t appType = TYPE_NORMAL;
    
    bool isWatch() { return appType==TYPE_WATCH; }
    bool isLauncher() { return appType==TYPE_LAUNCHER; }
    bool isNormal() { return appType==TYPE_NORMAL; }
    app_type_t getType() {return appType; }
    app_state_t getState() { return state; }
    
    lv_obj_t* myScr;
    
    static app_cb_t* hide_cb;
    static app_cb_t* show_cb;
    static rtc_cb_t* setAlarm_cb;

    // Call this method if you want to hide/terminate your app
    // Do not call the hide method yourself, it will be called this way.
    void hide_myself() {
        Serial.printf("App::hide_myself() with %s\n",getName());
        (*hide_cb)(this);
    }

    /**
     * Sets the alarm for hours:minutes (in the next 24 hours).
     * Your app will be shown and the 
     * processAlarm() method will be called, so implement it.
     * @param hours Hours 0-23
     * @param minutes Minutes 0-59
     */
    void setAlarm(int hours, int minutes) {
        Serial.println("App::setAlarm(...) start");
        (*setAlarm_cb)(this, hours, minutes);
        Serial.println("App::setAlarm(...) end");
    }

    /**
     * This method will be called, if the alarm triggers.
     * You need to set the alarm with the setAlarm method.
     * Implement it!
     */ 
    virtual void processAlarm(){}

    /**
     * Implement this method an return true, if 
     * you don't want your app to be hidden on
     * double tap.
     */
    virtual bool processDoubleTap(){ return false; }

    static void show_app(App * app) {
        Serial.printf("App::show_app() with %s\n",app->getName());
        (*show_cb)(app);
    }

    // Register a lv_obj to quit on swipe up
    void register_for_swipe_up(lv_obj_t * obj) {
        lv_obj_set_click(obj, true);
        lv_obj_set_user_data(obj, this);  // Make current instance accessible in callback
        lv_obj_set_event_cb(obj, hide_on_swipe_up);
    }

    void register_for_hide_on_click(lv_obj_t * obj) {
        lv_obj_set_click(obj, true);
        lv_obj_set_user_data(obj, this);  // Make current instance accessible in callback
        lv_obj_set_event_cb(obj, hide_on_click);
    }   

    static void hide_on_click(lv_obj_t *obj, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        App* self = (App*) lv_obj_get_user_data(obj);
        if (self!=nullptr && hide_cb!=nullptr) {
            (*hide_cb)(self);
            return;
        } 
        Serial.printf("hide_on_swipe: app %p or hide_cb %p not defined...", self, hide_cb);
    }




    static void hide_on_swipe_up(lv_obj_t *obj, lv_event_t event) {
        //Serial.printf("hide_on_swipe_up start with event %d\n", event);
        static enum {
            NICHTS,
            ABEN,
        } zustand = NICHTS;
        static uint16_t startx,starty;
        static uint16_t endx,endy;

        if (event == LV_EVENT_PRESSING) {
            if (zustand==NICHTS) {
                zustand = ABEN;
                ttgo->touch->getPoint(startx,starty);
                //Serial.printf("start at %d, %d\n", startx, starty);
            } else {
                ttgo->touch->getPoint(endx,endy);
            }
        } else if (zustand==ABEN) {
            if (event==LV_EVENT_PRESS_LOST) {
                zustand = NICHTS;      
                return; // Still pressing, slid off the object.   
            }
            if (event==LV_EVENT_RELEASED) {
                zustand = NICHTS;      
                //Serial.printf("start at %d, %d, end at %d, %d      %d < %d?\n", startx, starty, endx, endy, abs(endx-startx)*5+20 , starty-endy);
                if (abs(endx-startx)*5+20 < starty-endy) {
                    App* self = (App*) lv_obj_get_user_data(obj);
                    if (self!=nullptr && hide_cb!=nullptr) {
                        Serial.printf("hide_on_swipe: app %p will be hidden", self);
                        (*hide_cb)(self);
                    } else {
                        //Serial.printf("hide_on_swipe: app %p or hide_cb %p not defined...", self, hide_cb);
                    }
                }
            }
        }
    }

    private:
    

};


