#pragma once

#include "../app.h"

LV_IMG_DECLARE(shoppinglisticon);


class ShoppingList : public App {

    virtual bool create();
    virtual bool show() { return true;};
    virtual bool hide() { return true;};
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &shoppinglisticon; }
    const char * getName() { return "ShoppingList"; }
};
