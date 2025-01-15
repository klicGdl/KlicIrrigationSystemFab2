#ifndef PIO_UNIT_TESTING
#include "main.h"


#define ONE_SECOND 1000 // 1000 miliseconds

TFT_eSPI display = TFT_eSPI();

Keyboard kbrd;
ScheduleConf sch;

Menu menu(&display, &kbrd);
unsigned long lastTime = 0; // to mesure one seccond

void setup()
{
    Serial.begin(115200);

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

}

void loop()
{   

    /* Enter in this if every second*/
    if (millis() - lastTime >= ONE_SECOND)
    {
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
        }

        lastTime = millis();
    }
}

#endif
