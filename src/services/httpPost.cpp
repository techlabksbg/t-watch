/**
 * @author Ivo Blöchliger
 * 
 * Allows to post plain/text by http oder https (needs a certificate)
 */

#include "services.h"
#include "../apps/settingapps/wifimanager/wifimanager.h"
#include "../apps/launcher/launcher.h"
#include "HTTPClient.h"

#define DOWNLOAD_BUF (512)


typedef struct {
    const char* url;
    const char* root_ca; 
    const char * payload;
    HTTPClient* client=nullptr;
    WiFiClient* stream=nullptr;
    App* app=nullptr;
    char* buf=nullptr;
    int bytesSent;
    int bytesReceived;
    bool spinnerShown=false;
    int responseCode=0;
} HttpPostTaskData;

void httpPostTask(lv_task_t *data) {
    bool error = false;
    HttpPostTaskData* d = (HttpPostTaskData*) data->user_data;
    if (!d->spinnerShown) {
        d->buf = new char[DOWNLOAD_BUF];
        sprintf(d->buf, "Post to\n%s",d->url);
        styles.showSpinner(lv_scr_act(), d->buf);
        d->spinnerShown = true;
        d->client = new HTTPClient;
        Serial.println("downloadToSPIFFS(): Starting web request");
        if (d->url[4]=='s') {
            if (d->root_ca==nullptr) {
                Serial.println("ERROR: Missing root certificate for https!");
                delete d->client;
                error = true;
            } else {
                d->client->begin(d->url, d->root_ca);
            }
        } else { // http
            d->client->begin(d->url);
        }
        if (!error) {
            d->client->addHeader("Content-Type", "text/plain");
            d->responseCode = d->client->POST(d->payload);
            if (d->responseCode!=200) {
                Serial.printf("POST Response Code %d\n",d->responseCode);
                error = true;
            }
            d->bytesSent = d->client->getSize();
        }
        if (d->bytesSent<0) {
            error = true;
        }
        if (!error) {
            Serial.printf("About to get %d bytes\n", d->bytesSent);            
            d->stream = &(d->client->getStream());
            d->bytesReceived = 0;
        }
    }
    if (!error && d->bytesReceived < d->bytesSent) {
        while (d->stream->available()>0) {
            auto num = d->stream->readBytes(d->buf,DOWNLOAD_BUF);
            d->bytesReceived+=num;
            Serial.printf("Downloaded %d of %d bytes\n", d->bytesReceived, d->bytesSent);
        }
    }
    // Finished, so clean up
    if (error || d->bytesReceived == d->bytesSent) {
        if (d->client!=nullptr) {
            d->client->end();
            delete d->client;
        }
        delete[] d->buf;
        delete d;
        lv_task_del(data);
        styles.hideSpinner();
    }
}


void httpPost(App* app, const char * payload, const char * url, const char* root_ca) {
    static lv_task_t* task = nullptr;
    
    // Not connected?
    if (!WiFi.isConnected()) {
        // start the wifi dialog (and return to original app if successful).
        lv_async_call([](void* data) {
            wifiManager.connect((App*) data);
        }, app);
        return;
    }
    // WiFi is connected!
    if (task==nullptr) { // Task not already running?
        HttpPostTaskData* d = new HttpPostTaskData;
        d->url = url;
        d->root_ca = root_ca;
        d->app = app;
        d->payload = payload;
        task = lv_task_create(httpPostTask, 10, LV_TASK_PRIO_LOWEST, d);
    }
    return;
}
