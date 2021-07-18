/**
 * @author Ivo Blöchliger
 * The audio handling is based on the example code by
 * @author Lewis He
 */

#include "spiffsaudio.h"
#include "../../../services/services.h"

// See https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/examples/BasicUnit/PlayMP3FromSPIFFS/PlayMP3FromSPIFFS.ino

bool SpiffsAudio::create() {
    self = this;
    lv_obj_t* bg = styles.stdBgImage(myScr);
    register_for_swipe_up(bg);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);
    playButton = styles.stdButton(bg, "Sound!", play_cb);
    buttonLabel = lv_obj_get_child(playButton, NULL);
    lv_obj_align(playButton, bg, LV_ALIGN_CENTER, 0,0);

    return true;
}

void SpiffsAudio::buildAudioChain() {
    if (audioSource==nullptr) {
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
}

void SpiffsAudio::freeAudioChain() {
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
}

bool SpiffsAudio::show() {
    #include "../../certs/ISRG_Root_X1.h"
    if (downloadToSPIFFS(this, SPIFFSAUDIO_MP3FILE, "https://tech-lab.ch/twatch/upchime2.mp3",root_ca)) {
        buildAudioChain();
    }
    return true;
}

bool SpiffsAudio::hide() {
    if (audioTask!=nullptr) {
        Serial.println("Disabling audio loop task");
        lv_task_del(audioTask);  audioTask = nullptr;        
    }
    freeAudioChain();
    return true;
}

SpiffsAudio* SpiffsAudio::self = nullptr;
lv_task_t* SpiffsAudio::audioTask = nullptr;