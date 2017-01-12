#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__
#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>

struct SensorsData {
  unsigned long timestamp;
  int CO;          // attiny
  int LPG;         // attiny
  int hydrogen;    // attiny
  int ozone;       // attiny
  int methane;     // attiny
  int humidity;    // attiny
  int temperature; // attiny
  int pressure;
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
