#include "menu.h"

Menu::Menu(TFT_eSPI *D, Keyboard *b)
{
  Display = D;
  btn = b;

  MainMenu = new ItemMenu(Display);
  ConfigurationMenu = new ItemMenu(Display); // default is false, need not specify

  Zone1Menu = new EditMenu(Display);
  Zone2Menu = new EditMenu(Display);
  Zone3Menu = new EditMenu(Display);
  Zone4Menu = new EditMenu(Display);

  SensorsMenu = new EditMenu(Display);

  TimeDateMenu = new EditMenu(Display);
}

Menu::~Menu()
{
  delete (MainMenu);
  delete (ConfigurationMenu);

  delete (Zone1Menu);
  delete (Zone2Menu);
  delete (Zone3Menu);
  delete (Zone4Menu);

  delete (SensorsMenu);

  delete (TimeDateMenu);
}

void Menu::ProcessTimeAndDate()
{
  // set an inital flag that will be used to store what menu item the user exited on
  int TimeandDateMenuOption = 1;

  // blank out the screen
  Display->fillScreen(MENU_BACKGROUND);

  TimeDateMenu->draw();

  // run the processing loop until user move selector to title bar (which becomes exit)
  // and selectes it
  while (TimeandDateMenuOption != 0)
  {

    btn->update_buttons();
    if (btn->button_Up.pressed())
    {
      TimeDateMenu->MoveUp();
    }
    // Check down
    if (btn->button_Down.pressed())
    {
      TimeDateMenu->MoveDown();
    }

    // but wait...the user pressed the button on the encoder
    if (btn->button_Enter.pressed())
    {
      TimeandDateMenuOption = TimeDateMenu->selectRow();
    }
  }

  // out of menu now time for processing

  // save value
  this->timeAndDate.tm_hour = TimeDateMenu->value[hourID];
  this->timeAndDate.tm_min = TimeDateMenu->value[minID];
  this->timeAndDate.tm_sec = TimeDateMenu->value[secID];
  this->timeAndDate.tm_mday = TimeDateMenu->value[dayID];
  this->timeAndDate.tm_mon = TimeDateMenu->value[monthID];
  this->timeAndDate.tm_year = TimeDateMenu->value[yearID] - 1900;
}

void Menu::ProcessSensors()
{
  // set an inital flag that will be used to store what menu item the user exited on
  int SensorMenuOption = 1;

  // blank out the screen
  Display->fillScreen(MENU_BACKGROUND);

  SensorsMenu->draw();

  // run the processing loop until user move selector to title bar (which becomes exit)
  // and selectes it
  while (SensorMenuOption != 0)
  {

    btn->update_buttons();
    if (btn->button_Up.pressed())
    {
      SensorsMenu->MoveUp();
    }
    // Check down
    if (btn->button_Down.pressed())
    {
      SensorsMenu->MoveDown();
    }

    // but wait...the user pressed the button on the encoder
    if (btn->button_Enter.pressed())
    {
      SensorMenuOption = SensorsMenu->selectRow();
    }
  }

  // out of menu now time for processing

  // save value
  // OFF is Zero
  this->_rainSensor = SensorsMenu->value[rainSensor];
  this->_humiditySensor = SensorsMenu->value[humiditySensor];
  this->_pirSensor = SensorsMenu->value[pirSensor];

  Serial.println("Sensors");

  Serial.print("Rain ");
  Serial.println((int)SensorsMenu->value[rainSensor]);
  Serial.print("Humidity ");
  Serial.println((int)SensorsMenu->value[humiditySensor]);
  Serial.print("PIR ");
  Serial.println((int)SensorsMenu->value[pirSensor]);
}

