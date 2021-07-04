#pragma once

#include "AudioOutputI2S.h"
#include "AudioFileSourceHTTPStream.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceBuffer.h"


#include "../../app.h"
#include "../../settingapps/wifimanager/wifimanager.h"

LV_IMG_DECLARE(webaudioicon);


class WebAudio : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &webaudioicon; }
    const char * getName() { return "WebAudio"; }

    static void connect_cb(lv_obj_t *button, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        if (strcmp(lv_label_get_text(self->buttonLabel), "Quit")==0) {
            Serial.println("Button Quit");
            self->hide_myself();
        } else {
            Serial.println("Button Connect");
            wifiManager.connect(self);
        }
    }


    private:
    static WebAudio* self;

    static lv_task_t* audioTask;

    lv_obj_t* connectButton = nullptr;
    lv_obj_t* buttonLabel = nullptr;

    AudioOutputI2S* audioOutput = nullptr;
    AudioFileSourceHTTPStream* audioSource = nullptr;
    AudioFileSourceID3* audioID3 = nullptr;
    AudioGeneratorMP3* audioMp3 = nullptr;
    AudioOutputI2S* audioI2S = nullptr;
    // See https://github.com/earlephilhower/ESP8266Audio#audiofilesourcebuffer---double-buffering-useful-for-http-streams
    AudioFileSourceBuffer* audioBuff = nullptr;


    static void audioLoop(struct _lv_task_t *data) {
        int n = 0;
        if (self->audioMp3!=nullptr) {
            n = 1;
            if (self->audioMp3->isRunning()) {
                n = 2;
                if (!self->audioMp3->loop()) {
                    n = 3;
                    //self->audioMp3->stop();
                    //self->hide_myself();
                } else {
                    n = 4;
                }
            } else {
                n = 5;
                // self->hide_myself();
            }
        }
        Serial.print('0'+n);
    }



};
