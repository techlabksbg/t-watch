#include "shoppinglist.h"

static void uploadlist_cb(lv_obj_t *button, lv_event_t event) {
    if (event != LV_EVENT_SHORT_CLICKED) return;
    Serial.println("Upload list");
}

bool ShoppingList::create() {
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);

    firstShoppingItem = styles.stdLabel(myScr, "Porcini");

    lv_label_set_align(firstShoppingItem, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(firstShoppingItem, NULL, LV_ALIGN_IN_TOP_MID, 0,15);

    uploadListButton = styles.stdButton(myScr, "Upload List ", uploadlist_cb);

    lv_obj_align(uploadListButton, firstShoppingItem, LV_ALIGN_OUT_BOTTOM_MID, 0,20);

    Serial.println("Try log shop");

    return true;
}

void ShoppingList::registerHandlers() {
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        // From https://platformio.org/lib/show/6758/ESPAsyncWebServer-esphome/examples
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        response->print("<!DOCTYPE html><html><head><title>Shopping List</title></head><body><ul>");
        response->print("<p>Type your shopping list here:</p>");
        response->print("<form method=\"post\" action=\"/\">");
        response->print("<textarea name=\"list\"></textarea>");
        response->print("</form>");
        response->print("</body></html>\n");
        request->send(response);
    });
}

bool ShoppingList::show() {
    Serial.println("Show shopping list");

    if (!WiFi.isConnected()) {
        Serial.println("No Wifi");
    } else {
        if (server==nullptr) {
            Serial.println("WebFile::show(), new AsyncWebServer");
            server = new AsyncWebServer(80);
            Serial.println("register Handlers");
            registerHandlers();
            Serial.println("server->begin()");
            server->begin();
            Serial.printf("Server started: http://%s\n",WiFi.localIP().toString().c_str());
        } else {
            Serial.println("WebFile::show(), no new AsyncWebServer");
        }
    }
    return true;
}

bool ShoppingList::hide() {
     if (server!=nullptr) {
        server->end();
        delete server;
        server=nullptr;
    }
    return true;
}
