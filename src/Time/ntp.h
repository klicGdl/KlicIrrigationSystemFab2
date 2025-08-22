/******************************************************************************
 * Project: Klic Smart Irrigation System
 *
 * Description:
 * NTP class to obtain current time
 *
 * License:
 *    This project is open for use, modification, and distribution for personal
 *    or educational purposes only. Commercial use is strictly prohibited
 *    without explicit permission from the author
 ******************************************************************************/
#pragma once

#include <time.h>

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -21600; // offset to GMT in seconds, Mexico is -6 hours
const int daylightOffset_sec = 0;

class Ntp
{
private:
    bool valid;

public:
    Ntp() {}
    ~Ntp() {}
    void init() { configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); }

    int getTime(tm *t)
    {
        if (!getLocalTime(t))
        {
            // Serial.println("fail to get the time");
            this->valid = false;
            return false;
        }
        this->valid = true;
        return true;
    }

    bool isValid() { return this->valid; }
};
