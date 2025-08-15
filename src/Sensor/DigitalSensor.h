/******************************************************************************
 * Project: Klic Smart Irrigation System
 * 
 * Description:
 *    Module to read rain sensors potentiometer based (digital inputs) 
 *    
 * 
 * License:
 *    This project is open for use, modification, and distribution for personal
 *    or educational purposes only. Commercial use is strictly prohibited 
 *    without explicit permission from the author
 *
 ******************************************************************************/
#include <Arduino.h>
#include "Sensor.h"

 class DigitalSensor : public Sensor {
  private:
      int digitalInPin;
  
  public:
      DigitalSensor(uint16_t pin) : digitalInPin(pin) {

          validReading = SensorStatus::AwaitingData;

          pinMode(digitalInPin, INPUT);
      }
  
      uint16_t ReadSensor() override {
          value = digitalRead(digitalInPin);
          validReading = SensorStatus::Valid;
          return value;
      }
  };