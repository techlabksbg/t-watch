/**
 * @brief Central GUI-Class managing the life cycle of each app.
 * 
 * @author Ivo Blöchliger
 */

#pragma once

#include "../app.h"

/**
 * Include all apps to be shown in the launcher.
 * Dont't forget to register them below in the setup
 * methods.
 */

#include "../watches/techlabwatch/techlabwatch.h"
#include "../settingapps/brightness/brightness.h"
#include "../settingapps/reboot/reboot.h"
#include "../settingapps/poweroff/poweroff.h"
#include "../settingapps/sysinfo/sysinfo.h"
#include "../settingapps/wifimanager/wifimanager.h"
#include "../settingapps/webfiles/webfiles.h"
#include "../settingapps/ntpsync/ntpsync.h"
#include "../demoapps/simplealarm/simplealarm.h"
#include "../demoapps/motorapp/motorapp.h"
#include "../demoapps/webaudio/webaudio.h"
#include "../demoapps/spiffsaudio/spiffsaudio.h"
#include "../demoapps/micdemo/micdemo.h"
#include "../demoapps/acceldemo/acceldemo.h"
#include "../demoapps/shoppinglist/shoppinglist.h"
#include "../demoapps/tide/tide.h"


// 
// Converter at https://lvgl.io/tools/imageconverter
LV_IMG_DECLARE(settingsicon);
LV_IMG_DECLARE(demoappsicon);



class Launcher : public App {
    public:
    Launcher(const char * _name, Launcher* _parent): 
        parent(_parent), 
        name(_name)
    {
        appType=TYPE_LAUNCHER;
        hasParent = parent!=nullptr;
    }

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();
    
    void* icon;
    void* getIcon() { return icon; }
    const char * getName() { return name; }

    static Launcher* rootLauncher;


    private:
    lv_obj_t* page;
    lv_obj_t** tiles;
    Launcher* parent;
    bool hasParent;
    const char * name;
    
    void buildTileView();

    App* apps[MAXAPPS];
    int numberOfApps=0;
    void registerApp(App* app);
    

    void static tile_event_cb(lv_obj_t *obj, lv_event_t event) {        
        Serial.printf("Launcher::tile_event_cb() with event %d\n", event);
        if (event != LV_EVENT_SHORT_CLICKED) return;
        // Recover corresponding instance
        Launcher* launcher = (Launcher *) lv_obj_get_user_data(obj);
        // Should not happen...
        if (launcher==nullptr) return;
        Serial.printf("Callback with event %d in launcher %s\n", event, launcher->getName());
        int appNr=0;
        for (appNr=0; appNr < launcher->numberOfApps; appNr++) {
            if (launcher->tiles[appNr]==obj) break;
        }
        if (appNr==launcher->numberOfApps) return;
        Serial.printf("Launching app nr %d with name %s\n", appNr, launcher->apps[appNr]->getName());
        showApp(launcher->apps[appNr]);
    }

    /** 
     * Build the Settings Launcher, as a child of the
     * root launcher
     */
    static Launcher* setupSettingsLauncher() {
        Launcher* settings = new Launcher("Settings", rootLauncher);
        settings->icon = (void*) &settingsicon;
        settings->registerApp(new Brightness);
        settings->registerApp(&wifiManager);
        settings->registerApp(new NTP_Sync);
        settings->registerApp(new WebFiles);
        settings->registerApp(new SysInfo);
        settings->registerApp(new Reboot);
        settings->registerApp(new PowerOff);
        return settings;
    }

    /**
     * Build the demo launcher
     */
    static Launcher* setupDemoLauncher() {
        Launcher* demos = new Launcher("Demos", rootLauncher);
        demos->icon = (void*) &demoappsicon;
        demos->registerApp(new MotorApp);
        demos->registerApp(new AccelDemo);
        demos->registerApp(new SimpleAlarm);
        demos->registerApp(new WebAudio);
        demos->registerApp(new SpiffsAudio);
        demos->registerApp(new MicDemo);
        demos->registerApp(new ShoppingList);
        demos->registerApp(new Tide);
        return demos;
    }

    public:
    /**
     * Setup the main launcher (rootLauncher) and
     * all sub-launchers
     */
    static void setup() {
        App::hide_cb = &hideApp;
        App::show_cb = &showApp;
        App::setAlarm_cb = &setAlarm;
        Launcher::rootLauncher->registerApp(setupSettingsLauncher());
        Launcher::rootLauncher->registerApp(setupDemoLauncher());
        Launcher::rootLauncher->registerApp(new TechLabWatch);
        Serial.println("Launcher::setup() complete");
        if (configJson->containsKey("alarmApp")) {
            Serial.printf("Restoring Alarm callback for app %s\n", (const char *)((*configJson)["alarmApp"]));
            alarmApp = rootLauncher->getAppByName((const char *)((*configJson)["alarmApp"]));
        }
    }


    static void setAlarm(App* app, int hours, int minutes) {
        Serial.printf("Setting alarm to %02d:%02d to call App %s\n", hours, minutes, app->getName());
        time_t now;
        struct tm  info;
        time(&now);
        localtime_r(&now, &info);
        if (info.tm_min+info.tm_hour*60 > hours*60+minutes) { // Alarm time has already pased today
            Serial.printf("time smaller (now is %02d:%02d, day %d, wday %d\n", info.tm_hour, info.tm_min, info.tm_mday, info.tm_wday);
            info.tm_mday++;
            now = mktime(&info);
            localtime_r(&now, &info);
        }
        ttgo->rtc->setAlarm(hours, minutes, info.tm_mday, info.tm_wday);
        Serial.printf("Enabling alarm on day %d, wday %d\n", info.tm_mday, info.tm_wday);
        ttgo->rtc->enableAlarm();
        (*configJson)["alarmApp"] = app->getName();
        saveJsonConfig();
        alarmApp = app;
    }

   


