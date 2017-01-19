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
#include <Wire.h>

// DEFINES
/* I2C scanner found 3 devices: 0x1E, 0x5D and 0x6B */
#define __DEBUG__ 1
#define SD_CS_PIN A1
#define LPS331_I2C_ADDRESS 0x5D
#define LIS3MDL_I2C_ADDRESS 0x1E
#define LSM6_I2C_ADDRESS 0x6B

// GLOBAL VARIABLES
File logFile;

// FUNCTIONS
// # SD Card management #
inline void initLog() {
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

// # Misc I2C functions used by sensors #
inline void writeReg(byte address, byte reg, byte value) {
#ifdef __DEBUG__
  logFile.print(millis());
  logFile.print(" [DEBUG] Writing value ");
  logFile.print(value);
  logFile.print("to reg ");
  logFile.print(reg);
  logFile.print(" to device at ");
  logFile.println(address);
  logFile.flush();
#endif

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// # Sensors management #
inline void initSensors() {
// init LPS331 (temperature/pressure/attitude sensor)
#ifdef __DEBUG__
  logFile.print(millis());
  logFile.println(" [DEBUG] initialising sensors");
  logFile.flush();
#endif
  Wire.begin(LPS331_I2C_ADDRESS);
  writeReg(LPS331_I2C_ADDRESS, 0x20, 0b11100000);
}

// SETUP/LOOP
void setup() {
  initLog();
  initSensors();
}

void loop() {
  logWriteMessage("test");
  delay(1000);
}
