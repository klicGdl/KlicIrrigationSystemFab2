#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DisplayConsts.h"

// Configure OLED
#ifndef DISPLAY_ADDR
  #define DISPLAY_ADDR 0x3C // Mosto common address used for Oled displays
#endif

class Display : public Adafruit_SSD1306
{
public:
  Display(uint8_t w, uint8_t h, TwoWire *twi = &Wire, int8_t rst_pin = -1, uint32_t clkDuring = 400000UL,
          uint32_t clkAfter = 100000UL) : Adafruit_SSD1306(w, h, &Wire, rst_pin, clkDuring, clkAfter) {};
  Display(uint8_t w, uint8_t h, int8_t mosi_pin, int8_t sclk_pin,
          int8_t dc_pin, int8_t rst_pin, int8_t cs_pin) : Adafruit_SSD1306(w, h, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin) {};

  void init(uint8_t address = DISPLAY_ADDR);
  void ShowLogo();
  void demo(uint16_t dly = 1000);

private:
  // this function is just to test the display
  void testdrawline();
};
