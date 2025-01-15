#include <unity.h>

#include "main.h"

#ifndef TFT_RESET
#define TFT_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#endif

//Adafruit_ST7735 display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, TFT_RESET);
TFT_eSPI display = TFT_eSPI();
Keyboard kbrd;
Menu menu(&display, &kbrd);

void testMenus()
{
    menu.MenusSetup();
}

//----------------
void setup()
{
    Serial.begin(115200);

    UNITY_BEGIN();
    display.begin();
    //display.initR(INITR_BLACKTAB);
    display.fillScreen(ST7735_BLACK);
    RUN_TEST(testMenus);
    display.fillScreen(ST7735_BLACK);
    UNITY_END();
}

void loop()
{
}
