#pragma once
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include "DisplayConsts.h"

// Derived Class from SSD1306 driver
class KlicSSD1306 : public Adafruit_SSD1306
{
private:
    bool _backlight;
    unsigned long _lastUpdate;
    bool _autoRefresh;

public:
    // Constructores que heredan de la clase padre
    KlicSSD1306(uint8_t w, uint8_t h, TwoWire *twi = &Wire, int8_t rst_pin = -1,
                uint32_t clkDuring = 400000UL, uint32_t clkAfter = 100000UL)
        : Adafruit_SSD1306(w, h, twi, rst_pin, clkDuring, clkAfter)
    {
        _backlight = true;
        _lastUpdate = 0;
        _autoRefresh = true;
    }

    // Constructor para SPI
    KlicSSD1306(uint8_t w, uint8_t h, SPIClass *spi, int8_t dc_pin,
                int8_t rst_pin, int8_t cs_pin, uint32_t bitrate = 8000000UL)
        : Adafruit_SSD1306(w, h, spi, dc_pin, rst_pin, cs_pin, bitrate)
    {
        _backlight = true;
        _lastUpdate = 0;
        _autoRefresh = true;
    }

    // Inicialización mejorada con configuración personalizada
    bool init(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0x3C,
              bool reset = true, bool periphBegin = true)
    {
        bool result = begin(switchvcc, i2caddr, reset, periphBegin);
        if (result)
        {
            clearDisplay();
            setTextWrap(false);
            display();
        }
        return result;
    }

    // Brightness control
    void setBrightness(uint8_t brightness)
    {
        ssd1306_command(SSD1306_SETCONTRAST);
        ssd1306_command(brightness);
    }

    // Power save, turn off the backlight
    void powerSave(bool enable)
    {
        if (enable)
        {
            ssd1306_command(SSD1306_DISPLAYOFF);
            _backlight = false;
        }
        else
        {
            ssd1306_command(SSD1306_DISPLAYON);
            _backlight = true;
        }
    }

    // Center the string
    void printCentered(const String &text, int16_t y = -1)
    {
        if (y == -1)
            y = height() / 2 - 4;

        int16_t x1, y1;
        uint16_t w, h;
        getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

        int16_t x = (width() - w) / 2;
        setCursor(x, y);
        print(text);
    }

    void clearAndShow()
    {
        clearDisplay();
        display();
    }

    // fade in/out effect
    void fadeIn(unsigned long duration = 1000)
    {
        unsigned long startTime = millis();
        while (millis() - startTime < duration)
        {
            uint8_t progress = ((millis() - startTime) * 255) / duration;
            setBrightness(progress);
            delay(10);
        }
    }

    void fadeOut(unsigned long duration = 1000)
    {
        unsigned long startTime = millis();
        while (millis() - startTime < duration)
        {
            uint8_t progress = 255 - ((millis() - startTime) * 255) / duration;
            setBrightness(progress);
            delay(10);
        }
    }

    void drawFrame(int16_t x, int16_t y, int16_t w, int16_t h,
                   uint16_t color = SSD1306_WHITE, bool filled = false)
    {
        if (filled)
        {
            fillRoundRect(x, y, w, h, 3, color);
        }
        else
        {
            drawRoundRect(x, y, w, h, 3, color);
        }
    }

    // if fade is equal to zero, it will keep the logo
    // value is in miliseconds
    void showLogo(uint16_t fade = 2000)
    {
        drawBitmap(0, 0, bitmap_klic_logo, 128, 64, WHITE);
        display();
        if (fade)
        {
            fadeOut(fade);
            clearAndShow();
        }
    }

    // Print the time and date
    void printCurrentState(tm * t, uint16_t duration, uint8_t zone, bool on)
    {
        char buf[30];
        int16_t offset = -1;
        clearDisplay();
        setTextColor(WHITE);
        drawFrame(0,0,width(),height());
        if(on)
        {
            sprintf(buf,"Zone %d",zone + 1); 
            printCentered(buf,5);
            printCentered("ON",16);
            sprintf(buf,"for %d min",duration/60);
            printCentered(buf,26);
            offset = 40;
        }
        sprintf(buf,"%02d/%02d/%02d - %02d:%02d:%02d",t->tm_mday,t->tm_mon,t->tm_year,t->tm_hour,t->tm_min,t->tm_sec);
        printCentered(buf,offset);
        display();
    }
};
