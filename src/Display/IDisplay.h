#pragma once

#include <Arduino.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

#include "DisplayConsts.h"

#define IDISPLAY_BLACK ST77XX_BLACK
#define IDISPLAY_WHITE ST77XX_WHITE
#define IDISPLAY_RED ST77XX_RED
#define IDISPLAY_GREEN ST77XX_GREEN
#define IDISPLAY_BLUE ST77XX_BLUE
#define IDISPLAY_CYAN ST77XX_CYAN
#define IDISPLAY_MAGENTA ST77XX_MAGENTA
#define IDISPLAY_YELLOW ST77XX_YELLOW
#define IDISPLAY_ORANGE ST77XX_ORANGE

class IDisplay
{
private:
  /* data */
public:
  virtual void init(uint8_t address = DISPLAY_ADDR);
  virtual void setCursor(int16_t x, int16_t y);
  virtual void clearScreen();
  virtual void showLogo();
  virtual uint16_t width();
  virtual uint16_t height();
  virtual void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
  virtual void display();
  virtual void setTextColor(uint16_t color);
  virtual void setTextSize(uint8_t size);
  virtual void print(String s);
  virtual void print(int i);
  virtual void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  virtual void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
};
