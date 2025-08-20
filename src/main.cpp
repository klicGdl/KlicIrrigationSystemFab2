#ifndef PIO_UNIT_TESTING
#include "main.h"

#define ONE_SECOND 1000

TFT_eSPI display = TFT_eSPI();
Keyboard kbrd;
ScheduleConf sch;
tm iTime;
RelayManager iRelays;
AnalogSensor HumiditySensor(RAIN_SENSOR_PIN);
DigitalSensor RainSensor(RAIN_SENSOR_PIN);
Menu menu(&display, &kbrd);

// Verify if there is an action to take, either if is it time to turn on  or off a relay
void TaskCheckValvesForAction(void *pvParameters)
{
    schedule_t iSch;
    for (uint8_t zone = 0; zone < MAX_ZONES; zone++)
    {
        iSch = sch.getConf(zone);
        // if the relay is on, then check if needs to turn it off
        if (iRelays.status(zone))
        {
            if (iRelays.timeOnInSeconds(zone) >= (uint32_t)iSch.duration)
            {
                iRelays.off(zone);
            }
            else
            {
                // if there is one relay in On state, do not check other ones
                // because they are mutually excluded
                return;
            }
        }
        // Get here either because there was not a relay On or has just turn it off
        if (RainSensor.ReadSensor())
        {
            // Sensor detected rain, no need to do anything
            return;
        }
        // TODO: Need a time provider in orde to check if it is time to turn a relay on
    }
    vTaskDelay(pdMS_TO_TICKS(ONE_SECOND)); 
}

void TaskSensorMenu(void *pvParameters)
{
    for (;;)
    {
        Serial.println("\nRain Sensor value");
        Serial.println(RainSensor.ReadSensor());

        kbrd.update_buttons();

        if (kbrd.button_Enter.pressed())
        {
            menu.MenusSetup();
            schedule_t s;
            for (int zone = 0; zone < MAX_ZONES; zone++)
            {
                s.days = menu.getZoneConfdays(zone);
                s.hour = menu.getZoneConfhour(zone);
                s.minute = menu.getZoneConfmin(zone);
                s.duration = menu.getZoneConfduration(zone);
                s.humidity = menu.getZoneConfHumidity();
                s.rain = menu.getZoneConfRain();
                s.pir = menu.getZoneConfPir();
                sch.setConf(zone, &s);
            }
            iTime = menu.getTime();
            Serial.println(&iTime, "%H:%M:%S %A, %B %d %Y");
        }

        vTaskDelay(ONE_SECOND / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(115200);
    display.begin();
    display.fillScreen(TFT_NAVY);

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

    iTime.tm_year = 2025 - 1900;
    iTime.tm_mday = 19;
    iTime.tm_mon = 1;
    iTime.tm_hour = 22;
    iTime.tm_min = 20;
    iTime.tm_sec = 50;
    menu.setTime(iTime);

    // add all the relays into the RelayManager vector
    iRelays.addRelay(RELAY1);
    iRelays.addRelay(RELAY2);
    iRelays.addRelay(RELAY3);
    iRelays.addRelay(RELAY4);
    iRelays.addRelay(RELAY5, false, false, true); // this is the master relay that will provide the output voltage for the others

    // Create FreeRTOS tasks
    xTaskCreatePinnedToCore(TaskSensorMenu, "SensorTask", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(TaskCheckValvesForAction, "Turn on/off Valves", 2048, NULL, 1, NULL, 1);
}

void loop()
{
    // Nothing needed here anymore — everything is handled in FreeRTOS tasks
}

#endif
