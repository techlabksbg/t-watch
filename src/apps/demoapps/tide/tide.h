#pragma once

#include "../../app.h"
#include "HTTPClient.h"

LV_IMG_DECLARE(tideicon);


class Tide : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &tideicon; }
    const char * getName() { return "Tide"; }
};
