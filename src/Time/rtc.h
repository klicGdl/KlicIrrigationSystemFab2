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

#include <Wire.h>
#include <RTClib.h>
#include <time.h>

class Rtc
{
private:
    RTC_DS1307 rtc;
    DateTime now;

public:
    Rtc() {}
    ~Rtc() {}
    int init()
    {
        if (!rtc.begin())
        {
            Serial.println("Couldn't find RTC");
            return false;
        }
        return true;
    }

    void set(int sec, int min, int hour, int day, int month, int year) { rtc.adjust(DateTime(year, month, day, hour, min, sec)); }
    void set(tm *t) { rtc.adjust(DateTime(mktime(t))); }
    void get(int *sec, int *min, int *hour, int *day, int *month, int *year)
    {
        now = rtc.now();

        *year = now.year();
        *month = now.month();
        *day = now.day();
        *hour = now.hour();
        *min = now.minute();
        *sec = now.second();
    }

    tm get()
    {
        tm t;
        time_t unixtime;
        now = rtc.now();
        unixtime = (time_t)now.unixtime();
        gmtime_r(&unixtime, &t);

        return t;
    }
    bool isEnable() { return rtc.isrunning(); }
};
