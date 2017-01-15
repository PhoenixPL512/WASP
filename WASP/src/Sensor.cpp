#include "Sensor.hpp"

String SensorsData::getRawData() {
  /* sprintf(buffer,
     "%lu&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%s",
          timestamp, CO, LPG, hydrogen, ozone, methane, pressure, humidity,
          temperature, accel[0], accel[1], accel[2], gyro[0], gyro[1], gyro[2],
          magnet[0], magnet[1], magnet[2], gps); /old */

  String data = "";
  data += timestamp + '&' + CO + '&' + LPG + '&' + hydrogen + '&' + ozone +
          '&' + methane + '&' + humidity + '&' + temperature + '&' + pressure +
          '&' + altitude + '&' + accel[0] + '&' + accel[1] + '&' + accel[2] +
          '&' + gyro[0] + '&' + gyro[1] + '&' + gyro[2] + '&' + magnet[0] +
          '&' + magnet[1] + '&' + magnet[2] + '&' + gps;
  return data;
}

SensorReader::SensorReader() {
  Wire.begin();
  Serial.begin(9600);
  ps.init();
  ps.enableDefault();
  imu.init();
  imu.enableDefault();
  mag.init();
  mag.enableDefault();
}

SensorReader::~SensorReader() {}

SensorsData *SensorReader::readData() {
  SensorsData *data = new SensorsData;
  data->timestamp = millis();

  // TODO: insert analog sensors reading here
  data->CO = 0;
  data->LPG = 0;
  data->hydrogen = 0;
  data->ozone = 0;
  data->methane = 0;
  data->humidity = 0;

  // LPS331
  data->temperature = ps.readTemperatureC();
  data->pressure = ps.readPressureMillibars();
  data->altitude = ps.pressureToAltitudeMeters(data->pressure);

  // LSM6
  imu.read();
  data->accel[0] = imu.a.x;
  data->accel[1] = imu.a.y;
  data->accel[2] = imu.a.z;
  data->gyro[0] = imu.g.x;
  data->gyro[1] = imu.g.y;
  data->gyro[2] = imu.g.z;

  // LIS3MDL
  mag.read();
  data->magnet[0] = mag.m.x;
  data->magnet[1] = mag.m.y;
  data->magnet[2] = mag.m.z;

  // NEO-7M-C
  data->gps = Serial.readString();

  return data;
}
