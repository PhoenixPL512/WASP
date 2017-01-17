//#include "usiTwiSlave.h"
#include <avr/io.h>
#include <util/delay.h>

// main code
int main() {
  // usiTwiSlaveInit(0x69);
  DDRB = 3;
  PORTB = 3;
  while (1) {
    _delay_ms(500);
    PORTB ^= 3;
  }
  return 0;
}
