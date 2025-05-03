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
 * Usage:
 *    Rainsensor module with output digital and analog input formats
 *    a voltage comparator is widely use
 * 
 * TODO:
 *    
 ******************************************************************************/

 #include <Arduino.h>

enum class SensorStatus {
  NotInitialized,
  Ready,
  Error,
  AwaitingData
};

class Sensor {

  int Value;
  SensorStatus ValidReading;
  int AnalogInPin;

  public:

  Sensor(uint16_t Pin) {
    
    AnalogInPin = Pin;

    ValidReading = SensorStatus::AwaitingData;

    //
    // Set Desired pin for reading
    //
    pinMode(Pin, INPUT);

    //
    // 10 bits ADC counter 0 <= counts < 1023
    //
    analogReadResolution(10);
    
    //
    // Max Input voltage
    //
    analogSetAttenuation(ADC_11db);
  }

  uint16_t ReadSensor (void) {
      return analogRead (AnalogInPin);
  }
};