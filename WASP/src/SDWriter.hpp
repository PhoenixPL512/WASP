#ifndef __SDWRITER_HPP__
#define __SDWRITER_HPP__
#include "Sensor.hpp"
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class SDWriter {
private:
  int SPI_CS;
  File logFile;

public:
  SDWriter(int _SPI_CS);
  ~SDWriter();

  void writeData(SensorsData *data);
  void writeData(const char *text);
};
#endif
