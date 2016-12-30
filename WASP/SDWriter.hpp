#ifndef __SDWRITER_HPP__
#define __SDWRITER_HPP__
#include "Sensor.hpp"

class SDWriter {
private:
  int SPI_CS;
public:
  SDWriter(int _SPI_CS);
  ~SDWriter();

  void createNewFile();
  void writeData(SensorsData* data);
};
#endif
