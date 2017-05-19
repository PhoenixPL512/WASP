#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>

const uint8_t addresses[2][5] = {"JP2", "GMD"};
RF24 radio(41, 40);
char buffer[512];
const char* gps = "GPS DATA SHOULD BE HERE";
unsigned long timestamp;
size_t bufsize, gpssize;

void setup() {
  radio.begin();
  radio.setAddressWidth(4);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
}

void loop() {
  timestamp = millis();
  sprintf(buffer, "@%lu#%d|%d|%d#%d|%d|%d#%d|%d|%d#%d|%d|%d|%d|%d#%d|%d|%d|%d#%s", timestamp, 10,
          11, 12, 20, 21, 22, 30, 31, 32, 40,
          41, 42, 43, 44, 50, 51, 52, 53, gps);

  bufsize = strlen(buffer);

  for(size_t i = 0; i < bufsize; i += 32) {
    radio.write(&buffer[i], (bufsize - i >= 32 ? 32 : bufsize - i));
  }
}
