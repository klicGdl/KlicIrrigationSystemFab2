#pragma once

#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>

#define SPI_SCK 18
#define SPI_MISO 19  // Data command
#define SPI_DC 19    // Data Command
#define SPI_MOSI 23
#define SPI_CS 5

class DisplayTft : public Adafruit_ST7735 {
 public:
  DisplayTft(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
      : Adafruit_ST7735(cs, dc, mosi, sclk, rst) {};
  DisplayTft(int8_t cs, int8_t dc, int8_t rst)
      : Adafruit_ST7735(cs, dc, rst) {};
};
