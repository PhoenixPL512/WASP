/**
* @Author: Wojciech Olech
* @Date:   19-01-2017
* @Email:  phoenixpl512@gmail.com
* @Filename: WASP.ino
* @Last modified by:   Wojciech Olech
* @Last modified time: 19-01-2017
* @License: GNU GPL
* @Copyright: do whatever you want
*/

// INCLUDES
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

// DEFINES
#define SD_CS_PIN A1

// GLOBAL VARIABLES
File logFile;

// FUNCTIONS
// # SD Card management #
inline void logInit() {
  SD.begin(SD_CS_PIN);
  logFile = SD.open("LOG.TXT", FILE_WRITE);
  logFile.println("\n\n[LOGFILE OPENED]");
  logFile.flush();
}

inline void logWriteMessage(String message) {
  logFile.print(millis());
  logFile.print(' ');
  logFile.println(message);
  logFile.flush();
}

template <typename T> inline void logWriteReading(String name, T value) {
  logFile.print(millis());
  logFile.print(" [R]");
  logFile.print(name);
  logFile.print(':');
  logFile.println(value);
  logFile.flush();
}

// SETUP/LOOP
void setup() { logInit(); }

void loop() {}
