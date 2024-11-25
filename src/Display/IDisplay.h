#pragma once

#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Adafruit_ILI9341.h>

#include "DisplayConsts.h"

#ifndef SPI_SCK
#define SPI_SCK 18
#endif
#ifndef SPI_MISO
#define SPI_MISO 19 // Data command
#endif
#ifndef SPI_DC
#define SPI_DC 19 // Data Command
#endif
#ifndef SPI_MOSI
#define SPI_MOSI 23
#endif
#ifndef SPI_CS
#define SPI_CS 5
#endif

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
};
