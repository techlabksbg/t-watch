#include "rtchandler.h"

#include "os.h"

RTCHandler::RTCHandler() {

}

void RTCHandler::time(time_t* now) {
    // Sync rtc to system
    ttgo->rtc->syncToSystem();
    // Get system time
    time(now);
    if (lastSync>0) {
        *now = (*now-lastSync)*drift+lastSync;
    }
}

void RTCHandler::syncedNow() {
    time_t now;
    struct tm* timeinfo;
    time(&now);
    RTC_Date rtcnow = ttgo->rtc->getDateTime();
    timeinfo  = localtime(&now);
    timeinfo->tm_sec = rtcnow.second;
    timeinfo->tm_min = rtcnow.minute;
    timeinfo->tm_hour = rtcnow.hour;
    timeinfo->tm_mday = rtcnow.day;
    timeinfo->tm_mon = rtcnow.month;
    timeinfo->tm_year = rtcnow.year-1900;
    time_t rtctime = mktime(timeinfo);

    time_t rtcdiff = (rtctime-lastSync);
    time_t realdiff = (now-lastSync);
    if (rtcdiff > 600 && rtcdiff!=realdiff) {
        double driftEst = ((double)(now-lastSync))/(rtctime-lastSync);
    }

    

}