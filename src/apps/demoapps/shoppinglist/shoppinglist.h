#pragma once

#include "../../app.h"
#include "ESPAsyncWebServer.h"

LV_IMG_DECLARE(shoppinglisticon);


class ShoppingList : public App {

    virtual bool create();
    virtual bool show();
    virtual bool hide();
    virtual bool destroy() { return true;}
    
    void* getIcon() { return (void*) &shoppinglisticon; }
    const char * getName() { return "ShoppingList"; }

    void registerHandlers();

    lv_obj_t* itemsPage = nullptr;
    lv_obj_t* firstShoppingItem=nullptr;

    private:
    lv_obj_t* addEntryButton=nullptr;
    AsyncWebServer* server=nullptr;
    String currentShoppingList=String("");
    void printHomePage(AsyncResponseStream *response);
};
