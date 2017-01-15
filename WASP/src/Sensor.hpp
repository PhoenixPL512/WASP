#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__
#include <Arduino.h>
#include <LIS3MDL.h>
#include <LPS331.h>
#include <LSM6.h>
#include <Wire.h>
#include <stdio.h>

struct SensorsData {
  unsigned long timestamp;
  int CO;            // attiny
  int LPG;           // attiny
  int hydrogen;      // attiny
  int ozone;         // attiny
  int methane;       // attiny
  int humidity;      // attiny
  float temperature; // LPS331
  float pressure;    // LPS331
  float altitude;    // LSP331
  int accel[3];      // LSM6
  int gyro[3];       // LSM6
  int magnet[3];     // LIS3MDL
  String gps;        // NEO-7M-C

  String getRawData();
};

class SensorReader {
private:
  LPS331 ps;
  LSM6 imu;
  LIS3MDL mag;

public:
  SensorReader();
  ~SensorReader();

  SensorsData *readData();
};
#endif
