#ifndef SENSORS_HPP
#define SENSORS_HPP
#include <Wire.h>

#define LPS331_I2C_ADDRESS 0x5D
#define LIS3MDL_I2C_ADDRESS 0x1E
#define LSM6_I2C_ADDRESS 0x6B
#define ATTINY_I2C_ADDRESS 0x08

int16_t temperature, pressure, altitude;
int16_t acc[3], mag[3], gyr[3];
uint8_t gases[5];

void writeReg(byte address, byte reg, byte value) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void initSensors() {
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
  Wire.beginTransmission(LPS331_I2C_ADDRESS);
  Wire.write(0xAB);
  Wire.endTransmission();
  Wire.requestFrom(LPS331_I2C_ADDRESS, 2);

  while (Wire.available() < 2)
    ;
  uint8_t tl = Wire.read();
  uint8_t th = Wire.read();
  float t = (float)((int16_t)(th << 8 | tl));

  return 42.5 + t / 480;
}

float readPressure() {
  Wire.beginTransmission(LPS331_I2C_ADDRESS);
  Wire.write(0xA8);
  Wire.endTransmission();
  Wire.requestFrom(LPS331_I2C_ADDRESS, 3);
  while (Wire.available() < 3)
    ;

  uint8_t pxl = Wire.read();
  uint8_t pl = Wire.read();
  uint8_t ph = Wire.read();
  float p = (float)((int32_t)(int8_t)ph << 16 | (uint16_t)pl << 8 | pxl);

  return p / 4096;
}

float pressureToAttitude(float pressure) {
  return (1 - powf(pressure / 1013.25, 0.190263)) * 44330.8;
}

void readMagnet(int16_t *m) {
  Wire.beginTransmission(LIS3MDL_I2C_ADDRESS);
  Wire.write(0xA8);
  Wire.endTransmission();
  Wire.requestFrom(LIS3MDL_I2C_ADDRESS, 6);

  while (Wire.available() < 6)
    ;

  uint8_t xlm = Wire.read();
  uint8_t xhm = Wire.read();
  uint8_t ylm = Wire.read();
  uint8_t yhm = Wire.read();
  uint8_t zlm = Wire.read();
  uint8_t zhm = Wire.read();

  m[0] = (int16_t)(xhm << 8 | xlm);
  m[1] = (int16_t)(yhm << 8 | ylm);
  m[2] = (int16_t)(zhm << 8 | zlm);
}

void readAccel(int16_t *a) {
  Wire.beginTransmission(LSM6_I2C_ADDRESS);
  Wire.write(0x28);
  Wire.endTransmission();
  Wire.requestFrom(LSM6_I2C_ADDRESS, 6);

  while (Wire.available() < 6)
    ;

  uint8_t xla = Wire.read();
  uint8_t xha = Wire.read();
  uint8_t yla = Wire.read();
  uint8_t yha = Wire.read();
  uint8_t zla = Wire.read();
  uint8_t zha = Wire.read();

  a[0] = (int16_t)(xha << 8 | xla);
  a[1] = (int16_t)(yha << 8 | yla);
  a[2] = (int16_t)(zha << 8 | zla);
}

void readGyro(int16_t *g) {
  Wire.beginTransmission(LSM6_I2C_ADDRESS);
  Wire.write(0x22);
  Wire.endTransmission();
  Wire.requestFrom(LSM6_I2C_ADDRESS, 6);

  while (Wire.available() < 6)
    ;

  uint8_t xlg = Wire.read();
  uint8_t xhg = Wire.read();
  uint8_t ylg = Wire.read();
  uint8_t yhg = Wire.read();
  uint8_t zlg = Wire.read();
  uint8_t zhg = Wire.read();

  g[0] = (int16_t)(xhg << 8 | xlg);
  g[1] = (int16_t)(yhg << 8 | ylg);
  g[2] = (int16_t)(zhg << 8 | zlg);
}

void readGases(uint8_t* g) {
  Wire.beginTransmission(ATTINY_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.requestFrom(ATTINY_I2C_ADDRESS, 5);

  while(Wire.available() < 5)
    ;

  for (int i = 0; i < 5; ++i)
    g[i] = Wire.read();
}

void readSensors() {
  // logWrite("<Reading from LPS>");
  temperature = static_cast<int16_t>(readTemperature()*100);
  pressure = static_cast<int16_t>(readPressure()*100);
  altitude = static_cast<int16_t>(pressureToAttitude(pressure)*100);

  // logWrite("<Reading from LIS3MDL>");
  readMagnet(mag);

  // logWrite("<Reading from LSM6>");
  readAccel(acc);
  readGyro(gyr);
  readGases(gases);
}

#endif
