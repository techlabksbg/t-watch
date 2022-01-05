/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"
#include "sudoku.h"

LV_IMG_DECLARE(sudokuicon);


class SudokuApp : public App {
    public:
    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();
    virtual bool processDoubleTap() { return true; } // Do not terminate App on double Tap
    
    void* getIcon() { return (void*) &sudokuicon; }
    const char * getName() { return "Sudoku"; }

    void onClick(lv_obj_t *obj);

    private: 
    Sudoku* sudoku;

    void setNumber(int x, int y, byte value);
    int activex = 0;
    int activey = 0;

    enum {NORMAL, OVERLAY, KEYPAD} state = NORMAL;

    lv_obj_t** grid;
    lv_obj_t** entries;
    lv_obj_t** labels;
    void createGUI();
    void deleteGUI();
    // Register click handler on obj
    void setClick(lv_obj_t* obj);

    lv_obj_t* obase;
    lv_obj_t** oentries;
    lv_obj_t** olabels;
    void createOverlay(int x, int y);
    void destroyOverlay();


    lv_obj_t* kbase;
    lv_obj_t** kentries;
    lv_obj_t** klabels;
    void createNumberPad(int i);
    void destroyNumberPad();

    lv_style_t* grid_style;
    lv_style_t* entries_style;
    lv_style_t* labels_style;
    lv_style_t* labelsmed_style;
    lv_style_t* labelsfix_style;
    lv_style_t* labelsfixmed_style;
    lv_style_t* keypad_style;
    
    void createStyles();
    void deleteStyles();

};
