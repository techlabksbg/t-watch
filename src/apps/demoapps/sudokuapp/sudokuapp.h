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

    private: 
    Sudoku* sudoku;

};
