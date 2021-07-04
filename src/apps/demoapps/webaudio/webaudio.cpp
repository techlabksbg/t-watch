#include "webaudio.h"

// See https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/examples/BasicUnit/PlayMP3FromSPIFFS/PlayMP3FromSPIFFS.ino

bool WebAudio::create() {
    self = this;
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    connectButton = styles.stdButton(myScr, "Connect", connect_cb);
    buttonLabel = lv_obj_get_child(connectButton, NULL);
    lv_obj_align(connectButton, myScr, LV_ALIGN_CENTER, 0,0);

    return true;
}

bool WebAudio::show() {
    if (WiFi.isConnected()) {
        Serial.println("WebAudio::show() connected");
        lv_label_set_text(buttonLabel, "Quit");
        if (audioSource==nullptr) {
            Serial.println("Building audio chain...");
            ttgo->enableLDO3();
            audioSource = new AudioFileSourceHTTPStream("http://mp3.ffh.de/radioffh/hqlivestream.mp3");
            audioBuff = new AudioFileSourceBuffer(audioSource, 4096);
            audioID3 = new AudioFileSourceID3(audioSource);
            #if defined(STANDARD_BACKPLANE)
                audioI2S = new AudioOutputI2S(0, 1);
            #elif defined(EXTERNAL_DAC_BACKPLANE)
                audioI2S = new AudioOutputI2S();
                //External DAC decoding
                aduioI2S->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
            #endif
            audioMp3 = new AudioGeneratorMP3();
            audioMp3->begin(audioID3, audioI2S);
            Serial.println("Audiochain done.");
        }
        if (audioTask==nullptr) {
            Serial.println("WebAudio::show Creating task");
            audioTask = lv_task_create(audioLoop, 5, LV_TASK_PRIO_LOWEST, NULL);
        }

    } else {
        Serial.println("no connection...");
        lv_label_set_text(buttonLabel, "Connect");
    }
    return true;
}

bool WebAudio::hide() {
    if (audioMp3!=nullptr) {
        Serial.println("WebAudio::hide() tearing down audio chain...");
        if (audioMp3->isRunning()) {
            Serial.println("Stopping mp3");
            audioMp3->stop();
        }
        Serial.println("delete audioMP3");
        delete audioMp3;  audioMp3 = nullptr;
        Serial.println("delete audioI2S");
        delete audioI2S;  audioI2S = nullptr;
        Serial.println("delete audioID3");
        delete audioID3;  audioID3 = nullptr;
        Serial.println("delete audioBuff");
        delete audioBuff; audioBuff = nullptr;
        Serial.println("delete audioSource");
        delete audioSource; audioSource = nullptr;
        Serial.println("Tear down done, disabling hardware...");
        ttgo->disableAudio();
    }
    if (audioTask!=nullptr) {
        Serial.println("Disabling audio loop task");
        lv_task_del(audioTask);  audioTask = nullptr;        
    }
    return true;
}

WebAudio* WebAudio::self = nullptr;
lv_task_t* WebAudio::audioTask = nullptr;