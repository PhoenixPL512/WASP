#include <Arduino.h>
#include <stdio.h>
#include "log.hpp"
#include "sensors.hpp"

char buffer[128];

void setup() {
  Serial.begin(9600);
  initSensors();
  initLog();
}

void loop() {
  readSensors();

  sprintf(buffer, "#%d|%d|%d#%d|%d|%d#%d|%d|%d#%d|%d|%d|%d|%d#%d|%d|%d", mag[0],
          mag[1], mag[2], acc[0], acc[1], acc[2], gyr[0], gyr[1], gyr[2], gases[0],
          gases[1], gases[2], gases[3], gases[4], temperature, pressure, altitude);
  logFile.write('@');
  logFile.print(millis());
  logFile.write(buffer);
  logFile.flush();

  if (Serial.available()) {
    String gps = Serial.readStringUntil('\n');
    logFile.write('#');
    logFile.write(gps.c_str());
  } else {
    logFile.write('\n');
  }
  logFile.flush();
}
