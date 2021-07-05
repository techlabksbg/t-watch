#include "spiffsaudio.h"

// See https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/examples/BasicUnit/PlayMP3FromSPIFFS/PlayMP3FromSPIFFS.ino

bool SpiffsAudio::create() {
    self = this;
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    connectButton = styles.stdButton(myScr, "Capt'n!", connect_cb);
    buttonLabel = lv_obj_get_child(connectButton, NULL);
    lv_obj_align(connectButton, myScr, LV_ALIGN_CENTER, 0,0);

    return true;
}

bool SpiffsAudio::show() {
    if (!SPIFFS.exists(SPIFFSAUDIO_MP3FILE)) {
        lv_obj_t* label = styles.stdLabel(myScr, SPIFFSAUDIO_MP3FILE "\nnot found.");
        lv_label_set_text(buttonLabel, "Quit");
        lv_obj_align(label, connectButton, LV_ALIGN_OUT_BOTTOM_MID, 0,20);
        register_for_hide_on_click(connectButton);
    } else if (audioSource==nullptr) {
        Serial.println("Building audio chain...");
        ttgo->enableLDO3();
        ttgo->enableAudio();
        audioSource = new AudioFileSourceSPIFFS();
        audioID3 = new AudioFileSourceID3(audioSource);
        audioI2S = new AudioOutputI2S();
        //External DAC decoding
        audioI2S->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
        audioMp3 = new AudioGeneratorMP3();
        //audioMp3->begin(audioID3, audioI2S);
        Serial.println("Audiochain done.");
    }
    
    return true;
}

bool SpiffsAudio::hide() {
    if (audioTask!=nullptr) {
        Serial.println("Disabling audio loop task");
        lv_task_del(audioTask);  audioTask = nullptr;        
    }
    if (audioMp3!=nullptr) {
        Serial.println("SpiffsAudio::hide() tearing down audio chain...");
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
        Serial.println("delete audioSource");
        delete audioSource; audioSource = nullptr;
        Serial.println("Tear down done, disabling hardware...");
        ttgo->disableAudio();
    }
    return true;
}

SpiffsAudio* SpiffsAudio::self = nullptr;
lv_task_t* SpiffsAudio::audioTask = nullptr;