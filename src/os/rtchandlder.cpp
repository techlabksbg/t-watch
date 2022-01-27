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
    RTC_Date r = ttgo->rtc->getDateTime();
    Serial.printf("RTC is now %d-%02d-%02d %02d:%02d:%02d\n", r.year, r.month, r.day, r.hour, r.minute, r.second);
    ::time(now);
    Serial.printf("System (UTC) is now: %sSystem (local) %s", asctime(gmtime(now)), asctime(localtime(now)));
    // Set system time to RTC time
    ttgo->rtc->syncToSystem();  // Change system time from RTC time
    // Get system time
    ::time(now);  // Call function from the base namespace, not the RTCHandler::time
    Serial.printf("System (UTC) after sync from RTC: %sSystem (local) %s", asctime(gmtime(now)), asctime(localtime(now)));
    if (lastSync>0) {
        //Serial.println("correcting time");
        *now = (*now-lastSync)*drift+lastSync;
        Serial.printf("System (UTC) after drift correction: %sSystem (local) %s", asctime(gmtime(now)), asctime(localtime(now)));
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
    // use gmtime here!
    timeinfo  = gmtime(&now);   // returns a pointer to statically allocated struct in gmtime, shared with other functions, see https://en.cppreference.com/w/cpp/chrono/c/localtime
    Serial.printf("System time in UTC now is %s", asctime(timeinfo));
    timeinfo->tm_sec = rtcnow.second;
    timeinfo->tm_min = rtcnow.minute;
    timeinfo->tm_hour = rtcnow.hour;
    timeinfo->tm_mday = rtcnow.day;
    timeinfo->tm_mon = rtcnow.month-1;
    timeinfo->tm_year = rtcnow.year-1900;
    mktime(timeinfo);

    Serial.printf("RTC time now is %s", asctime(timeinfo));
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
    Serial.println("Setting RTC to system time");
    ttgo->rtc->syncToRtc(); // Change RTC time from system time

}

RTCHandler* rtcHandler;