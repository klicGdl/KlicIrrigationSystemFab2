/******************************************************************************
 * Project: Klic Smart Irrigation System
 *
 * Description:
 * Time provider library, it provides the time from different sources
 * either from a NTP or/and from a RTC device
 *
 * License:
 *    This project is open for use, modification, and distribution for personal
 *    or educational purposes only. Commercial use is strictly prohibited
 *    without explicit permission from the author
 ******************************************************************************/
#pragma once

#include "ntp.h"
#include "rtc.h"

class TimeService
{
private:
    Ntp ntp;
    Rtc rtc;
    tm currentTime;
    bool valid;

    bool _Update()
    {
        // NTP has precedence
        if (ntp.getTime(&currentTime))
        {
            this->_updateRTC();
        }
        else if (rtc.isValid())
        {
            // if there is no NTP use RTC
            currentTime = rtc.get();
        }
        else
        {
            // no valid clock
            return false;
        }
        return true;
    }

    void _updateRTC()
    {
        // update RTC if exist
        if (rtc.isValid())
        {
            rtc.set(&currentTime);
            this->valid = true;
        }
    }

public:
    TimeService() {}
    ~TimeService(){}

    void init()
    {
        ntp.init();
        rtc.init();

        if (ntp.getTime(&currentTime))
        {
            // NTP is working
            this->_updateRTC();
        }
        else if (rtc.isValid())
        {
            // No NTP, but we do have RTC
            // Serial.println("Using RTC");
            this->valid = true;
        }
        else
        {
            // No time provider
            Serial.println("No time provider available.");
            this->valid = false;
        }
    }

    tm getTimeDate()
    {
        this->_Update();
        return currentTime;
    }

    time_t getTimeDateRaw() { return mktime(&currentTime); }
};
