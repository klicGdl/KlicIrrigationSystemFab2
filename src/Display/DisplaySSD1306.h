#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "DisplayConsts.h"
#include "IDisplay.h"

// Configure OLED
#ifndef DISPLAY_ADDR
#define DISPLAY_ADDR 0x3C // Mosto common address used for Oled displays
#endif
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#endif
#ifndef OLED_RESET
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#endif

class SSD1306Display : public IDisplay
{
protected:
  Adafruit_SSD1306 device;

public:
  SSD1306Display(uint8_t w, uint8_t h, TwoWire *twi = &Wire, int8_t rst_pin = -1, uint32_t clkDuring = 400000UL,
                 uint32_t clkAfter = 100000UL) : device(w, h, &Wire, rst_pin, clkDuring, clkAfter) {};
  SSD1306Display(uint8_t w, uint8_t h, int8_t mosi_pin, int8_t sclk_pin,
                 int8_t dc_pin, int8_t rst_pin, int8_t cs_pin) : device(w, h, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin) {};

  void setCursor(int16_t x, int16_t y);
  void clearScreen(void);
  void showLogo();
  void init(uint8_t address);
  uint16_t width();
  uint16_t height();
  void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
  void display();
  void setTextColor(uint16_t color);
  void setTextSize(uint8_t size);
  void print(String s);
  void print(int i);

};

void SSD1306Display::print(String s)
{
  device.print(s);
}

void SSD1306Display::print(int i )
{
  device.print(i);
}

void SSD1306Display::setTextSize(uint8_t size)
{
  device.setTextSize(size);
}

void SSD1306Display::setTextColor(uint16_t color)
{
  if (color != SSD1306_BLACK )
  {
    color = SSD1306_WHITE;
  }
  device.setTextColor(color);
}


void SSD1306Display::display()
{
  device.display();
}

void SSD1306Display::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
  /* this is a monochrome display */
  if (color != SSD1306_BLACK )
  {
    color = SSD1306_WHITE;
  }
  device.drawLine(x0,y0,x1,y1,color);
}

uint16_t SSD1306Display::height()
{
  return device.height();
}

uint16_t SSD1306Display::width()
{
  return device.width();
}

void SSD1306Display::clearScreen(void) 
{
  device.clearDisplay(); 
}

void SSD1306Display::setCursor(int16_t x, int16_t y) 
{
  device.setCursor(x, y); 
}

void SSD1306Display::showLogo()
{
  device.clearDisplay();
  device.drawBitmap(0, 0, bitmap_klic_logo, 128, 64, WHITE);
  device.display();
}

void SSD1306Display::init(uint8_t address)
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!device.begin(SSD1306_SWITCHCAPVCC, address))
  {
    Serial.println(F("SSD1306 allocation failed"));
  }
}