#pragma once

#include "../../app.h"

LV_IMG_DECLARE(First_Appicon);

class First_app : public App
{

    virtual bool create();
    virtual bool show();
    virtual bool hide() { return true; };
    virtual bool destroy() { return true; }

    void *getIcon() { return (void *)&First_Appicon; }
    const char *getName() { return "First_App"; }
    lv_obj_t *hello_world;
};
