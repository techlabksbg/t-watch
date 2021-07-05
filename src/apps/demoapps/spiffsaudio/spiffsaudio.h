#pragma once

// Upload this file to SPIFFS (for example by using the webfiles app)
#define SPIFFSAUDIO_MP3FILE "/tngchirp.mp3"

#include <SPIFFS.h>

#include "AudioOutputI2S.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"


#include "../../app.h"
#include "../../settingapps/wifimanager/wifimanager.h"

LV_IMG_DECLARE(spiffsaudioicon);


class SpiffsAudio : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &spiffsaudioicon; }
    const char * getName() { return "SpiffsAudio"; }

    static SpiffsAudio* self;


    private:

    static lv_task_t* audioTask;

    lv_obj_t* connectButton = nullptr;
    lv_obj_t* buttonLabel = nullptr;

    AudioOutputI2S* audioOutput = nullptr;
    AudioFileSourceSPIFFS* audioSource = nullptr;
    AudioFileSourceID3* audioID3 = nullptr;
    AudioGeneratorMP3* audioMp3 = nullptr;
    AudioOutputI2S* audioI2S = nullptr;

    static void connect_cb(lv_obj_t *button, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        if (self->audioMp3!=nullptr) {
            if (!self->audioMp3->isRunning()) {
                self->audioSource->open(SPIFFSAUDIO_MP3FILE);
                self->audioMp3->begin(self->audioID3, self->audioI2S);
                if (self->audioTask==nullptr) {
                    Serial.println("SpiffsAudio::show Creating task");
                    self->audioTask = lv_task_create(audioLoop, 5, LV_TASK_PRIO_LOWEST, NULL);
                }
            }
        }
    }



    static void audioLoop(struct _lv_task_t *data) {
        //int n = 0;
        if (self->audioMp3!=nullptr) {
           //n = 1;
            if (self->audioMp3->isRunning()) {
                //n = 2;
                if (!self->audioMp3->loop()) {
                    self->audioSource->close();
                    self->audioMp3->stop();
                    Serial.println("audioLoop(): Stopping task");
                    lv_task_del(self->audioTask);
                    self->audioTask = nullptr;
                } else {
                    //n = 4;
                }
            } else {
                //n = 5;
                // self->hide_myself();
            }
        }
        //Serial.print('0'+n);
    }



};
