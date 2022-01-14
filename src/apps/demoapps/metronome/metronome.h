/**
 * @author Philipp Kündig
 */

#pragma once

#include <SPIFFS.h>

#include "AudioOutputI2S.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

#include "../../app.h"

LV_IMG_DECLARE(metronomeicon);

class Metronome : public App
{

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true; }

    virtual void loop();

    void *getIcon() { return (void *)&metronomeicon; }
    const char *getName() { return "Metronome"; }

private:
    AudioOutputI2S *audioOutput = nullptr;
    AudioFileSourceSPIFFS *audioSource = nullptr;
    AudioFileSourceID3 *audioID3 = nullptr;
    AudioGeneratorMP3 *audioMp3 = nullptr;
    AudioOutputI2S *audioI2S = nullptr;

    useconds_t file_lenght = 180;

    bool is_quarter = false;
    bool is_mute = true;
    bool is_motor = false;
    bool is_black_bg = true;
    int loop_count = 0;
    int time, time_last;
    int speed_init = 80;
    int wait_time = (60000 / (float)speed_init) + 0.5;
    int pulse_time = 50;
    int state = 0; // 0 = waiting for time to pass; 1 = playing a tone

    const char *audiofile[2] = {"/4d.mp3", "/4c#.mp3"};
    void buildAudioChain();
    void freeAudioChain();
    void play_tone();

    lv_obj_t *metronome, *slider, *metronome_speed_label, *quarter_note, *mute, *motor;

    static void mute_cb(lv_obj_t *obj, lv_event_t event)
    {
        if (event == LV_EVENT_VALUE_CHANGED)
        {
            ((Metronome *)lv_obj_get_user_data(obj))->is_mute = !((Metronome *)lv_obj_get_user_data(obj))->is_mute;
        }
    }

    static void motor_cb(lv_obj_t *obj, lv_event_t event)
    {
        if (event == LV_EVENT_VALUE_CHANGED)
        {
            ((Metronome *)lv_obj_get_user_data(obj))->is_motor = !((Metronome *)lv_obj_get_user_data(obj))->is_motor;
        }
    }

    static void quarter_note_cb(lv_obj_t *obj, lv_event_t event)
    {
        if (event == LV_EVENT_VALUE_CHANGED)
        {
            ((Metronome *)lv_obj_get_user_data(obj))->is_quarter = !((Metronome *)lv_obj_get_user_data(obj))->is_quarter;
        }
    }

    void set_labels(int speed)
    {
        static char buf[8];
        snprintf(buf, 8, "%u bpm", speed);
        lv_label_set_text(metronome_speed_label, buf);
        wait_time = (60000 / (float)speed) + 0.5;
        lv_obj_align(metronome_speed_label, slider, LV_ALIGN_OUT_TOP_MID, 0, -30);
    }

    static void slider_cb(lv_obj_t *obj, lv_event_t event)
    {
        if (event == LV_EVENT_VALUE_CHANGED)
        {
            int speed_label = lv_slider_get_value(obj);
            ((Metronome *)lv_obj_get_user_data(obj))->set_labels(speed_label);
        }
    }
};
