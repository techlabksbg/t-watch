/*
//#include "esp_https_ota.h"
#include "certs/certs.h"
#include "Arduino.h"
#include "HttpsOTAUpdate.h"

// See https://github.com/espressif/arduino-esp32/blob/master/libraries/Update/examples/HTTPS_OTA_Update/HTTPS_OTA_Update.ino
void HttpEvent(HttpEvent_t *event) {
    switch(event->event_id) {
        case HTTP_EVENT_ERROR:
            Serial.println("Http Event Error");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            Serial.println("Http Event On Connected");
            break;
        case HTTP_EVENT_HEADER_SENT:
            Serial.println("Http Event Header Sent");
            break;
        case HTTP_EVENT_ON_HEADER:
            Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            break;
        case HTTP_EVENT_ON_FINISH:
            Serial.println("Http Event On Finish");
            break;
        case HTTP_EVENT_DISCONNECTED:
            Serial.println("Http Event Disconnected");
            break;
    }
}

bool do_firmware_upgrade() {
    static HttpsOTAStatus_t otastatus;
    HttpsOTA.onHttpEvent(HttpEvent);
    Serial.println("Starting HttpsOTA...");
    HttpsOTA.begin("https://tech-lab.ch/twatch/firmware.bin", ISRG_ROOT_X1); 
    Serial.println("   HttpsOTA started...");
    while (true) {
        otastatus = HttpsOTA.status();
        if(otastatus == HTTPS_OTA_SUCCESS) { 
            Serial.println("Firmware written successfully.");
            sleep(5);
            ESP.restart();
        } else if(otastatus == HTTPS_OTA_FAIL) { 
            Serial.println("Firmware Upgrade Fail");
            return false;
        }
    }
    return true; // not happening
}

/*
// From https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_https_ota.html
bool do_firmware_upgrade_not_working() {
    Serial.println("Attempting web update...");
    esp_http_client_config_t config;
    config.url  = "https://tech-lab.ch/twatch/firmware.bin";
    config.host = "tech-lab.ch";
    config.port = 443;
    config.cert_pem = ISRG_ROOT_X1;
    config.username = nullptr;
    config.password = nullptr;
    config.client_cert_pem = nullptr;
    config.client_key_pem = nullptr;
    config.user_agent = "ESP_HTTPS_OTA";
    config.use_global_ca_store = false;

    esp_err_t ret = esp_https_ota(&config);
    Serial.println("Web update done");
    if (ret == ESP_OK) {
        esp_restart();
    } else {
        return false;
    }
    return true;
}
*/