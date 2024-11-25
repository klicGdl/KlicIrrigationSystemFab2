#pragma once

#include <Arduino.h>

#include "DisplayConsts.h"
#include "IDisplay.h"

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

class ST7735Display : public IDisplay
{
protected:
  Adafruit_ST7735 device;

public:
  ST7735Display(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : device(cs, dc, mosi, sclk, rst) {};
  ST7735Display(int8_t cs, int8_t dc, int8_t rst) : device(cs, dc, rst) {};

  void setCursor(int16_t x, int16_t y);
  void clearScreen(void);
  void showLogo();
  void init(uint8_t dummy);
  uint16_t width();
  uint16_t height();
  void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
  void display();
  void setTextColor(uint16_t color);
  void setTextSize(uint8_t size);
  void print(String s);
  void print(int i);
};

void ST7735Display::print(String s)
{
  device.print(s);
}

void ST7735Display::print(int i)
{
  device.print(i);
}

void ST7735Display::setTextSize(uint8_t size)
{
  device.setTextSize(size);
}

void ST7735Display::setTextColor(uint16_t color)
{
  device.setTextColor(color);
}

void ST7735Display::display()
{
  /* not needed for this library*/
}

void ST7735Display::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
  device.drawLine(x0, y0, x1, y1, color);
}

uint16_t ST7735Display::height()
{
  return device.height();
}

uint16_t ST7735Display::width()
{
  return device.width();
}

void ST7735Display::setCursor(int16_t x, int16_t y)
{
  device.setCursor(x, y);
}

void ST7735Display::clearScreen(void)
{
  device.fillScreen(ST7735_BLACK);
}

void ST7735Display::showLogo()
{
  device.fillScreen(ST7735_BLACK);
  device.drawBitmap(0, 0, bitmap_klic_logo, 128, 64, ST7735_BLUE);
}

void ST7735Display::init(uint8_t dummy)
{
  device.initR(INITR_BLACKTAB);
  device.fillScreen(ST7735_BLACK);
}