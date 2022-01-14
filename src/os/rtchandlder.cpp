#include "rtchandler.h"

#include "os.h"

RTCHandler::RTCHandler() {
    if (!configJson->containsKey("timeDrift")) {
        drift = 1.0;
        (*configJson)["timeDrift"] = drift;
        saveJsonConfig();
    } else {
        drift = (*configJson)["timeDrift"];
    }
    if (!configJson->containsKey("lastRTCsync")) {
        lastSync = 0;
        (*configJson)["lastRTCSync"] = lastSync;
        saveJsonConfig();
    } else {
        lastSync = (*configJson)["lastRTCsync"];
    }
    Serial.printf("Starting RTCHandler with drift=%f, lastSync=%ld\n", drift, lastSync);
}

void RTCHandler::time(time_t* now) {
    // Set system time to RTC time
    ttgo->rtc->syncToRtc();
    // Get system time
    ::time(now);  // Call function from the base namespace, not the RTCHandler::time
    if (lastSync>0) {
        //Serial.println("correcting time");
        *now = (*now-lastSync)*drift+lastSync;
    } else {
        //Serial.println("no time correction for now");
    }
}

// set new startpoint for drift 
// and update statistics
// Precondition: System time is now correct, rtc is still off.
// Postcondition: rtc is also correct
void RTCHandler::sys2rtc() {
    time_t now;
    struct tm* timeinfo;
    ::time(&now);
    Serial.println("Get current RTC time");
    RTC_Date rtcnow = ttgo->rtc->getDateTime();
    timeinfo  = localtime(&now);   // returns a pointer to statically allocated struct in localtime, shared with other functions, see https://en.cppreference.com/w/cpp/chrono/c/localtime
    timeinfo->tm_sec = rtcnow.second;
    timeinfo->tm_min = rtcnow.minute;
    timeinfo->tm_hour = rtcnow.hour;
    timeinfo->tm_mday = rtcnow.day;
    timeinfo->tm_mon = rtcnow.month-1;
    timeinfo->tm_year = rtcnow.year-1900;
    time_t rtctime = mktime(timeinfo);

    time_t rtcdiff = (rtctime-lastSync);
    time_t realdiff = (now-lastSync);
    Serial.printf("rtctime = %ld, now=%ld, lastSync=%ld, rtcdiff=%ld, realdiff=%ld\n",rtctime, now, lastSync, rtcdiff, realdiff);

    if (lastSync!=0) {
        if (rtcdiff > 600 && rtcdiff!=realdiff) {  // At least 10 mins
            drift = ((double)(realdiff))/rtcdiff;
            Serial.printf("Computing drift: rtcdiff=%ld, realdiff-%ld, drift=%f\n", rtcdiff, realdiff, drift);
            (*configJson)["timeDrift"] = drift;
            (*configJson)["lastRTCsync"] = now;
            saveJsonConfig();
        } else {
            Serial.printf("Keeping drift: rtcdiff=%ld, realdiff=%ld, drift=%f\n", rtcdiff, realdiff, drift);
        }
    } else {
        (*configJson)["lastRTCsync"] = now;
        saveJsonConfig();
        Serial.printf("Storing lastRTCsync=%ld\n", now);
    }
    lastSync = now;
    ttgo->rtc->syncToSystem(); // Set RTC to system time
}

RTCHandler* rtcHandler;