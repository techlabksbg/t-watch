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
    virtual void lv_event_callback(lv_obj_t* obj, lv_event_t event);

    
    void* getIcon() { return (void*) &game2048icon; }
    const char * getName() { return "2048"; }

    private: 
    // 16 entries, 0 means empty (contains the exponent!)
    byte* feld = nullptr;
    // 16 entries for graphical object
    lv_obj_t** objs = nullptr;
    lv_obj_t** labels = nullptr;

    void createObjs();

    // Setzt den entsrpechenden Feldinhalt auf den entsprechenden Exponenten
    // Liegt x/y ausserhalb des Feldes, passiert nichts.
    void setFeld(int x, int y, byte value);
    // Liefert den Exponenten für ein gegebenes Feld
    // Liegt x/y ausserhalb des Feldes, wird 0 (leer) zurückgeliefert.
    byte getFeld(int x, int y);
   
    // Zeigt das aktuelle Feld an
    void showFeld();

    static const char* powers[];
    static lv_color_t colors[];

    // Called, when the user taps on screen.
    void click(int x, int y);
};
