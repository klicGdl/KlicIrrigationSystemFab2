#include <unity.h>

#include "main.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define TFT_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 *displayOled;
TFT_eSPI *display;

//-----------
void testDrawLineOled()
{
  int16_t index;

  displayOled->clearDisplay();
  displayOled->display();

  for (index = 0; index < displayOled->width(); index += 4)
  {
    displayOled->drawLine(0, 0, index, displayOled->height() - 1, SSD1306_WHITE); 
    displayOled->display();
    delay(1);
  }

  for (index = 0; index < displayOled->height(); index += 4)
  {
    displayOled->drawLine(0, 0, displayOled->width() - 1, index, SSD1306_WHITE);
    displayOled->display();
    delay(1);
  }
  delay(250);

  displayOled->clearDisplay();

  for (index = 0; index < displayOled->width(); index += 4)
  {
    displayOled->drawLine(0, displayOled->height() - 1, index, 0, SSD1306_WHITE);
    displayOled->display();
    delay(1);
  }

  for (index = displayOled->height() - 1; index >= 0; index -= 4)
  {
    displayOled->drawLine(0, displayOled->height() - 1, displayOled->width() - 1, index, SSD1306_WHITE);
    displayOled->display();
    delay(1);
  }
  delay(250);

  displayOled->clearDisplay();

  for (index = displayOled->width() - 1; index >= 0; index -= 4)
  {
    displayOled->drawLine(displayOled->width() - 1, displayOled->height() - 1, index, 0, SSD1306_WHITE);
    displayOled->display();
    delay(1);
  }
  for (index = displayOled->height() - 1; index >= 0; index -= 4)
  {
    displayOled->drawLine(displayOled->width() - 1, displayOled->height() - 1, 0, index, SSD1306_WHITE);
    displayOled->display();
    delay(1);
  }
  delay(250);

  displayOled->clearDisplay();

  for (index = 0; index < displayOled->height(); index += 4)
  {
    displayOled->drawLine(displayOled->width() - 1, 0, 0, index, SSD1306_WHITE);
    displayOled->display();
    delay(1);
  }
  for (index = 0; index < displayOled->width(); index += 4)
  {
    displayOled->drawLine(displayOled->width() - 1, 0, index, displayOled->height() - 1, SSD1306_WHITE);
    displayOled->display();
    delay(1);
  }

  delay(2000);
}

void testInitAndLogoOled()
{
  if (!displayOled->begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR))
  {
    Serial.println(F("SSD1306 allocation failed"));
  }

  displayOled->clearDisplay();
  displayOled->setTextColor(SSD1306_WHITE);

  // show logo
  displayOled->drawBitmap(0, 0, bitmap_klic_logo, 128, 64, SSD1306_WHITE);
  displayOled->display();
  delay(2000);
  return;
}

void testWritingOled()
{
  for (int i = 1; i < 4; i++)
  {
    displayOled->clearDisplay();
    displayOled->display();
    displayOled->setCursor(0, displayOled->height() / 4);
    displayOled->setTextColor(SSD1306_WHITE);
    displayOled->setTextSize(i);
    displayOled->print("Text size");
    displayOled->print(i);
    displayOled->display();
    delay(1000);
  }
}

//-----------------

void testWritingle()
{
  for (int i = 1; i < 4; i++)
  {
    display->fillScreen(TFT_BLACK);
    display->setCursor(0, display->height() / 2);
    display->setTextColor(TFT_ORANGE);
    display->setTextSize(i);
    display->print("Text size");
    display->print(i);
    delay(1000);
  }
}

void testDrawLine()
{
  int16_t index;

  display->fillScreen(TFT_BLACK);

  for (index = 0; index < display->width(); index += 4)
  {
    display->drawLine(0, 0, index, display->height() - 1, TFT_WHITE);
    delay(1);
  }

  for (index = 0; index < display->height(); index += 4)
  {
    display->drawLine(0, 0, display->width() - 1, index, TFT_WHITE);
    delay(1);
  }
  delay(250);

  display->fillScreen(TFT_BLACK);

  for (index = 0; index < display->width(); index += 4)
  {
    display->drawLine(0, display->height() - 1, index, 0, TFT_LIGHTGREY);
    delay(1);
  }

  for (index = display->height() - 1; index >= 0; index -= 4)
  {
    display->drawLine(0, display->height() - 1, display->width() - 1, index, TFT_CYAN);
    delay(1);
  }
  delay(250);

  display->fillScreen(TFT_BLACK);

  for (index = display->width() - 1; index >= 0; index -= 4)
  {
    display->drawLine(display->width() - 1, display->height() - 1, index, 0, TFT_YELLOW);
    delay(1);
  }
  for (index = display->height() - 1; index >= 0; index -= 4)
  {
    display->drawLine(display->width() - 1, display->height() - 1, 0, index, TFT_WHITE);
    delay(1);
  }
  delay(250);

  display->fillScreen(TFT_BLACK);

  for (index = 0; index < display->height(); index += 4)
  {
    display->drawLine(display->width() - 1, 0, 0, index, TFT_RED);
    delay(1);
  }
  for (index = 0; index < display->width(); index += 4)
  {
    display->drawLine(display->width() - 1, 0, index, display->height() - 1, TFT_PINK);
    delay(1);
  }

  delay(2000);
}

void testInitAndLogo()
{
  display->begin();
  display->fillScreen(TFT_BLACK);
  // show logo
  display->drawBitmap(0, 0, bitmap_klic_logo, 128, 64, TFT_DARKCYAN);
  delay(2000);
}

void testWriting()
{
  for (int i = 1; i < 4; i++)
  {
    display->fillScreen(TFT_BLACK);
    display->setCursor(0, display->height() / 2);
    display->setTextColor(TFT_ORANGE);
    display->setTextSize(i);
    display->print("Text size");
    display->print(i);
    delay(1000);
  }
}
//----------------
void setup()
{
  Serial.begin(115200);

  UNITY_BEGIN();
  // display = new SSD1306Display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  displayOled = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  RUN_TEST(testInitAndLogoOled);
  RUN_TEST(testDrawLineOled);
  RUN_TEST(testWritingOled);
  displayOled->clearDisplay();
  displayOled->display();

  delete displayOled;
  Serial.println("Oled display test done");

  // display = new ST7735Display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, TFT_RESET);
  display = new TFT_eSPI();
  RUN_TEST(testInitAndLogo);
  RUN_TEST(testDrawLine);
  RUN_TEST(testWriting);
  display->fillScreen(TFT_BLACK);
  delete display;
  Serial.println("TFT display test done");
  UNITY_END();
}

void loop()
{
}
