/*Ww10.4 1:08 AM Im creating this module to enable RTC in the Irrigation ssystem
I need to review functionality and it it compiles enough for today.
*/

#include <Wire.h>
#include <RTClib.h>



#include <time.h>

class RTC_module
{
private:
    /* data */
    tm date_time;
    RTC_DS1307 rtc;
    bool valid;

public:
    RTC_module(/* args */);
    ~RTC_module();
    tm getdatetime();
    void init();
    void setdatetime(tm t);
    bool isValid();
};
