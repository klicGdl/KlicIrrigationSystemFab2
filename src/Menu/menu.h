#pragma once

#include <Arduino.h>
// required libraries
#include "menu_manager.h"
#include "../buttons/keyboard.h"
#include <time.h>

// pins select button on encoder
#define CS_PIN 5
#define DC_PIN 19
#define UP_PIN 35
#define DN_PIN 33
#define EN_PIN 14
#define MOSI 23
#define CLK 18

#define MAX_ZONES 4

#define ROW_HEIGHT 20
#define DATA_COLUMN 100
#define TITLE_HEIGHT 20
#define MAXROWSPERSCREEN ((160 - TITLE_HEIGHT) / ROW_HEIGHT) // ST7735 is 128x160
#define ROWS MAXROWSPERSCREEN
#define TITLE_LEFT_MARGIN 40
#define TITLE_TOP_MARGIN 6
#define BORDERRADIUS 4
#define BORDERTHICKNESS 2
#define LEFTMARGIN 10
#define TOPMARGIN 5
#define MARGINBETWEENTITLEBARANDMENU 4

// set default colors
#define MENU_TEXT 0x0000          // Black
#define MENU_BACKGROUND 0xFFFF    // White
#define MENU_HIGHLIGHTTEXT 0x0000 // Black
#define MENU_HIGHLIGHT 0x157c     //
#define MENU_HIGHBORDER 0x157D    //
#define MENU_SELECTTEXT 0x0000    // Black
#define MENU_SELECT 0x07E0        // Green
#define MENU_SELECTBORDER 0x0620  // darker green
#define MENU_DISABLE 0xC618       // Grey
#define TITLE_TEXT 0xE71C         // light grey
#define TITLE_BACK 0x0812         // some blue

enum
{
    PIR_OFF,
    PIR_ACTIVE,
    PIR_STOP
};


class Menu
{
private:
    // must have variables for each menu item
    // all these are the IDs for each menu and menu options
    int MenuOptionAuto = 0;
    int MenuOptionOnOff = 0;
    int MenuOptionConf = 0;

    int ConfigurationZone1 = 0;
    int ConfigurationZone2 = 0;
    int ConfigurationZone3 = 0;
    int ConfigurationZone4 = 0;
    int ConfigurationSensors = 0;
    int ConfigurationTimeandDate = 0;
    int humiditySensor = 0;
    int rainSensor = 0;
    int pirSensor = 0;
    uint8_t hourID = 0;
    uint8_t minID = 0;
    uint8_t secID = 0;
    uint8_t dayID = 0;
    uint8_t monthID = 0;
    uint8_t yearID = 0;

    int hourConf[MAX_ZONES] = {0, 0, 0, 0};
    int minConf[MAX_ZONES] = {0, 0, 0, 0};
    uint16_t durConf[MAX_ZONES] = {0, 0, 0, 0};
    int ZoneSun[MAX_ZONES] = {0, 0, 0, 0};
    int ZoneMon[MAX_ZONES] = {0, 0, 0, 0};
    int ZoneTue[MAX_ZONES] = {0, 0, 0, 0};
    int ZoneWed[MAX_ZONES] = {0, 0, 0, 0};
    int ZoneThu[MAX_ZONES] = {0, 0, 0, 0};
    int ZoneFri[MAX_ZONES] = {0, 0, 0, 0};
    int ZoneSat[MAX_ZONES] = {0, 0, 0, 0};
    // End of IDs

    // create some selectable menu sub-items, these are lists inside a menu item
    const char *OffOnItems[2] = {"Off", "On"};
    const char *presenceSensor[3] = {"Off", "Active", "Stop"};

    // store the values set on the menu
    uint8_t days[MAX_ZONES]{0x7F, 0x7F, 0x7F, 0x7F};
    uint8_t hour[MAX_ZONES] = {23, 23, 23, 23};
    uint8_t minute[MAX_ZONES] = {10, 20, 30, 40};
    uint16_t duration[MAX_ZONES] = {5, 5, 5, 5};
    bool _humiditySensor = false;
    bool _rainSensor = false;
    uint8_t _pirSensor = 0;
    tm timeAndDate;



    TFT_eSPI *Display;
    Keyboard *btn;

    ItemMenu *MainMenu;

    // EditMenu ConfigurationMenu(&Display); // default is false, need not specify
    ItemMenu *ConfigurationMenu; // default is false, need not specify

    EditMenu *Zone1Menu;
    EditMenu *Zone2Menu;
    EditMenu *Zone3Menu;
    EditMenu *Zone4Menu;

    EditMenu *SensorsMenu;

    EditMenu *TimeDateMenu;

    void ProcessMainMenu();
    void ProcessConfMenu();
    void ProcessZone(int zone);
    void ProcessSensors();
    void ProcessTimeDate();
    void ProcessTimeAndDate();

public:
    Menu(TFT_eSPI *Display, Keyboard *btn);
    ~Menu();
    void MenusSetup();
    int setZoneConf(int zone, uint8_t days, uint8_t hour, uint8_t min, uint16_t dur, bool humidity, bool rain, uint8_t pir);
    uint8_t getZoneConfdays(int zone);
    uint8_t getZoneConfhour(int zone);
    uint8_t getZoneConfmin(int zone);
    uint16_t getZoneConfduration(int zone);
    bool getZoneConfHumidity();
    bool getZoneConfRain();
    uint8_t getZoneConfPir();
    void setTime(tm t);
    tm getTime();
};
