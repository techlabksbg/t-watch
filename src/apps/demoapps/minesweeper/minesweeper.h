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
    

    void buildButtons();
    void removeButtons();
    lv_obj_t* createButton();
    void buttonClicked(lv_obj_t* button, bool shortClick);
    void setButton(int x, int y);
    void uncover(int x, int y);

    static void button_cb(lv_obj_t* button, lv_event_t event) {
        if (event == LV_EVENT_SHORT_CLICKED) {
            ((Minesweeper* ) button->user_data)->buttonClicked(button, true);
            return;
        }
        if (event == LV_EVENT_LONG_PRESSED) {
            ((Minesweeper* ) button->user_data)->buttonClicked(button, false);
            return;
        }
    }

    static const char* staticLabels[];
};
