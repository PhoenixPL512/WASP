#ifndef COMMS_HPP
#define COMMS_HPP
#define PRINT_RADIO_DETAILS

#include <RF24.h>
#include <nRF24L01.h>
#ifdef PRINT_RADIO_DETAILS
  #include "log.hpp"
#endif

RF24 radio(A3, A2);

const uint8_t addresses[2][5] = {"JP2", "GMD"};

#ifdef PRINT_RADIO_DETAILS
  int serial_putc( char c, FILE * ) {
    logFile.write( c );
    return c;
  }

  void printf_begin(void){
    fdevopen( &serial_putc, 0 );
  }
#endif

void initCommunication() {
  #ifdef PRINT_RADIO_DETAILS
    printf_begin();
  #endif
  radio.begin();
  radio.setAddressWidth(4);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  #ifdef PRINT_RADIO_DETAILS
    radio.printDetails();
    logFile.write('\n');
    logFile.flush();
  #endif
//  radio.startListening();
}

void sendData(char* data, size_t length) {
  for(size_t i = 0; i < length; i += 32) {
    radio.write(&data[i], (length - i >= 32 ? 32 : length - i));
  }
}

#endif
