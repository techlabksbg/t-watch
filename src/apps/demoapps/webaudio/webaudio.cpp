/**
 * @author Ivo Blöchliger
 * The audio handling is based on example code by 
 * @author Lewis He
 */


#include "webaudio.h"

// See https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/examples/BasicUnit/PlayMP3FromSPIFFS/PlayMP3FromSPIFFS.ino

bool WebAudio::create() {
    lv_obj_t* bg = styles.stdBgImage(myScr);
    register_for_swipe_up(bg);
    lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &styles.background);
    connectButton = styles.stdButton(bg, "Connect", [](lv_obj_t *button, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        ((WebAudio*)(button->user_data))->button_pressed();
    }, this);
    buttonLabel = lv_obj_get_child(connectButton, NULL);
    lv_obj_align(connectButton, myScr, LV_ALIGN_CENTER, 0,0);

    return true;
}

void WebAudio::button_pressed() {
    if (strcmp(lv_label_get_text(buttonLabel), "Quit")==0) {
        hide_myself();
    } else {
        wifiManager.connect(this);
    }
}

bool WebAudio::show() {
    if (WiFi.isConnected()) {
        Serial.println("WebAudio::show() connected");
        lv_label_set_text(buttonLabel, "Quit");
        if (audioSource==nullptr) {
            Serial.println("Building audio chain...");
            ttgo->enableLDO3();
            ttgo->enableAudio();
            audioSource = new AudioFileSourceHTTPStream("http://mp3.ffh.de/radioffh/hqlivestream.mp3");
            audioBuff = new AudioFileSourceBuffer(audioSource, 4096);
            audioID3 = new AudioFileSourceID3(audioBuff);
            /*#if defined(STANDARD_BACKPLANE)
                audioI2S = new AudioOutputI2S(0, 1);
            #elif defined(EXTERNAL_DAC_BACKPLANE) */
            audioI2S = new AudioOutputI2S();
            //External DAC decoding
            audioI2S->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
            //#endif
            audioMp3 = new AudioGeneratorMP3();
            audioMp3->begin(audioID3, audioI2S);
            Serial.println("Audiochain done.");
        }
        start_loop(5);
    } else {
        // connect to wifi after initialisation
        lv_async_call([](void *userdata) {
            Serial.println("Async connect to WiFi...");
            wifiManager.connect((App*)userdata);
        }, this);
        Serial.println("no connection... async call scheduled");
        lv_label_set_text(buttonLabel, "Connect");
    }
    return true;
}

bool WebAudio::hide() {
    stop_loop();
    if (audioMp3!=nullptr) {
        Serial.println("WebAudio::hide() tearing down audio chain...");
        if (audioMp3->isRunning()) {
            Serial.println("Stopping mp3");
            audioMp3->stop();
        }
        Serial.println("Closing source");
        audioSource->close();
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
    return true;
}


void WebAudio::loop() {
    if (audioMp3!=nullptr && audioMp3->isRunning()) {
        if (!audioMp3->loop()) {
            audioMp3->stop();
            hide_myself();
        }
    } 
}

