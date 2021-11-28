/**
 * @author Ivo Blöchliger
 */

#include "services.h"
#include "../apps/settingapps/wifimanager/wifimanager.h"
#include "../apps/launcher/launcher.h"
#include "HTTPClient.h"


#define DOWNLOAD_BUF (512)

typedef struct {
    const char* filename;
    const char* url;
    const char* root_ca; 
    HTTPClient* client=nullptr;
    WiFiClient* stream=nullptr;
    fs::File* file=nullptr;
    App* app=nullptr;
    char* buf=nullptr;
    int bytesSent;
    int bytesReceived;
    bool spinnerShown=false;
} DownloadTaskData;

void downloadTask(lv_task_t *data) {
    bool error = false;
    DownloadTaskData* d = (DownloadTaskData*) data->user_data;
    if (!d->spinnerShown) {
        d->buf = new char[DOWNLOAD_BUF];
        sprintf(d->buf, "Downloading\n%s",d->filename);
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
            d->client->GET();
            d->bytesSent = d->client->getSize();
        }
        if (d->bytesSent<0) {
            error = true;
        }
        if (!error) {
            Serial.printf("About to get %d bytes\n", d->bytesSent);
            d->file = new fs::File;
            *(d->file) = SPIFFS.open(d->filename, FILE_WRITE);
            d->stream = &(d->client->getStream());
            d->bytesReceived = 0;
        }
    }
    if (!error && d->bytesReceived < d->bytesSent) {
        if (d->stream->available()>0) {
            auto num = d->stream->readBytes(d->buf,DOWNLOAD_BUF);
            for (int b=0; b<num; b++) {
                d->file->write(d->buf[b]);
            }
            d->bytesReceived+=num;
            Serial.printf("Downloaded %d of %d bytes\n", d->bytesReceived, d->bytesSent);
        }
    }
    // Finished, so clean up
    if (error || d->bytesReceived == d->bytesSent) {
        if (d->client!=nullptr) {
            d->client->end();
            delete d->client;
            if (d->file!=nullptr) {
                d->file->close();
                delete d->file;
            }
        }
        delete[] d->buf;
        delete d;
        lv_task_del(data);
        styles.hideSpinner();
    }
}

bool downloadToSPIFFS(App* app, const char * filename, const char * url, const char* root_ca, bool remove) {
    static lv_task_t* task = nullptr;
    if (remove) {
        SPIFFS.remove(filename);
    }
    // All good?
    if (SPIFFS.exists(filename)) {
        task = nullptr;
        return true;
    }
    // Not connected?
    if (!WiFi.isConnected()) {
        // this will be called in the show-method, which has to terminate
        // before we can hide the app again and start the wifi dialog.
        lv_async_call([](void* data) {
            wifiManager.connect((App*) data);
        }, app);
        return false;
    }
    // WiFi is connected!
    if (task==nullptr) { // Task not already running?
        DownloadTaskData* d = new DownloadTaskData;
        d->filename = filename;
        d->url = url;
        d->root_ca = root_ca;
        d->app = app;
        task = lv_task_create(downloadTask, 10, LV_TASK_PRIO_LOWEST, d);
    }
    return false;
}
