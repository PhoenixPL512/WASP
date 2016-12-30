#ifndef __AUTONOMY_HPP__
#define __AUTONOMY_HPP__
#include <Servo.h>
#include "Sensor.hpp"

class Autonomy {
private:
  Servo servoLeft, servoRight;
public:
  Autonomy(int servoLeftPin, int servoRightPin);
  ~Autonomy();

  bool doAutonomicStuff(SensorsData* data);
  bool checkIfLanded(SensorsData* data);
  bool correctVelocity(SensorsData* data);
};
#endif
