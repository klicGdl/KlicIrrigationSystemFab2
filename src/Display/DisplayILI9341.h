#pragma once

#include <Arduino.h>

#include "DisplayConsts.h"
#include "IDisplay.h"


class ILI9341Display : public IDisplay
{
protected:
  Adafruit_ILI9341 device;

public:
  //ILI9341Display(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : device(cs, dc, mosi, sclk, rst) {};
  ILI9341Display(int8_t cs, int8_t dc) : device(cs, dc) {};

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

void ILI9341Display::print(String s)
{
  device.print(s);
}

void ILI9341Display::print(int i)
{
  device.print(i);
}

void ILI9341Display::setTextSize(uint8_t size)
{
  device.setTextSize(size);
}

void ILI9341Display::setTextColor(uint16_t color)
{
  device.setTextColor(color);
}

void ILI9341Display::display()
{
  /* not needed for this library*/
}

void ILI9341Display::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
  device.drawLine(x0, y0, x1, y1, color);
}

uint16_t ILI9341Display::height()
{
  return device.height();
}

uint16_t ILI9341Display::width()
{
  return device.width();
}

void ILI9341Display::setCursor(int16_t x, int16_t y)
{
  device.setCursor(x, y);
}

void ILI9341Display::clearScreen(void)
{
  device.fillScreen(ST7735_BLACK);
}

void ILI9341Display::showLogo()
{
  device.fillScreen(ST7735_BLACK);
  device.drawBitmap(0, 0, bitmap_klic_logo, 128, 64, ST7735_BLUE);
}

void ILI9341Display::init(uint8_t dummy)
{
  device.fillScreen(ST7735_BLACK);
}