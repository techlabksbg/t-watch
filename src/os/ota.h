/**
 * @author Ivo Blöchliger
 */

#include "HTTPClient.h"


class WatchOTA {
    public:

    WatchOTA() { self = this; state=OFF; };

    void loop();

    enum otaType {
        ARDUINO_SERVER,
        WEB_DOWNLOAD
    } type;

    // Callback function to get status updates (as char*)
    typedef std::function<void(const char *)> status_cb_t;
    bool start(otaType type, status_cb_t status_cb);
    bool stop();

    private:

    enum {
        OFF,
        WAITING,
        UPDATING,
        DONE
    } state;

    status_cb_t status_cb = nullptr;

    bool setupDone=false;
    void setupArduinoOTA();
    bool startArduinoOTA();
    void loopArduinoOTA();
    void stopArduinoOTA();

    bool startWebOTA();
    void loopWebOTA();
    void stopWebOTA();

    static WatchOTA* self;
    static void ota_loop() {
        self->loop();
    }

    typedef struct {
        const char* filename;
        const char* url;
        const char* root_ca; 
        HTTPClient* client=nullptr;
        WiFiClient* stream=nullptr;
        char* buf=nullptr;
        int bytesSent;
        int bytesReceived;
        enum {START, DOWNLOADING, FAIL, DONE} status = START;
    } DownloadTaskData;    
    DownloadTaskData* d = nullptr;

};

extern WatchOTA* watchOTA;