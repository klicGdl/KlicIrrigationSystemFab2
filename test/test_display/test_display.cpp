#include <unity.h>

#include "main.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define TFT_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)


// SSD1306Display display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// ST7735Display tft(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, TFT_RESET);
IDisplay *display;

void testDrawLine()
{
  int16_t index;

  display->clearScreen();

  for (index = 0; index < display->width(); index += 4)
  {
    display->drawLine(0, 0, index, display->height() - 1, IDISPLAY_WHITE);
    display->display();
    delay(1);
  }

  for (index = 0; index < display->height(); index += 4)
  {
    display->drawLine(0, 0, display->width() - 1, index, IDISPLAY_WHITE);
    display->display();
    delay(1);
  }
  delay(250);

  display->clearScreen();

  for (index = 0; index < display->width(); index += 4)
  {
    display->drawLine(0, display->height() - 1, index, 0, IDISPLAY_WHITE);
    display->display();
    delay(1);
  }

  for (index = display->height() - 1; index >= 0; index -= 4)
  {
    display->drawLine(0, display->height() - 1, display->width() - 1, index, IDISPLAY_WHITE);
    display->display();
    delay(1);
  }
  delay(250);

  display->clearScreen();

  for (index = display->width() - 1; index >= 0; index -= 4)
  {
    display->drawLine(display->width() - 1, display->height() - 1, index, 0, IDISPLAY_WHITE);
    display->display();
    delay(1);
  }
  for (index = display->height() - 1; index >= 0; index -= 4)
  {
    display->drawLine(display->width() - 1, display->height() - 1, 0, index, IDISPLAY_WHITE);
    display->display();
    delay(1);
  }
  delay(250);

  display->clearScreen();

  for (index = 0; index < display->height(); index += 4)
  {
    display->drawLine(display->width() - 1, 0, 0, index, IDISPLAY_WHITE);
    display->display();
    delay(1);
  }
  for (index = 0; index < display->width(); index += 4)
  {
    display->drawLine(display->width() - 1, 0, index, display->height() - 1, IDISPLAY_WHITE);
    display->display();
    delay(1);
  }

  delay(2000);
}

void testInitAndLogo()
{
  display->init(DISPLAY_ADDR);
  display->clearScreen();
  display->showLogo();
  delay(2000);
}

void testWriting()
{
  for (int i = 1; i < 4; i++)
  {
    display->clearScreen();
    display->display();
    display->setCursor(0,display->height()/2);
    display->setTextColor(IDISPLAY_ORANGE);
    display->setTextSize(i);
    display->print("Text size");
    display->print(i);
    display->display();
    delay(1000);
  }
}
//----------------
void setup()
{
  Serial.begin(115200);

  UNITY_BEGIN();
  display = new SSD1306Display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  RUN_TEST(testInitAndLogo);
  RUN_TEST(testDrawLine);
  RUN_TEST(testWriting);
  display->clearScreen();
  display->display();
  delete display;

  display = new ST7735Display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, TFT_RESET);
  RUN_TEST(testInitAndLogo);
  RUN_TEST(testDrawLine);
  RUN_TEST(testWriting);
  display->clearScreen();
  display->display();
  delete display;

  display = new ILI9341Display(SPI_CS, SPI_DC);
  RUN_TEST(testInitAndLogo);
  RUN_TEST(testDrawLine);
  RUN_TEST(testWriting);
  display->clearScreen();
  display->display();
  delete display;
  UNITY_END();
}

void loop()
{
}
