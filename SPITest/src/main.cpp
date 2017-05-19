#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <RF24.h>
#include <nRF24L01.h>

#define SD_CS_PIN 40
File logFile;
RF24 radio(38, 39);

const uint8_t addresses[2][5] = {"JP2", "GMD"};

int serial_putc( char c, FILE * ) {
  logFile.write( c );
  return c;
}

void printf_begin(void){
  fdevopen( &serial_putc, 0 );
}

void setup() {
  SD.begin(SD_CS_PIN);
  char filename[16];
  for (unsigned int i = 1;; ++i) {
    sprintf(filename, "LOG%d.txt", i);
    if (!SD.exists(filename)) break;
  }
  logFile = SD.open(filename, FILE_WRITE);
  logFile.println("well shit");
  logFile.flush();

  printf_begin();
  radio.begin();
  radio.setAddressWidth(4);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.printDetails();
  logFile.flush();
  radio.startListening();

  logFile.write("\ntop kek\n");
  logFile.flush();
  logFile.close();
}

void loop() {

}
