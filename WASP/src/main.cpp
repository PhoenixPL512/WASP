//#define NO_RADIO
#include <Arduino.h>
#include <stdio.h>
#include "log.hpp"
#include "sensors.hpp"
#include "comms.hpp"

char buffer[128];
String gps;

void setup() {
  Serial.begin(9600);
  initSensors();
  initLog();
  #ifndef NO_RADIO
    initCommunication();
  #endif
}

void loop() {
  //read sensors
  readSensors();

  if (Serial.available()) {
    gps = Serial.readStringUntil('\n');;
  } else {
    gps = "";
  }

  //save it to file
  sprintf(buffer, "@%lu#%d|%d|%d#%d|%d|%d#%d|%d|%d#%d|%d|%d|%d|%d#%d|%d|%d|%d#%s\n", timestamp, mag[0],
          mag[1], mag[2], acc[0], acc[1], acc[2], gyr[0], gyr[1], gyr[2], gases[0],
          gases[1], gases[2], gases[3], gases[4], temperature, humidity, pressure, altitude, gps.c_str());
  logFile.write(buffer);
  logFile.flush();

  //send it
  #ifndef NO_RADIO
    sendData(buffer, strlen(buffer));
  #endif
}