    static App* activeApp;
    static App* lastLauncher;
    static App* lastWatch;
    static App* lastApp;
    static App* sleepyApp;
    static App* alarmApp;

    static void goToSleep() {
        if (activeApp!=nullptr && activeApp->state==STATE_SHOWN && activeApp->isNormal()) {
            Serial.printf("goToSleep(): Putting app %s to sleep\n", activeApp->getName());
            sleepyApp = activeApp;
            hideApp(activeApp);
        } else {
            Serial.printf("Launcher::goToSleep(): No applicable active app (%s does not qualify)\n", activeApp==nullptr ? "nullptr" : activeApp->getName());
            sleepyApp = nullptr;
        }
    }
    static void wakeUp() {
        if (sleepyApp!=nullptr) {
            Serial.printf("Launcher::wakeUp(): Waking app %s back up\n", sleepyApp->getName());
            showApp(sleepyApp);
            sleepyApp = nullptr;
        }
    }

    static void rtcAlarmFired() {
        if (alarmApp!=nullptr) {
            Serial.printf("Launcher::rtcAlarmFired(): Showing app %s\n", alarmApp->getName());
            showApp(alarmApp);
            Serial.printf("Launcher::rtcAlarmFired(): Calling processAlarm on app %s\n", alarmApp->getName());
            alarmApp->processAlarm();
            alarmApp = nullptr;
        }
    }

    static void doubleTap() {
        if (activeApp->isLauncher() && activeApp!=rootLauncher) {
            hideApp(activeApp);
        }
    }
   

    static void rememberApp(App* app) {
        lastApp = app;
        switch (app->getType()) {
            case TYPE_WATCH:
            lastWatch = app;
            break;
            case TYPE_LAUNCHER:
            lastLauncher = app;
            break;
            default:
            break;
        }
    }

    static void showApp(void * app) {
        showApp((App*) app);
    }
    static void showApp(App* app) {
        Serial.printf("About to show app %s\n", app->getName());
        if (app->state == STATE_UNINITALIZED || app->state == STATE_DESTROYED) {
            Serial.println("setting up myScr");
            app->myScr = lv_obj_create(NULL, NULL);
            //Serial.println("aligning myScr");
            //lv_obj_align(app->myScr, NULL, LV_ALIGN_CENTER, 0, 0);
            Serial.println("  create...");
            if (app->create()) {
                app->state = STATE_CREATED;
            } else {
                lv_obj_del(app->myScr);
                app->myScr = nullptr;
                app->state = STATE_ERROR;
            }
        } else {
            Serial.println("  not creating again");
        }

        if (app->state == STATE_CREATED || app->state==STATE_HIDDEN) {
            Serial.println("  show...");
            if (app->show()) {
                if (activeApp!=nullptr) {
                    Serial.printf("  hiding %s\n", activeApp->getName());
                    activeApp->hide();
                    activeApp->state = STATE_HIDDEN;
                    rememberApp(activeApp);
                }
                Serial.printf("  switching screen to %p...\n", app->myScr);
                lv_scr_load(app->myScr);
                Serial.println("    switching done");
                app->state = STATE_SHOWN;
                activeApp = app;
            } else {
                Serial.println("  error, about to destroy app..");
                app->destroy();
                app->state = STATE_ERROR;
            }
        } else {
            Serial.printf(" ?? The app is in state %d\n", app->state);
        }
    }

    static void hideApp(void * app) {
        hideApp((App*)app);
    }
    static void hideApp(App* app) {
        Serial.printf("Request to hide app %s\n", app->getName());
        if (app->state == STATE_SHOWN) {
            if (app->isWatch()) {
                lastWatch = app;
            }
            Serial.print("  hide...");
            if (app->hide()) {
                Serial.println(" ok");
                app->state = STATE_HIDDEN;
                App* nextApp = nullptr;
                switch(app->getType()) {
                case TYPE_NORMAL:
                    nextApp = lastLauncher;
                    break;
                case TYPE_LAUNCHER:
                    {
                        Launcher* l = (Launcher*) app;
                        if (l->hasParent) {
                            nextApp = l->parent;
                        } else {
                            nextApp = app;
                        }
                    }
                    break;
                case TYPE_WATCH:
                    nextApp = lastApp!=nullptr ? lastApp : rootLauncher;
                    break;
                }
                activeApp=nullptr;                
                lv_async_call(showApp, nextApp);
            } else {
                Serial.println(" failed, about to destroy");
                app->state = STATE_ERROR;
                app->destroy();
                activeApp=nullptr;                
                lv_async_call(showApp, rootLauncher);
            }
        } else {
            Serial.println("  app not in STATE_SHOWN");
        }
    }

    // Toggle between Watch and any App
    static void buttonPressed() {
        Serial.printf("Button pressed, active App is %s of type %d",activeApp->getName(), activeApp->getType());
        if (activeApp->getType()==TYPE_WATCH) {
            Serial.println("   it's a watch!");
            if (lastApp!=nullptr && lastApp->isWatch()) {
                showApp(lastApp);
            } else {
                showApp(rootLauncher);
            }
        } else {
            if (lastWatch!=nullptr) {
                showApp(lastWatch);
            } else {
                Serial.println("lastWatch is nullptr");
                App* watch = rootLauncher->findWatch();
                if (watch!=nullptr) {
                    showApp(watch);
                } else {
                    Serial.println("No watch found");
                    showApp(rootLauncher);
                }
            }
        } 
    }

    private:
    /** returns the first watch found (or nullptr) */
    App* findWatch();
    /** returns the first app instance with this name (or nullptr) */
    App* getAppByName(const char* name);


};

extern Launcher launcher;