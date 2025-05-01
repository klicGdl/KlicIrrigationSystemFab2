#ifndef PIO_UNIT_TESTING
#include "main.h"

#define ONE_SECOND 1000 // 1000 miliseconds
#define BOARD_LED           2
TFT_eSPI display = TFT_eSPI();

Keyboard kbrd;
ScheduleConf sch;
tm iTime;

Menu menu(&display, &kbrd);
unsigned long lastTime = 0; // to mesure one seccond

void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200);
    display.begin();
    display.fillScreen(TFT_RED);

    sch.init();
    schedule_t s_val;
    for (int i = 0; i < MAX_ZONES; i++)
    {
        s_val = sch.getConf(i);
        if (menu.setZoneConf(i, s_val.days, s_val.hour, s_val.minute, s_val.duration, s_val.humidity, s_val.rain, s_val.pir))
        {
            Serial.println("Error loading data to the menus");
        }
    }
    // if there is a time provider (NTP or RTC set the time in the menu)
    // TODO: Change this code to get the values from the time provider.
    iTime.tm_year = 2025 - 1900;
    iTime.tm_mday = 19;
    iTime.tm_mon = 1;
    iTime.tm_hour = 22;
    iTime.tm_min = 20;
    iTime.tm_sec = 50;
    menu.setTime(iTime);
    pinMode(2, OUTPUT);
}

void loop()
{
    //digitalWrite(BOARD_LED, 1);
    //delay(200);
    //digitalWrite(BOARD_LED, 0);
    //delay(200);
    /* Enter in this if every second*/
    if (millis() - lastTime >= ONE_SECOND)
    {
        Serial.println("bip");
        // digitalWrite(BOARD_LED, ~digitalRead(BOARD_LED));
        // come here every second
        kbrd.update_buttons();

        if (kbrd.button_Enter.pressed())
        {
            // press the enter button to call the menu
            menu.MenusSetup();
            schedule_t s;
            for (int i = 0; i < MAX_ZONES; i++)
            {
                s.days = menu.getZoneConfdays(i);
                s.hour = menu.getZoneConfhour(i);
                s.minute = menu.getZoneConfmin(i);
                s.duration = menu.getZoneConfduration(i);
                s.humidity = menu.getZoneConfHumidity();
                s.rain = menu.getZoneConfRain();
                s.pir = menu.getZoneConfPir();
                // save in memory
                sch.setConf(i, &s);
            }
            iTime = menu.getTime();
            Serial.println(&iTime, "%H:%M:%S %A, %B %d %Y");
        }

        lastTime = millis();
    }
}

#endif
