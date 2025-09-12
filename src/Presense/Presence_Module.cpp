#include "Presence_Module.h"
#include "./Sensor/DigitalSensor.h"

extern bool DetectMovement (DigitalSensor PIR);

bool DetectMovement (DigitalSensor PIR){

  int Movement = 0; 
  
  for (int Sample; Sample < SAMPLES_NUMBER; Sample++){
      Movement += PIR.ReadSensor();
  }

  if( (100 * Movement/SAMPLES_NUMBER) > PIR_TOLERANCE ){
    return 1;

  } else {
    return 0;
  }

}