/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"
//#include "BluetoothSerial.h"

LV_IMG_DECLARE(rpslsgameicon);
LV_IMG_DECLARE(rpslsgamebg);



class RpslsGame : public App {
    public:

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();
    virtual void loop();
    virtual void lv_event_callback(lv_obj_t* obj, lv_event_t event);

    virtual void* getIcon() { return (void*)&rpslsgameicon; }
    virtual const char * getName() {return "Rock, Scissors,...";}

    class Overlay {
        public:
        Overlay(lv_obj_t* bg, int cx, int cy, int r, lv_color_t color);
        ~Overlay();
        private:
        lv_obj_t* overlay;
        lv_style_t* style;
    };

    private:
    // image background
    lv_obj_t* bg; 
    // currently active overlay (or nullptr if none)
    Overlay* active = nullptr;
    // Server/Client buttons
    lv_obj_t* serverBT;
    lv_obj_t* clientBT;
    lv_obj_t* soloBT;
    int meine_Auswahl = -1;
    int seine_Auswahl = -1;

    void startGame();
    // Called when clicked on bg
    void click(int x, int y, bool long_pressed);
    // Labels for the score.
    lv_obj_t* labels[2];
    int scores[2];
    // display the scores
    void updateLabels();
    //BluetoothSerial * SerialBT; 
    void GameEvent(int Auswahl,int wer);

    enum {INIT, HAN_STYLE, WAITING_FOR_SERVER, WAITING_FOR_CLIENT, CONNECTED_AS_SERVER, CONNECTED_AS_CLIENT} zustand = INIT;

};
