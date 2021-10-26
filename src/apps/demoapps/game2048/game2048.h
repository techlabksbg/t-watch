/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(game2048icon);


class Game2048 : public App {
    public:
    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy();
    virtual bool processDoubleTap() { return true; } // Do not terminate App on double Tap
    
    void* getIcon() { return (void*) &game2048icon; }
    const char * getName() { return "2048"; }

    private: 
    // 16 entries, 0 means empty (contains the exponent!)
    byte* feld = nullptr;

   
};
