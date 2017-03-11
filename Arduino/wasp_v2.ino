#include <Arduino.h>
#include <RF24.h>
#include <stdio.h>
#include "log.hpp"
#include "sensors.hpp"

char buffer[128];

RF24 radio(A2, A3);
byte addresses[][7] = {"CanSat", "RaspPi"};

void setup() {
  Serial.begin(9600);
  initSensors();
  initLog();

  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  // logFile.println("RF24");
  // logFile.println(radio.getPALevel());
  // logFile.println(radio.getDataRate());
}

void loop() {
  readSensors();

  sprintf(buffer, "[MAG]:#%d|%d|%d#[ACC]:#%d|%d|%d#[GYR]:#%d|%d|%d#", mag[0],
          mag[1], mag[2], acc[0], acc[1], acc[2], gyr[0], gyr[1], gyr[2]);
  logFile.write('@');
  logFile.print(millis());
  logFile.write(buffer);
  logFile.flush();

  logFile.write("[TPA]:#");
  logFile.print(temperature);
  logFile.write('|');
  logFile.print(pressure);
  logFile.write('|');
  logFile.print(altitude);
  logFile.write('#');
  logFile.flush();

  if (Serial.available()) {
    String gps = Serial.readStringUntil('\n');
    logFile.write("[GPS]");
    logFile.write(gps.c_str());
  } else {
    logFile.write('\n');
  }
  logFile.flush();
}