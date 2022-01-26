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
    lv_slider_set_value(slider, speed_init, LV_ANIM_ON);
    lv_obj_set_user_data(slider, this);

    metronome_speed_label = lv_label_create(myScr, NULL);
    lv_obj_add_style(metronome_speed_label, LV_OBJ_PART_MAIN, &styles.textLabel);
    set_labels(speed_init);

    metronome = lv_label_create(myScr, NULL);
    lv_obj_add_style(metronome, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_label_set_text(metronome, "Metronome");
    lv_label_set_align(metronome, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(metronome, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);

    quarter_note = lv_checkbox_create(myScr, NULL);
    lv_checkbox_set_text(quarter_note, "Quarter Notes");
    lv_obj_set_event_cb(quarter_note, quarter_note_cb);
    lv_obj_align(quarter_note, NULL, LV_ALIGN_IN_LEFT_MID, 0, 40);
    lv_obj_set_user_data(quarter_note, this);

    mute = lv_checkbox_create(myScr, NULL);
    lv_obj_add_state(mute, LV_STATE_CHECKED);
    lv_checkbox_set_text(mute, "Mute");
    lv_obj_set_event_cb(mute, mute_cb);
    lv_obj_align(mute, NULL, LV_ALIGN_IN_LEFT_MID, 0, 70);
    lv_obj_set_user_data(mute, this);

    motor = lv_checkbox_create(myScr, NULL);
    lv_checkbox_set_text(motor, "Vibrate");
    lv_obj_set_event_cb(motor, motor_cb);
    lv_obj_align(motor, NULL, LV_ALIGN_IN_LEFT_MID, 0, 100);
    lv_obj_set_user_data(motor, this);

    Serial.println("Creating Metronome");
    return true;
}

bool Metronome::show()
{
    Serial.println("Showing Metronome");
    lv_obj_set_event_cb(slider, slider_cb);
    start_loop(5);
    time_last = millis();
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

void Metronome::freeAudioChain()
{
    if (audioMp3 != nullptr)
    {
        Serial.println("SpiffsAudio::hide() tearing down audio chain...");
        if (audioMp3->isRunning())
        {
            Serial.println("Stopping mp3");
            audioMp3->stop();
        }
        Serial.println("delete audioMP3");
        delete audioMp3;
        audioMp3 = nullptr;
        Serial.println("delete audioI2S");
        delete audioI2S;
        audioI2S = nullptr;
        Serial.println("delete audioID3");
        delete audioID3;
        audioID3 = nullptr;
        Serial.println("delete audioSource");
        delete audioSource;
        audioSource = nullptr;
        Serial.println("Tear down done, disabling hardware...");
        ttgo->disableAudio();
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
            audioSource->close();
            audioMp3->stop();
            state = 0;
        }
        else if (!audioMp3->isRunning()) // && SPIFFS.exists(audiofile[choose_file]))
        {
            if (!is_mute)
            {
                if (!is_black_bg)
                {
                    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
                    is_black_bg = true;
                }
                if (!SPIFFS.exists(audiofile[choose_file]))
                {
                    return;
                }
                if (is_motor)
                {
                    ttgo->motor->onec(10);
                }
                if (loop_count == 4)
                {
                    loop_count = 0;
                }
                audioSource->open(audiofile[choose_file]);
                audioMp3->begin(audioID3, audioI2S);
                loop_count++;
            }
            else
            {
                if (state == 0)
                {
                    is_black_bg = false;
                    if (is_quarter && loop_count == 4)
                    {
                        lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
                    }
                    else
                    {
                        lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
                    }
                    if (is_motor)
                    {
                        ttgo->motor->onec(10);
                    }
                    if (loop_count == 4)
                    {
                        loop_count = 0;
                    }
                    loop_count++;
                }
                else if (state == 1)
                {
                    if (millis() - time_last > pulse_time)
                    {
                        lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
                        is_black_bg = true;
                        state = 0;
                    }
                }
            }
        }
    }
}

void Metronome::loop()
{
    time = millis(); // overflow not managed; overflow at ~49 days
    if (state == 0)
    {
        if (time - time_last > wait_time)
        {
            play_tone();
            state = 1;
            time_last = time;
        }
    }
    else if (state == 1)
    {
        play_tone();
    }
}

bool Metronome::hide()
{
    stop_loop();
    freeAudioChain();
    return true;
}
