#include "shoppinglist.h"

bool ShoppingList::create() {
    register_for_swipe_up(myScr);

    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);

    itemsPage = styles.stdPage(myScr);

    lv_obj_t* quitButton = styles.stdButton(itemsPage, "Quit");
    register_for_hide_on_click(quitButton);

    title = lv_label_create(itemsPage, NULL);
    lv_obj_add_style(title, LV_OBJ_PART_MAIN, &styles.textLabel);
    lv_obj_align(title, quitButton, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 24);
    lv_label_set_text(title, "WiFi to Update");

    firstShoppingItem = lv_label_create(itemsPage, NULL); 
    lv_obj_add_style(firstShoppingItem, LV_OBJ_PART_MAIN, &styles.textLabel);
    lv_obj_align(firstShoppingItem, title, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 24);
    lv_label_set_text(firstShoppingItem, currentShoppingList.c_str());

    Serial.println("Try log shop");

    return true;
}

void ShoppingList::printHomePage(AsyncResponseStream *response) {
    response->print("<!DOCTYPE html><html><head>");
    response->print("<meta charset=\"utf-8\" />");
    response->print("<title>Shopping List</title></head><body><ul>");
    response->print("<p>Type your shopping list here:</p>");
    response->print("<form method=\"post\" action=\"/\" enctype=\"multipart/form-data\" accept-charset=\"utf-8\">");
    response->print("<textarea name=\"list\" rows=\"20\">");
    if (this->currentShoppingList != nullptr) {
        response->print(this->currentShoppingList);
    }
    response->print("</textarea>");
    response->print("<p><button type=\"submit\">Submit</button></p>");
    response->print("</form>");
    response->print("</body></html>\n");
}

void ShoppingList::registerHandlers() {
    server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        // From https://platformio.org/lib/show/6758/ESPAsyncWebServer-esphome/examples
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        this->printHomePage(response);
        request->send(response);
    });
    server->on("/", HTTP_POST, [this](AsyncWebServerRequest *request) {
        AsyncWebParameter* listParameter = request->getParam("list", true, false);
        this->currentShoppingList = listParameter->value();

        lv_label_set_text(this->firstShoppingItem, this->currentShoppingList.c_str());

        AsyncResponseStream *response = request->beginResponseStream("text/html");
        this->printHomePage(response);
        request->send(response);
    });
}

bool ShoppingList::show() {
    Serial.println("Show shopping list");

    if (!WiFi.isConnected()) {
        Serial.println("No Wifi");
    } else {
        if (server==nullptr) {
            Serial.println("ShoppingList::show(), new AsyncWebServer");
            server = new AsyncWebServer(80);
            Serial.println("register Handlers");
            registerHandlers();
            Serial.println("server->begin()");
            server->begin();
            Serial.printf("Server started: http://%s\n",WiFi.localIP().toString().c_str());
        } else {
            Serial.println("ShoppingList::show(), no new AsyncWebServer");
        }
        String header = "Connect to Update\nhttp://";
        header+=WiFi.localIP().toString();
        lv_label_set_text(title, header.c_str());
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
