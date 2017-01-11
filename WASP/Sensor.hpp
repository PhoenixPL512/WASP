#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__
#include <Wire.h>
#include <stdio.h>

struct SensorsData {
  unsigned long timestamp;
  int CO;
  int LPG;
  int hydrogen;
  int ozone;
  int methane;
  int pressure;
  int humidity;
  int temperature;
  int accel[3];
  int gyro[3];
  int magnet[3];
  char gps[1500];

  void getRawData(char *buffer);
};

class SensorReader {
public:
  SensorReader();
  ~SensorReader();

  SensorsData *readData();
};
#endif
