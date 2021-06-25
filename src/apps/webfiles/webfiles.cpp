#include "webfiles.h"
#include "SPIFFS.h"


bool WebFiles::create() {
    Serial.println("WebFiles::create() start");
    self = this;

    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);
    title = styles.stdTitle(myScr, "WebFiles\nNot connected");
    lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0,15);

    Serial.println("Title done");
    connect = styles.stdButton(myScr, "Connect");
    lv_obj_align(connect, title, LV_ALIGN_OUT_BOTTOM_MID, 0,20);
    lv_obj_set_event_cb(connect, connect_cb);

    Serial.println("Connect button done");
    quit = styles.stdButton(myScr, "Quit");
    lv_obj_align(quit, connect, LV_ALIGN_OUT_BOTTOM_MID, 0,20);
    App::register_for_hide_on_click(quit);

    Serial.println("WebFiles::create() end");

    return true;
}

void WebFiles::registerHandlers() {
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        // From https://platformio.org/lib/show/6758/ESPAsyncWebServer-esphome/examples
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        response->print("<!DOCTYPE html><html><head><title>WebFiles</title></head><body>");
        // from https://techtutorialsx.com/2019/02/23/esp32-arduino-list-all-files-in-the-spiffs-file-system/
        fs::File root = SPIFFS.open("/");
        fs::File file = root.openNextFile();
        while(file) {
            response->print("<li><a href=\"");
            response->print(file.name());
            response->print("\">");
            response->print(file.name());
            response->print("</li>\n");
            Serial.println(file.name());        
            file = root.openNextFile();
        }

        response->print("</ul></body></html>\n");
        request->send(response);
    });

}


bool WebFiles::show() {    
    Serial.println("WebFiles::show()");
    if (!WiFi.isConnected()) {
        Serial.println("No Wifi");
        lv_label_set_text(title, "WebFiles\nNot connected");
    } else {
        if (server==nullptr) {
            Serial.println("WebFile::show(), new AsyncWebServer");
            server = new AsyncWebServer(80);
            registerHandlers();
            server->begin();
            Serial.printf("Server started: http://%s\n",WiFi.localIP().toString().c_str());
        } else {
            Serial.println("WebFile::show(), no new AsyncWebServer");
        }
        String header = "WebFiles\n";
        header+=WiFi.localIP().toString();
        lv_label_set_text(title, header.c_str());
    }
    return true;
}

bool WebFiles::hide() {
    Serial.println("WebFiles::hide()");
    if (server!=nullptr) {
        server->end();
        delete server;
        server=nullptr;
        lv_label_set_text(title, "WebFiles\nStopped");
    }
    return true;
}

WebFiles* WebFiles::self = nullptr;