void Menu::ProcessZone(int zone)
{
  // set an inital flag that will be used to store what menu item the user exited on
  int ZoneMenuOption = 1;
  EditMenu *zMenu;

  // blank out the screen
  Display->fillScreen(MENU_BACKGROUND);

  // draw the main menu
  switch (zone)
  {
  case ZONE_1:
    zMenu = Zone1Menu;
    break;
  case ZONE_2:
    zMenu = Zone2Menu;
    break;
  case ZONE_3:
    zMenu = Zone3Menu;
    break;
  case ZONE_4:
    zMenu = Zone4Menu;
    break;
  default:
    break;
  }
  zMenu->draw();
  // run the processing loop until user move selector to title bar (which becomes exit)
  // and selectes it
  while (ZoneMenuOption != 0)
  {

    btn->update_buttons();
    if (btn->button_Up.pressed())
    {
      zMenu->MoveUp();
    }
    // Check down
    if (btn->button_Down.pressed())
    {
      zMenu->MoveDown();
    }
    // but wait...the user pressed the button on the encoder
    if (btn->button_Enter.pressed())
    {
      ZoneMenuOption = zMenu->selectRow();
    }
  }

  // out of menu now time for processing

  // save values
  uint8_t week;
  week = ((uint8_t)zMenu->value[ZoneSun[zone]] << 6 |
          (uint8_t)zMenu->value[ZoneMon[zone]] << 5 |
          (uint8_t)zMenu->value[ZoneTue[zone]] << 4 |
          (uint8_t)zMenu->value[ZoneWed[zone]] << 3 |
          (uint8_t)zMenu->value[ZoneThu[zone]] << 2 |
          (uint8_t)zMenu->value[ZoneFri[zone]] << 1 |
          (uint8_t)zMenu->value[ZoneSat[zone]]);
  this->days[zone] = week;

  this->hour[zone] = (int)zMenu->value[hourConf[zone]];
  this->minute[zone] = (int)zMenu->value[minConf[zone]];
  this->duration[zone] = (uint16_t)(zMenu->value[durConf[zone]] * 60); // menu shows in minutes, but save it in seconds

  Serial.println("Configuration");
  Serial.printf("Zone %d = 0x%X\n", zone + 1, this->days[zone]);
  Serial.printf("%02d:%02d - %d(%d)\n", this->hour[zone], this->minute[zone], this->duration[zone], (this->duration[zone] / 60));
}

void Menu::ProcessConfMenu()
{
  // set an inital flag that will be used to store what menu item the user exited on
  int ConfMenuOption = 1;

  // blank out the screen
  Display->fillScreen(MENU_BACKGROUND);

  // draw the main menu
  ConfigurationMenu->draw();

  // run the processing loop until user move selector to title bar (which becomes exit)
  // and selectes it
  while (ConfMenuOption != 0)
  {

    btn->update_buttons();
    if (btn->button_Up.pressed())
    {
      ConfigurationMenu->MoveUp();
    }
    // Check down
    if (btn->button_Down.pressed())
    {
      ConfigurationMenu->MoveDown();
    }

    // but wait...the user pressed the button on the encoder
    if (btn->button_Enter.pressed())
    {
      ConfMenuOption = ConfigurationMenu->selectRow();
      if (ConfMenuOption == ConfigurationZone1)
      {
        ProcessZone(ZONE_1);
      }
      else if (ConfMenuOption == ConfigurationZone2)
      {
        ProcessZone(ZONE_2);
      }
      else if (ConfMenuOption == ConfigurationZone3)
      {
        ProcessZone(ZONE_3);
      }
      else if (ConfMenuOption == ConfigurationZone4)
      {
        ProcessZone(ZONE_4);
      }
      else if (ConfMenuOption == ConfigurationSensors)
      {
        ProcessSensors();
      }
      else if (ConfMenuOption == ConfigurationTimeandDate)
      {
        ProcessTimeAndDate();
      }
      Display->fillScreen(MENU_BACKGROUND);
      ConfigurationMenu->draw();
    }
  }
}

void Menu::ProcessMainMenu()
{

  // set an inital flag that will be used to store what menu item the user exited on
  int MainMenuOption = 1;

  // blank out the screen
  Display->fillScreen(MENU_BACKGROUND);
  // draw the main menu
  MainMenu->draw();

  // run the processing loop until user move selector to title bar (which becomes exit, i.e. 0 return val)
  // and selectes it
  // note menu code can return - 1 for errors so run unitl non zero

  while (MainMenuOption != 0)
  {
    btn->update_buttons();
    if (btn->button_Up.pressed())
    {
      MainMenu->MoveUp();
    }
    // Check down
    if (btn->button_Down.pressed())
    {
      MainMenu->MoveDown();
    }

    // but wait...the user pressed the button on the encoder
    if (btn->button_Enter.pressed())
    {
      MainMenuOption = MainMenu->selectRow();

      // here is where you process accordingly
      // remember on pressing button on title bar 0 is returned and will exit the loop

      if (MainMenuOption == MenuOptionAuto)
      {
        return;
      }
      if (MainMenuOption == MenuOptionOnOff)
      {
        // if select to disable the system, grey out other options
        if (MainMenu->getEnableState(MenuOptionAuto))
        {
          MainMenu->disable(MenuOptionAuto);
          MainMenu->disable(MenuOptionConf);
        }
        else
        {
          MainMenu->enable(MenuOptionAuto);
          MainMenu->enable(MenuOptionConf);
        }
        MainMenu->draw();
      }
      // Configuration menu was selected
      if (MainMenuOption == MenuOptionConf)
      {

        ProcessConfMenu();
        Display->fillScreen(MENU_BACKGROUND);
        MainMenu->draw();
      }
    }
  }
}

