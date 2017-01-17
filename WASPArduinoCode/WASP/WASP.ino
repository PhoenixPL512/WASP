#include "LPS331.h"
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <stdio.h>

// global vars
File file;
LPS331 lps;

// structures
struct SensorData {
  float pressure;
  float temperature;
  float attitude;
};

// SD functions
inline void writeInfo(String text) {
  file.print(millis());
  file.print(" ");
  file.println(text);
  file.flush();
}

inline void writeSensorInfo() {
  file.print(millis());
  file.println("Sensors info: ");

  file.print("temperatue: ");
  file.close();
  // HALT
  char test[10];
  sprintf(test, "%f", lps.readTemperatureC());
  file = SD.open("log.txt", FILE_WRITE);
  file.println(test);

  file.flush();
}

inline void openLog() {
  SD.begin(A1);
  file = SD.open("log.txt", FILE_WRITE);
  file.println("BEGINNING OF NEW LOG!");
}

inline void initSensors() {
  lps.init();
  lps.enableDefault();
}

void setup() {
  openLog();
  initSensors();
  writeInfo("Sensors initialised!");
  /*Serial.begin(9600);
  if (!Serial) {
    writeInfo("Failed initialising serial");
  }*/
}

void loop() {
  writeInfo("testing this");
  writeSensorInfo();
  delay(300);
}
