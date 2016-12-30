#ifndef __COMMUNICATOR_HPP__
#define __COMMUNICATOR_HPP__
#include "Sensor.hpp"

class Communicator {
private:
  int SPI_CE, SPI_CS;
public:
  Communicator(int _SPI_CE, int SPI_CS);
  ~Communicator();

  bool tryConnect();
  bool checkConnection();
  void sendData(SensorsData* data);
};
#endif