void Menu::MenusSetup()
{

  Display->begin();
  Display->fillScreen(TFT_BGR);
  Display->setRotation(0);
  Display->setTextSize(1);
  /*
    init(TextColor, BackgroundColor, HighLtTextColor, HighLtColor,
    ItemROW_HEIGHT, MaxRowsPerScreen, TitleText);
  */
  MainMenu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, ROW_HEIGHT, MAXROWSPERSCREEN, "Main Menu");
  // MainMenu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Main Menu");

  // now add each item, text is what's displayed for each menu item, there are no other arguements
  // there are 3 add types
  // 1. addNI for no icon
  // 2. addMono for simple 1 color  icon each item
  // 3. add565 for displaying a color icon for each item
  MenuOptionAuto = MainMenu->addNI("Auto");
  MenuOptionOnOff = MainMenu->addNI("On/Off");
  MenuOptionConf = MainMenu->addNI("Configuration");

  // setTitleColors(TitleTextColor, TitleFillColor);
  MainMenu->setTitleColors(TITLE_TEXT, TITLE_BACK);

  // setTitleBarSize(TitleTop, TitleLeft, TitleWith, TitleHeight);
  MainMenu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);

  // setTitleTextMargins(LeftMargin, TopMargin);
  MainMenu->setTitleTextMargins(TITLE_LEFT_MARGIN, TITLE_TOP_MARGIN);

  // the library will not determine font height and such and adjust--that will be on you
  // setMenuBarMargins(LeftMargin, Width, BorderRadius, BorderThickness);
  MainMenu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);

  // the library will not determine font height and such and adjust--that will be on you
  // setItemColors(DisableTextColor, BorderColor);
  MainMenu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER);

  // setItemTextMargins(LeftMargin, TopMargin, MarginBetweenTitleBarAndMenu);
  MainMenu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);

  // end of MainMenu setup

  // ConfigurationMenu.init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT,
  //                        DATA_COLUMN, ROW_HEIGHT, ROWS, "Configuration");

  ConfigurationMenu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, ROW_HEIGHT, MAXROWSPERSCREEN, "Configuration");

  ConfigurationZone1 = ConfigurationMenu->addNI("Zone1");
  ConfigurationZone2 = ConfigurationMenu->addNI("Zone2");
  ConfigurationZone3 = ConfigurationMenu->addNI("Zone3");
  ConfigurationZone4 = ConfigurationMenu->addNI("Zone4");
  ConfigurationSensors = ConfigurationMenu->addNI("Sensors");
  ConfigurationTimeandDate = ConfigurationMenu->addNI("Time and Date");

  ConfigurationMenu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  // setTitleColors(TitleTextColor, TitleFillColor);
  ConfigurationMenu->setTitleColors(TITLE_TEXT, TITLE_BACK);

  // setTitleBarSize(TitleTop, TitleLeft, TitleWith, TitleHeight);
  ConfigurationMenu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);

  // setTitleTextMargins(LeftMargin, TopMargin);
  ConfigurationMenu->setTitleTextMargins((Display->width() - (sizeof("Configuration") * 6)) / 2, TITLE_TOP_MARGIN);

  // setItemTextMargins(LeftMargin, TopMargin, MarginBetweenTitleBarAndMenu);
  ConfigurationMenu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);

  // setItemColors(DisableTextColor, HighLt, EditSelectBorderColor);
  // ConfigurationMenu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);
  ConfigurationMenu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER);

  //------------------------- Zone 1 ----------------------------------------------------
  Zone1Menu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Zone 1");
  ZoneSun[0] = Zone1Menu->addNI("Sunday", (this->days[0] >> 6) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneMon[0] = Zone1Menu->addNI("Monday", (this->days[0] >> 5) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneTue[0] = Zone1Menu->addNI("Tuesday", (this->days[0] >> 4) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneWed[0] = Zone1Menu->addNI("Wednesday", (this->days[0] >> 3) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneThu[0] = Zone1Menu->addNI("Thursday", (this->days[0] >> 2) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneFri[0] = Zone1Menu->addNI("Friday", (this->days[0] >> 1) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneSat[0] = Zone1Menu->addNI("Saturday", (this->days[0]) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);

  hourConf[0] = Zone1Menu->addNI("Hour", hour[0], 0, 23, 1, 0, NULL);
  minConf[0] = Zone1Menu->addNI("Minute", minute[0], 0, 59, 1, 0, NULL);
  durConf[0] = Zone1Menu->addNI("Duration", duration[0] / 60, 0, 59, 1, 0, NULL); // stored in seconds, but show it in minutes

  Zone1Menu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  Zone1Menu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  Zone1Menu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  Zone1Menu->setTitleTextMargins((Display->width() - (sizeof("Zone 1") * 6)) / 2, TITLE_TOP_MARGIN);
  Zone1Menu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  Zone1Menu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-------------------------- Zone 2---------------------------------------------------
  Zone2Menu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Zone 2");
  ZoneSun[1] = Zone2Menu->addNI("Sunday", (this->days[1] >> 6) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneMon[1] = Zone2Menu->addNI("Monday", (this->days[1] >> 5) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneTue[1] = Zone2Menu->addNI("Tuesday", (this->days[1] >> 4) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneWed[1] = Zone2Menu->addNI("Wednesday", (this->days[1] >> 3) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneThu[1] = Zone2Menu->addNI("Thursday", (this->days[1] >> 2) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneFri[1] = Zone2Menu->addNI("Friday", (this->days[1] >> 1) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneSat[1] = Zone2Menu->addNI("Saturday", this->days[1] & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);

  hourConf[1] = Zone2Menu->addNI("Hour", hour[1], 0, 23, 1, 0, NULL);
  minConf[1] = Zone2Menu->addNI("Minute", minute[1], 0, 59, 1, 0, NULL);
  durConf[1] = Zone2Menu->addNI("Duration", duration[1] / 60, 0, 59, 1, 0, NULL); // stored in seconds, but show it in minutes

  Zone2Menu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  Zone2Menu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  Zone2Menu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  Zone2Menu->setTitleTextMargins((Display->width() - (sizeof("Zone 2") * 6)) / 2, TITLE_TOP_MARGIN);
  Zone2Menu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  Zone2Menu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-------------------------- Zone 3 ---------------------------------------------------
  Zone3Menu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Zone 3");
  ZoneSun[2] = Zone3Menu->addNI("Sunday", (this->days[2] >> 6) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneMon[2] = Zone3Menu->addNI("Monday", (this->days[2] >> 5) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneTue[2] = Zone3Menu->addNI("Tuesday", (this->days[2] >> 4) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneWed[2] = Zone3Menu->addNI("Wednesday", (this->days[2] >> 3) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneThu[2] = Zone3Menu->addNI("Thursday", (this->days[2] >> 2) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneFri[2] = Zone3Menu->addNI("Friday", (this->days[2] >> 1) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneSat[2] = Zone3Menu->addNI("Saturday", this->days[2] & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);

  hourConf[2] = Zone3Menu->addNI("Hour", hour[2], 0, 23, 1, 0, NULL);
  minConf[2] = Zone3Menu->addNI("Minute", minute[2], 0, 59, 1, 0, NULL);
  durConf[2] = Zone3Menu->addNI("Duration", duration[2] / 60, 0, 59, 1, 0, NULL); // stored in seconds, but show it in minutes

  Zone3Menu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  Zone3Menu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  Zone3Menu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  Zone3Menu->setTitleTextMargins((Display->width() - (sizeof("Zone 3") * 6)) / 2, TITLE_TOP_MARGIN);
  Zone3Menu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  Zone3Menu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-------------------------- Zone 4 ---------------------------------------------------
  Zone4Menu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Zone 4");
  ZoneSun[3] = Zone4Menu->addNI("Sunday", (this->days[3] >> 6) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneMon[3] = Zone4Menu->addNI("Monday", (this->days[3] >> 5) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneTue[3] = Zone4Menu->addNI("Tuesday", (this->days[3] >> 4) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneWed[3] = Zone4Menu->addNI("Wednesday", (this->days[3] >> 3) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneThu[3] = Zone4Menu->addNI("Thursday", (this->days[3] >> 2) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneFri[3] = Zone4Menu->addNI("Friday", (this->days[3] >> 1) & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneSat[3] = Zone4Menu->addNI("Saturday", this->days[3] & 0x1, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);

  hourConf[3] = Zone4Menu->addNI("Hour", hour[3], 0, 23, 1, 0, NULL);
  minConf[3] = Zone4Menu->addNI("Minute", minute[3], 0, 59, 1, 0, NULL);
  durConf[3] = Zone4Menu->addNI("Duration", duration[3] / 60, 0, 59, 1, 0, NULL); // stored in seconds, but show it in minutes

  Zone4Menu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  Zone4Menu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  Zone4Menu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  Zone4Menu->setTitleTextMargins((Display->width() - (sizeof("Zone 4") * 6)) / 2, TITLE_TOP_MARGIN);
  Zone4Menu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  Zone4Menu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-------------------------- Sensors  ---------------------------------------------------
  SensorsMenu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN - 20, ROW_HEIGHT, ROWS, "Sensors");
  humiditySensor = SensorsMenu->addNI("Humidity", this->_humiditySensor, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  rainSensor = SensorsMenu->addNI("Rain", this->_rainSensor, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  pirSensor = SensorsMenu->addNI("Presence", this->_pirSensor, 0, sizeof(presenceSensor) / sizeof(presenceSensor[0]), 1, 0, presenceSensor);

  SensorsMenu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  SensorsMenu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  SensorsMenu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  SensorsMenu->setTitleTextMargins((Display->width() - (sizeof("Sensors") * 6)) / 2, TITLE_TOP_MARGIN);
  SensorsMenu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  SensorsMenu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-------------------------- Time and Date-------------------------------------------------
  TimeDateMenu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN - 20, ROW_HEIGHT, ROWS, "Time & Date");
  hourID = TimeDateMenu->addNI("Hour", timeAndDate.tm_hour, 0, 23, 1, 0, NULL);
  minID = TimeDateMenu->addNI("Minute", timeAndDate.tm_min, 0, 59, 1, 0, NULL);
  secID = TimeDateMenu->addNI("Seconds", timeAndDate.tm_sec, 0, 59, 1, 0, NULL);
  dayID = TimeDateMenu->addNI("Day", timeAndDate.tm_mday, 1, 31, 1, 0, NULL);
  monthID = TimeDateMenu->addNI("Month", timeAndDate.tm_mon, 1, 12, 1, 0, NULL);
  yearID = TimeDateMenu->addNI("Year", timeAndDate.tm_year + 1900, 2025, 2040, 1, 0, NULL);

  TimeDateMenu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  TimeDateMenu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  TimeDateMenu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  TimeDateMenu->setTitleTextMargins((Display->width() - (sizeof("Time & Date") * 6)) / 2, TITLE_TOP_MARGIN);
  TimeDateMenu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  TimeDateMenu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-----------------------------------------------------------------------------

  ProcessMainMenu();

  // menu code done, now proceed to your code
  Display->fillScreen(MENU_TEXT); // Black
}

int Menu::setZoneConf(int zone, uint8_t days, uint8_t hour, uint8_t min, uint16_t dur, bool humidity, bool rain, uint8_t pir)
{
  if (zone >= MAX_ZONES)
  {
    // this zone does not exist
    return 1;
  }
  this->days[zone] = days;
  this->hour[zone] = hour;
  this->minute[zone] = min;
  this->duration[zone] = dur;
  // the sensors as linked to the first  zone
  // check for bad configuration
  this->_pirSensor = pir > 2 ? 0 : pir;
  this->_humiditySensor = (bool)humidity;
  this->_rainSensor = (bool)rain;
  return 0;
}

uint8_t Menu::getZoneConfdays(int zone)
{
  if (zone >= MAX_ZONES)
  {
    // this zone does not exist
    return 0xFF;
  }
  return this->days[zone];
}

uint8_t Menu::getZoneConfhour(int zone)
{
  if (zone >= MAX_ZONES)
  {
    // this zone does not exist
    return 0xFF;
  }
  return this->hour[zone];
}

uint8_t Menu::getZoneConfmin(int zone)
{
  if (zone >= MAX_ZONES)
  {
    // this zone does not exist
    return 0xFF;
  }

  return this->minute[zone];
}

uint16_t Menu::getZoneConfduration(int zone)
{
  if (zone >= MAX_ZONES)
  {
    // this zone does not exist
    return 0xFF;
  }

  return this->duration[zone];
}

bool Menu::getZoneConfHumidity()
{
  return this->_humiditySensor;
}

uint8_t Menu::getZoneConfPir()
{
  return this->_pirSensor;
}

bool Menu::getZoneConfRain()
{
  return this->_rainSensor;
}

void Menu::setTime(tm t)
{
  this->timeAndDate = t;
}

tm Menu::getTime()
{
  return this->timeAndDate;
}
