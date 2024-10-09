#include "Display.h"

void Display::init(uint8_t address)
{
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!this->begin(SSD1306_SWITCHCAPVCC, address))
    {
        Serial.println(F("SSD1306 allocation failed"));
    }

    // Clear the buffer
    this->clearDisplay();
    // Draw the logo
    this->ShowLogo();
    delay(3000);

    Serial.println(F("Display Initialized"));
}

void Display::ShowLogo()
{
    this->drawBitmap(0, 0, bitmap_klic_logo, 128, 64, WHITE);
    this->display();
}

void Display::demo(uint16_t dly)
{
    this->testdrawline();
    delay(dly / 2);
    this->clearDisplay();
    this->ShowLogo();
    delay(dly / 2);
}

void Display::testdrawline()
{
    int16_t i;

    clearDisplay(); // Clear display buffer

    for (i = 0; i < width(); i += 4)
    {
        drawLine(0, 0, i, height() - 1, WHITE);
        display(); // Update screen
        delay(1);
    }
    for (i = 0; i < height(); i += 4)
    {
        drawLine(0, 0, width() - 1, i, WHITE);
        display();
        delay(1);
    }
    delay(250);

    clearDisplay();

    for (i = 0; i < width(); i += 4)
    {
        drawLine(0, height() - 1, i, 0, WHITE);
        display();
        delay(1);
    }
    for (i = height() - 1; i >= 0; i -= 4)
    {
        drawLine(0, height() - 1, width() - 1, i, WHITE);
        display();
        delay(1);
    }
    delay(250);

    clearDisplay();

    for (i = width() - 1; i >= 0; i -= 4)
    {
        drawLine(width() - 1, height() - 1, i, 0, WHITE);
        display();
        delay(1);
    }
    for (i = height() - 1; i >= 0; i -= 4)
    {
        drawLine(width() - 1, height() - 1, 0, i, WHITE);
        display();
        delay(1);
    }
    delay(250);

    clearDisplay();

    for (i = 0; i < height(); i += 4)
    {
        drawLine(width() - 1, 0, 0, i, WHITE);
        display();
        delay(1);
    }
    for (i = 0; i < width(); i += 4)
    {
        drawLine(width() - 1, 0, i, height() - 1, WHITE);
        display();
        delay(1);
    }

    delay(2000); // Pause for 2 seconds
}

