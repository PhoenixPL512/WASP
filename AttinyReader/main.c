#include <avr/io.h>
//#include "usiTwiSlave.h"
#include <util/delay.h>

int main() {

    DDRB = 0x03;
    PORTB = 0x03;
    //usiTwiSlaveInit(0x69);
    while(1) {
        PORTB ^= 0x03;
        _delay_ms(500);
    }
    return 0;
}
