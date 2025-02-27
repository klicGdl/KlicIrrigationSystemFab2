#pragma once

#include <Arduino.h>
#include <Bounce2.h>

#ifndef BUTTONUP
#define BUTTONUP 35
#endif
#ifndef BUTTONDOWN
#define BUTTONDOWN 33
#endif
#ifndef BUTTONRIGHT
#define BUTTONRIGHT 34
#endif
#ifndef BUTTONLEFT
#define BUTTONLEFT 26
#endif
#ifndef BUTTONENTER
#define BUTTONENTER 14
#endif
#ifndef DEBOUNCE_DELAY
#define DEBOUNCE_DELAY 5 // Time in miliseconds
#endif

enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    MAX_BUTTONS
};

class Keyboard
{
private:
    uint8_t b_up = BUTTONUP;
    uint8_t b_down = BUTTONDOWN;
    uint8_t b_left = BUTTONLEFT;
    uint8_t b_right = BUTTONRIGHT;
    uint8_t b_enter = BUTTONENTER;

    void setup_buttons();

public:
    // Create the instace for each button
    Bounce2::Button button_Up;
    Bounce2::Button button_Down;
    Bounce2::Button button_Right;
    Bounce2::Button button_Left;
    Bounce2::Button button_Enter;

    Keyboard(uint8_t b_up, uint8_t b_down, uint8_t b_left, uint8_t b_right, uint8_t b_enter)
    {
        this->b_up = b_up;
        this->b_down = b_down;
        this->b_left = b_left;
        this->b_right = b_right;
        this->b_enter = b_enter;
        this->button_Up = Bounce2::Button();
        this->button_Down = Bounce2::Button();
        this->button_Left = Bounce2::Button();
        this->button_Right = Bounce2::Button();
        this->button_Enter = Bounce2::Button();

        this->setup_buttons();
    };
    Keyboard()
    {
        this->button_Up = Bounce2::Button();
        this->button_Down = Bounce2::Button();
        this->button_Left = Bounce2::Button();
        this->button_Right = Bounce2::Button();
        this->button_Enter = Bounce2::Button();
        this->setup_buttons();
    };
    ~Keyboard() {};
    void update_buttons();
};
