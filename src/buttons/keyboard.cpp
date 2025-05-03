#include "keyboard.h"

void Keyboard::setup_buttons()
{
    // Buttons setup

    // Set the pin connected to the button
    button_Up.attach(this->b_up, INPUT);
    button_Down.attach(this->b_down, INPUT);
    button_Right.attach(this->b_right, INPUT);
    button_Left.attach(this->b_left, INPUT);
    button_Enter.attach(this->b_enter, INPUT);

    // Debounce interval in miliseconds
    button_Up.interval(DEBOUNCE_DELAY);
    button_Down.interval(DEBOUNCE_DELAY);
    button_Right.interval(DEBOUNCE_DELAY);
    button_Left.interval(DEBOUNCE_DELAY);
    button_Enter.interval(DEBOUNCE_DELAY);

    // Indicate that the high state corresponds to physically pressing button
    button_Up.setPressedState(HIGH);
    button_Down.setPressedState(HIGH);
    button_Right.setPressedState(HIGH);
    button_Left.setPressedState(HIGH);
    button_Enter.setPressedState(HIGH);
}

void Keyboard::update_buttons()
{
    button_Up.update();
    button_Down.update();
    button_Right.update();
    button_Left.update();
    button_Enter.update();
}
