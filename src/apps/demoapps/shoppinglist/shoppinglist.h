#pragma once

#include "../../app.h"

LV_IMG_DECLARE(shoppinglisticon);


class ShoppingList : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide() { return true;};
    virtual bool destroy() { return true;}

    lv_obj_t* firstShoppingItem=nullptr;
    
    void* getIcon() { return (void*) &shoppinglisticon; }
    const char * getName() { return "ShoppingList"; }
};
