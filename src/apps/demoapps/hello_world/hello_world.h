#pragma once

#include "../../app.h"

LV_IMG_DECLARE(hello_worldicon);

class Hello_World : public App
{

    virtual bool create();
    virtual bool show();
    virtual bool hide() { return true; };
    virtual bool destroy() { return true; }

    void *getIcon() { return (void *)&hello_worldicon; }
    const char *getName() { return "Hello world"; }
    lv_obj_t *hello_world;
};
