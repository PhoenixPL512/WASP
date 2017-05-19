#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
//#include <printf.h>

const uint8_t addresses[2][5] = {"JP2", "GMD"};
RF24 radio(5, 6);
char readbuf[128];
String data;
uint8_t len;

void setup() {
  Serial.begin(115200);
  //printf_begin();
  radio.begin();
  radio.setRetries(15, 15);
  radio.setAddressWidth(4);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.startListening();
  //radio.printDetails();
}

void loop() {
    while(!radio.available());

    len = radio.getDynamicPayloadSize();
    radio.read(readbuf, len);
    if (readbuf[0] == '@') {
      Serial.println(data);
      data = readbuf;
    } else data += readbuf;

}
