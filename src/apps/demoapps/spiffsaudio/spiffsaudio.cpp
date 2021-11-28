/**
 * @author Ivo Blöchliger
 * The audio handling is based on the example code by
 * @author Lewis He
 */

#include "spiffsaudio.h"
#include "../../../services/services.h"
#include "../../../os/certs/certs.h"

// See https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/examples/BasicUnit/PlayMP3FromSPIFFS/PlayMP3FromSPIFFS.ino

bool SpiffsAudio::create() {
    lv_obj_t* bg = styles.stdBgImage(myScr);
    register_for_swipe_up(bg);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);
    playButton = styles.stdButton(bg, "Sound!", [](lv_obj_t *button, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        ((SpiffsAudio*)(button->user_data))->play_button();
    }, this);

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
    if (downloadToSPIFFS(this, SPIFFSAUDIO_MP3FILE, "https://tech-lab.ch/twatch/upchime2.mp3", ISRG_ROOT_X1)) {
        buildAudioChain();
    }
    return true;
}

bool SpiffsAudio::hide() {
    stop_loop();
    freeAudioChain();
    return true;
}

void SpiffsAudio::play_button() {
    if (audioMp3==nullptr) {
        buildAudioChain();
    }
    if (audioMp3!=nullptr) {
        if (!audioMp3->isRunning() && SPIFFS.exists(SPIFFSAUDIO_MP3FILE)) {
            audioSource->open(SPIFFSAUDIO_MP3FILE);
            audioMp3->begin(audioID3, audioI2S);
            start_loop(5);
        }
    }
}

void SpiffsAudio::loop() {
    if (audioMp3!=nullptr) {
        if (audioMp3->isRunning()) {
            if (!audioMp3->loop()) {
                audioSource->close();
                audioMp3->stop();
                stop_loop();
            }        
        }
    }
}