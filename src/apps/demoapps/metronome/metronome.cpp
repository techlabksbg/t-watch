/**
 * @author Philipp Kündig
 */
#include "metronome.h"
#include "../../../services/services.h"

bool Metronome::create()
{
    register_for_swipe_up(myScr);
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    slider = lv_slider_create(myScr, NULL);
    lv_obj_set_width(slider, 200);
    lv_obj_align(slider, myScr, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(slider, 20, 250);
    lv_slider_set_value(slider, 80, LV_ANIM_ON);
    lv_obj_set_user_data(slider, this);

    metronome_speed_label = lv_label_create(myScr, NULL);
    lv_obj_add_style(metronome_speed_label, LV_OBJ_PART_MAIN, &styles.textLabel);
    set_labels(80);

    metronome = lv_label_create(myScr, NULL);
    lv_obj_add_style(metronome, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_label_set_text(metronome, "Metronome");
    lv_label_set_align(metronome, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(metronome, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);

    Serial.println("Creating Metronome");
    return true;
}

bool Metronome::show()
{
    Serial.println("Showing Metronome");
    int wait_time = (60000 / (float)80) + 0.5;
    start_loop(wait_time);
    lv_obj_set_event_cb(slider, slider_cb);
    return true;
}
void Metronome::buildAudioChain()
{
    if (audioSource == nullptr)
    {
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

void Metronome::play_tone()
{
    if (audioMp3 == nullptr)
    {
        buildAudioChain();
    }
    if (audioMp3 != nullptr)
    {
        if (audioMp3->isRunning() && !audioMp3->loop())
        {
            audioSource->close();
            audioMp3->stop();
        }
        else if (!audioMp3->isRunning() && SPIFFS.exists(SPIFFSAUDIO_MP3FILE))
        {
            audioSource->open(SPIFFSAUDIO_MP3FILE);
            audioMp3->begin(audioID3, audioI2S);
            // start_loop(5);
        }
    }
}

// void Metronome::loop()
// {
//     if (audioMp3 != nullptr)
//     {
//         if (audioMp3->isRunning())
//         {
//             if (!audioMp3->loop())
//             {
//                 audioSource->close();
//                 audioMp3->stop();
//                 stop_loop();
//             }
//         }
//     }
// }

void Metronome::loop()
{
    Serial.println("beep");
    // if (audioMp3 != nullptr)
    // {
    //     if (audioMp3->isRunning())
    //     {
    //         if (!audioMp3->loop())
    //         {
    //             audioSource->close();
    //             audioMp3->stop();
    //             stop_loop();
    //         }
    //     }
    // }
    play_tone();
    // Serial.println("after beep");
}

bool Metronome::hide()
{
    if (audioMp3 != nullptr)
    {
        if (audioMp3->isRunning())
        {
            if (!audioMp3->loop())
            {
                audioSource->close();
                audioMp3->stop();
                // stop_loop();
            }
        }
    }
    stop_loop();
    return true;
}
