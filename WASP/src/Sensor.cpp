#include "Sensor.hpp"

void SensorsData::getRawData(char *buffer) {
  sprintf(buffer, "%lu&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%s",
          timestamp, CO, LPG, hydrogen, ozone, methane, pressure, humidity,
          temperature, accel[0], accel[1], accel[2], gyro[0], gyro[1], gyro[2],
          magnet[0], magnet[1], magnet[2], gps);
}

SensorReader::SensorReader() { Wire.begin(); }

SensorReader::~SensorReader() {}

SensorsData *SensorReader::readData() {
  SensorsData *data = new SensorsData;
  data->timestamp = millis();

  return data;
}
