/**
 * @author Ivo Blöchliger
 * 
 * Allows to post plain/text by http oder https (needs a certificate)
 */

#include "services.h"
#include "../apps/settingapps/wifimanager/wifimanager.h"
#include "../apps/launcher/launcher.h"
#include "HTTPClient.h"


bool httpPost(const char * payload, const char * url, const char* root_ca) {
    if (!WiFi.isConnected()) {
        return false;
    }
    // WiFi is connected!
    bool error = false;
    HTTPClient http;
    //Serial.printf("httpPost(): request to %s\n",url);
    if (url[4]=='s') {
        if (root_ca==nullptr) {
            Serial.println("ERROR: Missing root certificate for https!");
            error = true;
        } else {
            http.begin(url, root_ca);
        }
    } else { // http
       http.begin(url);
    }
    if (!error) {        
        http.addHeader("Content-Type", "text/plain", false, true);
        int response = http.POST((uint8_t *)payload, strlen(payload));
        if (response !=200) {
            Serial.printf("POST Response Code %d\n",response);
            return false;
        }
        // Ignore answer
        http.end();
        return true;
    }
    return false;
}
