
enum class SensorStatus {
  NotInitialized,
  Ready,
  Error,
  AwaitingData
};

class Sensor {

  int Value;
  SensorStatus ValidReading;

  public:

  Sensor() {
    ValidReading = SensorStatus::AwaitingData;
  }

};