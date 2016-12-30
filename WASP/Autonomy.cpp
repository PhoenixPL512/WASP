#include "Autonomy.hpp"

Autonomy::Autonomy(int servoLeftPin, int servoRightPin) {
  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);
}

Autonomy::~Autonomy() {
  
}

