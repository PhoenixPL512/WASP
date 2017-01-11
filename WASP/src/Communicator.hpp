#ifndef __COMMUNICATOR_HPP__
#define __COMMUNICATOR_HPP__
#include "Sensor.hpp"
#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>

class Communicator {
private:
  int SPI_CE, SPI_CS;
  RF24 *radio;
  const uint64_t pipe = 0xDEADBEEF69;

public:
  Communicator(int _SPI_CE, int SPI_CS);
  ~Communicator();

  bool tryConnect();
  bool checkConnection();
  void sendData(SensorsData *data);
};
#endif
