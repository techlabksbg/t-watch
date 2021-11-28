/**
 * @author Ivo Blöchliger
 */

#include "webfiles.h"
#include "SPIFFS.h"


bool WebFiles::create() {
    Serial.println("WebFiles::create() start");
    self = this;

    lv_obj_t* bg = styles.stdBgImage(myScr);
    title = styles.stdTitle(myScr, "WebFiles\nNot connected");
    lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0,15);

    Serial.println("Title done");
    connect = styles.stdButton(bg, "Connect");
    lv_obj_align(connect, title, LV_ALIGN_OUT_BOTTOM_MID, 0,20);
    lv_obj_set_event_cb(connect, connect_cb);

    Serial.println("Connect button done");
    quit = styles.stdButton(bg, "Quit");
    lv_obj_align(quit, connect, LV_ALIGN_OUT_BOTTOM_MID, 0,20);
    App::register_for_hide_on_click(quit);

    Serial.println("WebFiles::create() end");

    return true;
}

void WebFiles::registerHandlers() {

    // See https://github.com/me-no-dev/ESPAsyncWebServer for more examples etc.

    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        // From https://platformio.org/lib/show/6758/ESPAsyncWebServer-esphome/examples
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        response->print("<!DOCTYPE html><html><head><title>WebFiles</title></head><body><ul>");
        response->print("<li><a href=\"/files/\">Browse Files</a></li>");
        response->print("<li>Upload Files:</li>");
        response->print("</ul><form action=\"/upload\" method=\"post\" enctype=\"multipart/form-data\"><input type=\"file\" id=\"myfile\" name=\"myfile\"><input type=\"submit\"></form>");
        response->print("</body></html>\n");
        request->send(response);
    });
    server->on("/files/", HTTP_GET, [](AsyncWebServerRequest *request) {
        // From https://platformio.org/lib/show/6758/ESPAsyncWebServer-esphome/examples
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        response->print("<!DOCTYPE html><html><head><title>WebFiles</title></head><body>");
        // from https://techtutorialsx.com/2019/02/23/esp32-arduino-list-all-files-in-the-spiffs-file-system/
        fs::File root = SPIFFS.open("/");
        fs::File file = root.openNextFile();
        while(file) {
            response->print("<li><a href=\"/files");
            response->print(file.name());
            response->print("\">");
            response->print(file.name());
            // TODO: FIXME: correctly url-encode file names.
            response->print("</a> &nbsp; <a href=\"/delete?filename=");
            response->print(file.name());
            response->print("\">delete</a>");
            response->print("</li>\n");
            Serial.println(file.name());        
            file = root.openNextFile();
        }

        response->print("</ul></body></html>\n");
        request->send(response);
    });
    server->on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {}, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        static fs::File file;
        if (!index) {
            Serial.print("Upoad URL = ");
            Serial.println(request->url());
            file = SPIFFS.open(String("/") + filename, FILE_WRITE);
            if (!file) {
                Serial.print("Could not open file ");
                Serial.println(filename);
            }
            Serial.printf("UploadStart: %s\n", filename.c_str());
        }
        for (size_t i = 0; i < len; i++) {
            if (file) {
                Serial.print((char)data[i]);
                file.write(data[i]);
            }
        }
        if (final) {
            if (file) {
                file.close();
                request->redirect("/");
                //request->send(200, "text/plain", "Upload complete");
                if (filename == "config.json") {
                    loadJsonConfig();
                    Serial.println("Please reboot...");
                }
            } else {
                request->redirect("/");
                //request->send(200, "text/plain", "Upload FAILED!");
            }
            Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
        }
    });

    server->on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("on /delete");
        if(request->hasParam("filename")) {
            // TODO: FIXME: correctly decode url-encoded file names
            Serial.println(request->getParam("filename")->value());
            SPIFFS.remove(request->getParam("filename")->value());
        }
        request->redirect("/files/");
    });


    server->serveStatic("/files/", SPIFFS, "/");


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
            Serial.println("register Handlers");
            registerHandlers();
            Serial.println("server->begin()");
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