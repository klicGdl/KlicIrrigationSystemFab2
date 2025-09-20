#ifndef PIO_UNIT_TESTING
#include "main.h"

#define ONE_SECOND 1000

KlicSSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

TFT_eSPI display = TFT_eSPI();
Keyboard kbrd;
ScheduleConf sch;
tm iTime;
RelayManager iRelays;
TimeService iTimeProvider;
AnalogSensor HumiditySensor(RAIN_SENSOR_PIN);
DigitalSensor RainSensor(RAIN_SENSOR_PIN);
Menu menu(&display, &kbrd);
SysLogger logger(&Serial);
RTC_module rtc2;

bool CheckIfIsTime(schedule_t s, tm t)
{
    if (((0x40 >> t.tm_wday) & s.days) &&
        (t.tm_hour == s.hour) &&
        (t.tm_min == s.minute))
    {
        return true;
    }
    return false;
}

// Verify if there is an action to take, either if is it time to turn on  or off a relay
void TaskCheckValvesForAction(void *pvParameters)
{
    schedule_t iSch;
    bool anyZoneIsOn = false;
    uint8_t zone;
    while (true)
    {
        if (!menu.isSystemEnable())
        {
            // if the system is disable, no need to turn off all and do nothing else
            for (uint8_t zone = 0; zone < MAX_ZONES; zone++)
            {
                iRelays.off(zone);
            }
            continue;
        }

        iTime = iTimeProvider.getTimeDate();
        for (zone = 0; zone < MAX_ZONES; zone++)
        {
            iSch = sch.getConf(zone);
            // if the relay is on, then check if needs to turn it off
            if (iRelays.status(zone))
            {
                if (iRelays.timeOnInSeconds(zone) >= (uint32_t)iSch.duration)
                {
                    iRelays.off(zone);
                    anyZoneIsOn = false;
                }
                else
                {
                    // if there is one relay in On state, do not check other ones
                    // because they are mutually excluded
                    break;
                }
            }
            // Get here either because there was not a relay On or has just turn it off
            if (RainSensor.ReadSensor())
            {
                // Sensor detected rain, no need to do anything
                break;
            }
            // if (HumiditySensor.ReadSensorInPercentage() >= 60)
            // {
            //     // if the humidity is higher than 60% do nothing
            //     break;
            // }
            // Check if it is time to turn a relay on
            if (CheckIfIsTime(iSch, iTime))
            {
                iRelays.on(zone);
                anyZoneIsOn = true;
                //  if already tunr on, no need to check for others
                break;
            }
        }
        Serial.println(&iTime, "%H:%M:%S %A, %B %d %Y");
        display2.printCurrentState(&iTime,iSch.duration,zone,anyZoneIsOn);

        vTaskDelay(pdMS_TO_TICKS(ONE_SECOND));
    }
}

void TaskSensorMenu(void *pvParameters)
{
    for (;;)
    {
        // Serial.println("\nRain Sensor value");
        // Serial.println(RainSensor.ReadSensor());

        kbrd.update_buttons();

        if (kbrd.button_Enter.pressed())
        {
            iTime = iTimeProvider.getTimeDate();
            menu.setTime(iTime);
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
    display.fillScreen(TFT_BLACK);
    display.setSwapBytes(true);
    display.pushImage(0,0,128,160,bitmap_klic_logo_color);
    WiFiDriver::WiFiInitialize();
    rtc2.init_rtc();
    while(!iTimeProvider.init())
    {
        // if there is not time provider, not need to continue.
        // but keep trying
        Serial.println("Not time no work");
        delay(100);
    }
    display2.init(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR);
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

    iTime = iTimeProvider.getTimeDate();
    menu.setTime(iTime);

    // add all the relays into the RelayManager vector
    iRelays.addRelay(RELAY1_PIN);
    iRelays.addRelay(RELAY2_PIN);
    iRelays.addRelay(RELAY3_PIN);
    iRelays.addRelay(RELAY4_PIN);
    // Parameters: Pin, activelow, initialOn, master
    iRelays.addRelay(RELAY5_PIN, true, false, true); // this is the master relay that will provide the output voltage for the others

    // the logo will show for 3 seconds and then will fade-off
    display2.showLogo(3000);
    // turn off the TFT screen
    display.fillScreen(TFT_BLACK);

    // Create FreeRTOS tasks
    xTaskCreatePinnedToCore(TaskSensorMenu, "SensorTask", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(TaskCheckValvesForAction, "Turn on/off Valves", 4096 /*2048*/, NULL, 1, NULL, 1);
}

void loop()
{
    rtc2.PrintDateInSerial();
    // Nothing needed here anymore — everything is handled in FreeRTOS tasks
}

#endif
