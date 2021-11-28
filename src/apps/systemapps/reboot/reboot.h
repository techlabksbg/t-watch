/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../app.h"

LV_IMG_DECLARE(rebooticon);


class Reboot : public App {

    virtual bool create() { esp_restart(); return false;}
    virtual bool show() {return false;}
    virtual bool hide() {return false;}
    virtual bool destroy() {return false;}
    
    void* getIcon() { return (void*) &rebooticon; }
    const char * getName() { return "Reboot"; }

};
