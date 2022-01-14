/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(powerofficon);


class PowerOff : public App {

    virtual bool create() { ttgo->power->shutdown(); return false; }
    virtual bool show() {return false;}
    virtual bool hide() {return false;}
    virtual bool destroy() {return false;}
    
    void* getIcon() { return (void*) &powerofficon; }
    const char * getName() { return "Power Off"; }

};
