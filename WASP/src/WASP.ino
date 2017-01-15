#include "Autonomy.hpp"
#include "SDWriter.hpp"
#include "Sensor.hpp"
#include <Arduino.h>

/* WASP Project
 * by Phoenix_PL AKA Wojciech Olech
 * Read README.md for more information
 */
SensorReader reader;
SDWriter writer(A1);
Autonomy autonomy(5, 6);
SensorsData *data;

void setup() {
  pinMode(13, OUTPUT);
  // autonomy.checkServos();
  writer.writeData("dis gon b gud");
}

void loop() {
  data = reader.readData();
  writer.writeData(data);
  digitalWrite(13, 1);
  delay(200);
  digitalWrite(13, 0);
  delay(300);
}
