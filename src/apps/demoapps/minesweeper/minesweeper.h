/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(minesweepericon);


class Minesweeper : public App {
    public:
    Minesweeper();
    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();
    virtual bool processDoubleTap() { return true; } // Do not terminate App on double Tap
    
    void* getIcon() { return (void*) &minesweepericon; }
    const char * getName() { return "Minesweeper"; }

    private: 
    /**
     * Double array (but contigous memory) size*size.
     * Values 0-8 number on field, hidden (+16 if shown)
     * Value 9 for a mine (+16 if show, +32 if marked)
     */
    byte** feld = nullptr;
    int size = 0;
    void initFeld(int mines=20);
    void dumpFeld();

    void initStyles();

    lv_obj_t* page;
    lv_obj_t** buttons = nullptr;
    lv_style_t hiddenStyle;
    lv_style_t shownStyle;
    lv_style_t labelStyle;
    lv_style_t highlightStyle;
    

    void buildButtons();
    void removeButtons();
    lv_obj_t* createButton();
    void buttonShortClick(lv_obj_t* button);
    void buttonLongPressed(lv_obj_t* button);
    void buttonLeave(lv_obj_t* button);
    void setButton(int x, int y);
    void highlightButtons(int x, int y, bool highlight);
    void uncoverZeros(int x, int y);
    void uncoverFilled(int x, int y);
    void boom(int a=-1, int b=-1);
    void winner();
    void newGame();

    enum {
        INIT,
        PLAYING,        
        LOST,
        WON
    } gameState;

    static void button_cb(lv_obj_t* button, lv_event_t event) {
        if (event == LV_EVENT_SHORT_CLICKED) {
            ((Minesweeper* ) button->user_data)->buttonShortClick(button);
            return;
        }
        if (event == LV_EVENT_LONG_PRESSED) {
            ((Minesweeper* ) button->user_data)->buttonLongPressed(button);
            return;
        }
        if (event == LV_EVENT_RELEASED || event==LV_EVENT_LEAVE) {
            ((Minesweeper* ) button->user_data)->buttonLeave(button);
            return;
        }
    }

    static const char* staticLabels[];
};
