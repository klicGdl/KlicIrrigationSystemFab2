#pragma once

#include <Preferences.h> // this functions are to use the EEPROM

#define SUNDAY BIT6
#define MONDAY BIT5
#define TUESDAY BIT4
#define WEDNESDAY BIT3
#define THURSDAY BIT2
#define FRIDAY BIT1
#define SATURDAY BIT0
#define ALLWEEK (SUNDAY|MONDAY|TUESDAY|WEDNESDAY|THURSDAY|FRIDAY|SATURDAY)

enum
{
    ZONE1 = 0,
    ZONE2,
    ZONE3,
    ZONE4,
    MAXZONES
} zones_e;

/*
 * Day: NA | Sun | Mon | Tues | wed | thur | Fri | Sat |
 *       0    1     0      1     0     1      0     0   = 0x54
 * Hour: 24 format
 * Minutes:
 * Duration: in seconds
 */
typedef struct
{
    uint8_t days;
    uint8_t hour;
    uint8_t minute;
    uint16_t duration;
    bool rain;
    bool humidity;
    uint8_t pir;
} schedule_t;

class ScheduleConf
{
private:
    Preferences *store;
    String _zoneNames[MAXZONES] = {"zone1", "zone2", "zone3", "zone4"};
    // these are the default values if not previous configuration was done.
    schedule_t _zones[MAXZONES] = {{0x54, 23, 0, 120,false,false,0}, {0x54, 23, 5, 120,false,false,0}, {0x54, 23, 10, 120,false,false,0}, {0x54, 23, 15, 120,false,false,0}};
    String _nameSpace = "schedule";
    String _modeSpace = "mode";
    String _opMode = "opmode";
    bool mode = true;

public:
    ScheduleConf();
    ~ScheduleConf();

    void init(void);
    schedule_t getConf(int zone);
    void setConf(int zone, schedule_t *conf);
    void setMode(bool m);
    bool getMode();
};
