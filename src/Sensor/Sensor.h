/******************************************************************************
 * Project: Klic Smart Irrigation System
 * 
 * Description:
 *    Generic Sensor implementation
 *    
 * 
 * License:
 *    This project is open for use, modification, and distribution for personal
 *    or educational purposes only. Commercial use is strictly prohibited 
 *    without explicit permission from the author
 * 
 * Usage:
 *    
 *    
 * 
 ******************************************************************************/

 #include <Arduino.h>


enum class SensorStatus {
  AwaitingData,
  Valid,
  Invalid
};


class Sensor {
  protected:
      int value;
      SensorStatus validReading;
  
  public:
      virtual ~Sensor() = default;
  
      Sensor() : value(0), validReading(SensorStatus::AwaitingData) {}
  
      virtual uint16_t ReadSensor() = 0;
  
      SensorStatus getStatus() const {
          return validReading;
      }
  
      int getValue() const {
          return value;
      }
  };