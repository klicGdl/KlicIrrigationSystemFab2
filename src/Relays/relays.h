#pragma once
#include <Arduino.h>
#include <vector>

class Relay
{
public:
    // Constructor parameters
    // pin: GPIO number
    // activeLow: true if the relay module is active-low
    Relay(int pin, bool activeLow = false)
        : _pin(pin), _activeLow(activeLow) {}

    void init(bool initialOn = false)
    {
        pinMode(_pin, OUTPUT);
        set(initialOn);
        _timeOn = 0;
    }

    void on()
    {
        write(true);
        _timeOn = millis();
    }
    void off()
    {
        write(false);
        _timeOn = 0;
    }
    void set(bool on) { write(on); }
    bool isOn() const { return _statusOn; }
    uint32_t timeOnInSeconds() { return ((millis() - _timeOn) / 1000); }

    int pin() const { return _pin; }
    bool activeLow() const { return _activeLow; }

private:
    int _pin;
    bool _activeLow;
    bool _statusOn = false;
    unsigned long _timeOn;

    void write(bool on)
    {
        _statusOn = on;
        // Depending the Relay can be active on low or high
        digitalWrite(_pin, _activeLow ? !on : on);
    }
};

class RelayManager
{
public:
    // Adds a relay at yhe end of the vector and initializes it.
    // Returns its index.
    uint8_t addRelay(int pin, bool activeLow = false, bool initialOn = false)
    {
        _relays.emplace_back(pin, activeLow);
        _relays.back().init(initialOn);
        _isThereaRelayOn = false;
        return _relays.size() - 1;
    }

    // Turn on the relay, it requires the index in the vector
    void on(uint8_t i)
    {
        // avoid to have more than one relay on at the same time
        if (_isThereaRelayOn == true)
        {
            return;
        }
        _relays.at(i).on();
        _isThereaRelayOn = true;
    }
    // Turn off the relay, it requires the index in the vector
    void off(uint8_t i)
    {
        _relays.at(i).off();
        _isThereaRelayOn = false;
    }

    // returns true if the relay is on
    bool status(uint8_t i) { return _relays.at(i).isOn(); }

    // return the time the relay has been On, in seconds
    uint32_t timeOnInSeconds(uint8_t r) { return _relays.at(r).timeOnInSeconds(); }

private:
    // use a vector to store the relays
    std::vector<Relay> _relays;
    bool _isThereaRelayOn;
};
