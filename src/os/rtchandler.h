#pragma once

#include <time.h>

class RTCHandler {
    public:
    RTCHandler();

    // get the corrected time
    void time(time_t* now);

    // set new startpoint for drift 
    // and update statistics
    // Precondition: System time is now correct, rtc is still off.
    // Postcondition: rtc is also correct
    void sys2rtc();

    private:
    double drift=1.0;
    time_t lastSync = 0;
};

extern RTCHandler* rtcHandler;