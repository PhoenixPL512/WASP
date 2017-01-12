#define __AVR_ATtiny24__
#define F_CPU 80000000
#include "usi_i2c_slave.h"

int main() {
  USI_I2C_Init(0x69);
  return 0;
}
