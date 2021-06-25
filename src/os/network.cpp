#include "os.h"
#include "../apps/settingapps/wifimanager/wifimanager.h"

// See https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiClientEvents/WiFiClientEvents.ino


void setupNetwork()
{
    WiFi.mode(WIFI_STA);
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        xEventGroupClearBits(g_event_group, G_EVENT_WIFI_CONNECTED);
        Serial.println("SYSTEM_EVENT_STA_DISCONNECTED");
        wifiManager.connectionLost(); // Bad ssid/pw
    }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        uint8_t data = Q_EVENT_WIFI_SCAN_DONE;
        xQueueSend(g_event_queue_handle, &data, portMAX_DELAY);
    }, WiFiEvent_t::SYSTEM_EVENT_SCAN_DONE);

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        xEventGroupSetBits(g_event_group, G_EVENT_WIFI_CONNECTED);
    }, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED);


    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        wifiManager.connectionEstablished();
    }, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        Serial.println("SYSTEM_EVENT_STA_LOST_IP");
        wifiManager.connectionLost();
    }, WiFiEvent_t::SYSTEM_EVENT_STA_LOST_IP);

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        Serial.println("SYSTEM_EVENT_STA_STOP");
        wifiManager.connectionLost();
    }, WiFiEvent_t::SYSTEM_EVENT_STA_STOP);

    
}

