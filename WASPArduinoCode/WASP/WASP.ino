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
  logFile.println();
  logFile.println("[LOGFILE OPENED]");
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
  logFile.print(" to reg ");
  logFile.print(reg);
  logFile.print(" to device at ");
  logFile.print(address);
  logFile.flush();
#endif

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);

#ifdef __DEBUG__
  int rc = Wire.endTransmission();
  logFile.print(" (returned ");
  logFile.print(rc);
  logFile.println(')');
  logFile.flush();
#else
  Wire.endTransmission();
#endif
}

// # Sensors management #
inline void initSensors() {
#ifdef __DEBUG__
  logFile.print(millis());
  logFile.println(" [DEBUG] initialising sensors");
  logFile.flush();
#endif
  Wire.begin();

  // init LPS331 (temperature/pressure/attitude sensor)
  writeReg(LPS331_I2C_ADDRESS, 0x20, 0b11100000);

  // init LIS3MDL (magnetometer)
  writeReg(LIS3MDL_I2C_ADDRESS, 0x20, 0x70);
  writeReg(LIS3MDL_I2C_ADDRESS, 0x21, 0x00);
  writeReg(LIS3MDL_I2C_ADDRESS, 0x22, 0x00);
  writeReg(LIS3MDL_I2C_ADDRESS, 0x23, 0x0C);

  // init LSM6 (gyro/accelerometer)
  writeReg(LSM6_I2C_ADDRESS, 0x10, 0x80);
  writeReg(LSM6_I2C_ADDRESS, 0x11, 0x80);
  writeReg(LSM6_I2C_ADDRESS, 0x12, 0x04);
}

float readTemperature() {
#ifdef __DEBUG__
  logFile.print(millis());
  logFile.println(" [DEBUG] Reading temperature");
  logFile.flush();
#endif

  Wire.beginTransmission(LPS331_I2C_ADDRESS);
  Wire.write(0x2B | (1 << 7));
#ifdef __DEBUG__
  logFile.print("Wrote to LPS331, returned ");
  int rc = Wire.endTransmission();
  logFile.println(rc);
#else
  Wire.endTransmission();
#endif
  Wire.requestFrom(LPS331_I2C_ADDRESS, 2);

  while (Wire.available() < 2)
    ;
  uint8_t tl = Wire.read();
  uint8_t th = Wire.read();
  float t = (float)((int16_t)(th << 8 | tl));

  return 42.5 + t / 480;
}

// SETUP/LOOP
void setup() {
  initLog();
  initSensors();
}

void loop() {
  int16_t temperature = readTemperature();
  logWriteReading("temperature", temperature);
  delay(1000);
}
