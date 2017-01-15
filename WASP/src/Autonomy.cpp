#include "Autonomy.hpp"

Autonomy::Autonomy(int servoLeftPin, int servoRightPin) {
  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);
}

Autonomy::~Autonomy() {}

void Autonomy::checkServos() {
  for (int i = 45; i <= 135; ++i) {
    servoLeft.write(i);
    servoRight.write(i);
    delay(10);
  }
}
