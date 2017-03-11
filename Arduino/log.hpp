#ifndef __LOG_HPP__
#define __LOG_HPP__
#include <SD.h>
#include <SPI.h>

#define SD_CS_PIN A1

File logFile;
String tmp;

inline void initLog() {
  SD.begin(SD_CS_PIN);
  char filename[16];
  for (unsigned int i = 1;; ++i) {
    sprintf(filename, "LOG%d.txt", i);
    if (!SD.exists(filename)) break;
  }
  logFile = SD.open(filename, FILE_WRITE);
}

inline void logWrite(String message) {
  tmp = String(millis() + message);
  logFile.write(tmp.c_str());
  //   logFile.print(millis());
  //   logFile.print(' ');
  //   logFile.println(message);
  logFile.flush();
}

/*template <typename T>
inline void logWriteReading(String name, T value) {
  logFile.print("[R]");
  logFile.print(name);
  logFile.print(':');
  logFile.println(value);
  logFile.flush();
}*/

#endif