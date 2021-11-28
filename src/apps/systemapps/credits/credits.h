/**
 * @author Ivo Blöchliger
 */


#pragma once

#include "../../app.h"

LV_IMG_DECLARE(creditsicon);


class Credits : public App {

    virtual bool create();
    virtual bool show() { return true; }
    virtual bool hide() { return true; }
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &creditsicon; }
    const char * getName() { return "Credits"; }

       
};
