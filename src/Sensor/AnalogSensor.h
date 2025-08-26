/******************************************************************************
 * Project: Klic Smart Irrigation System
 * 
 * Description:
 *    Module to read rain sensors potentiometer based (analog and digital inputs) 
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

#define ADC_RESOLUTION 10
 class AnalogSensor : public Sensor {
  private:
      int analogInPin;
  
  public:
      AnalogSensor(uint16_t pin) : analogInPin(pin) {

          validReading = SensorStatus::AwaitingData;

          pinMode(analogInPin, INPUT);
          
          //
          // ADC resultion
          //
          analogReadResolution(ADC_RESOLUTION);
          
          //
          // Vref 0 - 3.3
          //
          analogSetAttenuation(ADC_11db);
      }
  
      uint16_t ReadSensor() override {
          value = analogRead(analogInPin);
          validReading = SensorStatus::Valid;
          return value;
      }

      uint8_t ReadSensorInPercentage() {
        value = analogRead(analogInPin);
        return map (value,0,pow(2,ADC_RESOLUTION),0,100);
      }
  };
