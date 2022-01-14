/**
 * @author Ivo Blöchliger
 */

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
    virtual void lv_event_callback(lv_obj_t* obj, lv_event_t event);

    
    void* getIcon() { return (void*) &webaudioicon; }
    const char * getName() { return "WebAudio"; }

    virtual void loop();
   
    private:

    lv_obj_t* connectButton = nullptr;
    lv_obj_t* buttonLabel = nullptr;

    AudioOutputI2S* audioOutput = nullptr;
    AudioFileSourceHTTPStream* audioSource = nullptr;
    AudioFileSourceID3* audioID3 = nullptr;
    AudioGeneratorMP3* audioMp3 = nullptr;
    AudioOutputI2S* audioI2S = nullptr;
    // See https://github.com/earlephilhower/ESP8266Audio#audiofilesourcebuffer---double-buffering-useful-for-http-streams
    AudioFileSourceBuffer* audioBuff = nullptr;


 

};
