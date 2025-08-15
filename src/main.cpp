#ifndef PIO_UNIT_TESTING
#include "main.h"
#include "utils/logger.h"
#include "wifi/WiFiDriver.h"

#define ONE_SECOND        1000

TFT_eSPI display = TFT_eSPI();
Keyboard kbrd;
ScheduleConf sch;
tm iTime;
AnalogSensor RainSensor(RAIN_SENSOR_PIN);
Menu menu(&display, &kbrd);
SysLogger logger(&Serial);

void TaskSensorMenu(void *pvParameters) {
    for (;;) {
        Serial.println("\nRain Sensor value");
        Serial.println(RainSensor.ReadSensor());

        kbrd.update_buttons();

        if (kbrd.button_Enter.pressed()) {
            menu.MenusSetup();
            schedule_t s;
            for (int i = 0; i < MAX_ZONES; i++) {
                s.days     = menu.getZoneConfdays(i);
                s.hour     = menu.getZoneConfhour(i);
                s.minute   = menu.getZoneConfmin(i);
                s.duration = menu.getZoneConfduration(i);
                s.humidity = menu.getZoneConfHumidity();
                s.rain     = menu.getZoneConfRain();
                s.pir      = menu.getZoneConfPir();
                sch.setConf(i, &s);
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
    WiFiDriver::WiFiInitialize();

    sch.init();
    schedule_t s_val;
    for (int i = 0; i < MAX_ZONES; i++) {
        s_val = sch.getConf(i);
        if (menu.setZoneConf(i, s_val.days, s_val.hour, s_val.minute, s_val.duration, s_val.humidity, s_val.rain, s_val.pir)) {
            Serial.println("Error loading data to the menus");
        }
    }

    iTime.tm_year = 2025 - 1900;
    iTime.tm_mday = 19;
    iTime.tm_mon  = 1;
    iTime.tm_hour = 22;
    iTime.tm_min  = 20;
    iTime.tm_sec  = 50;
    menu.setTime(iTime);

    // Create FreeRTOS tasks
    xTaskCreatePinnedToCore(TaskSensorMenu, "SensorTask", 4096, NULL, 1, NULL, 1);
}

void loop() {
    // Nothing needed here anymore — everything is handled in FreeRTOS tasks
}

#endif
