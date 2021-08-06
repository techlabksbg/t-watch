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
     * Value 9 for a mine (+16 if show)
     */
    byte** feld = nullptr;
    int size = 0;
    void initFeld(int mines=20);
    void dumpFeld();

};
