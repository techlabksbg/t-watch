/**
 * @author Ivo Blöchliger
 */

#pragma once

// Upload this file to SPIFFS (for example by using the webfiles app)
#define SPIFFSAUDIO_MP3FILE "/upchime2.mp3"

#include <SPIFFS.h>

#include "AudioOutputI2S.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"


#include "../../app.h"

LV_IMG_DECLARE(spiffsaudioicon);


class SpiffsAudio : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}
    virtual void lv_event_callback(lv_obj_t* obj, lv_event_t event);
    
    void* getIcon() { return (void*) &spiffsaudioicon; }
    const char * getName() { return "SpiffsAudio"; }

    virtual void loop();

    private:

    lv_obj_t* playButton = nullptr;
    lv_obj_t* buttonLabel = nullptr;

    AudioOutputI2S* audioOutput = nullptr;
    AudioFileSourceSPIFFS* audioSource = nullptr;
    AudioFileSourceID3* audioID3 = nullptr;
    AudioGeneratorMP3* audioMp3 = nullptr;
    AudioOutputI2S* audioI2S = nullptr;

    void buildAudioChain();
    void freeAudioChain();

    void play_button();


};
