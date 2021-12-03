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
    lv_slider_set_range(slider, 20, 300); // mp3 files all 180ms long
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

    quarter_note = lv_checkbox_create(myScr, NULL);
    lv_checkbox_set_text(quarter_note, "Quarter Notes");
    lv_obj_set_event_cb(quarter_note, quarter_note_cb);
    lv_obj_align(quarter_note, NULL, LV_ALIGN_CENTER, 0, 40);
    lv_obj_set_user_data(quarter_note, this);

    Serial.println("Creating Metronome");
    return true;
}

bool Metronome::show()
{
    Serial.println("Showing Metronome");
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
        Serial.println("Audiochain done.");
    }
}

void Metronome::play_tone()
{
    int choose_file = 1;
    if (audioMp3 == nullptr)
    {
        buildAudioChain();
    }
    if (is_quarter && loop_count == 4)
    {
        choose_file = 0;
    }
    if (audioMp3 != nullptr)
    {
        if (audioMp3->isRunning() && !audioMp3->loop())
        {
            is_running = false;
            audioSource->close();
            audioMp3->stop();
        }
        else if (!audioMp3->isRunning() && SPIFFS.exists(audiofile[choose_file]))
        {
            if (loop_count == 4)
            {
                loop_count = 0;
            }
            audioSource->open(audiofile[choose_file]);
            audioMp3->begin(audioID3, audioI2S);
            usleep(180);
            is_running = true;
            loop_count++;
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
    play_tone();
    while (is_running)
    {
        play_tone();
    }
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
            }
        }
    }
    stop_loop();
    return true;
}
