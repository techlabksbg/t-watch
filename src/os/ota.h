/**
 * @author Ivo Blöchliger
 */

class WatchOTA {
    public:

    WatchOTA() { self = this; state=OFF; };

    void loop();

    enum otaType {
        ARDUINO_SERVER,
        WEB_DOWNLOAD
    } type;

    // Callback function to get status updates (as char*)
    typedef void status_cb_t(const char *);
    bool start(otaType type, status_cb_t status_cb);
    bool stop();

    private:

    enum {
        OFF,
        WAITING,
        UPDATING,
        DONE
    } state;

    status_cb_t* status_cb = nullptr;

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

};

extern WatchOTA* watchOTA;