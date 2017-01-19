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
HardwareSerial gpsSerial(Serial);
boolean gpsStatus[] = {false, false, false, false, false, false, false};
unsigned long start;

// SENSORS STRUCTURE
struct SensorsData {
  long timestamp;
  float temperature;
  float pressure;
  float attitude;
  String GPS;
};
SensorsData sensorsData;

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
  logFile.print("[R]");
  logFile.print(name);
  logFile.print(':');
  logFile.println(value);
  logFile.flush();
}

inline void logWriteSensorsData(SensorsData &data) {
  logFile.println("[SENSORSDATA]");
  logFile.print("[T]");
  logFile.println(data.timestamp);
  logFile.flush();

  logWriteReading("Temperature", data.temperature);
  logWriteReading("Pressure", data.pressure);
  logWriteReading("Attitude", data.attitude);
  logWriteReading("GPS", data.GPS);

  logFile.println("[SENSORSDATA_END]");
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
  gpsSerial.begin(9600);

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
  Wire.write(0xAB);
#ifdef __DEBUG__
  logFile.print("Wrote at 0xAB to LPS331, returned ");
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

float readPressure() {
#ifdef __DEBUG__
  logFile.print(millis());
  logFile.println(" [DEBUG] Reading pressure");
  logFile.flush();
#endif

  Wire.beginTransmission(LPS331_I2C_ADDRESS);
  Wire.write(0xA8);
#ifdef __DEBUG__
  logFile.print("Wrote at 0xA8 to LPS331, returned ");
  int rc = Wire.endTransmission();
  logFile.println(rc);
#else
  Wire.endTransmission();
#endif

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

String readGPS() {
#ifdef __DEBUG__
  logFile.print(millis());
  if (!Serial) {
    logFile.println(" [DEBUG] Serial not initialised");
    return "{NODATA}";
  } else if (!Serial.available()) {
    logFile.println(" [DEBUG] Nothing is available on serial port");
    return "{NODATA}";
  } else {
    logFile.println(" [DEBUG] Reading string from serial...");
    return Serial.readString();
  }
#else
  if (!gpsSerial || !gpsSerial.available())
    return "{NODATA}";
  else
    return gpsSerial.readString();
#endif
}

void calcChecksum(byte *checksumPayload, byte payloadSize) {
  byte CK_A = 0, CK_B = 0;
  for (int i = 0; i < payloadSize; i++) {
    CK_A = CK_A + *checksumPayload;
    CK_B = CK_B + CK_A;
    checksumPayload++;
  }
  *checksumPayload = CK_A;
  checksumPayload++;
  *checksumPayload = CK_B;
}

void sendUBX(byte *UBXmsg, byte msgLength) {
  for (int i = 0; i < msgLength; i++) {
    gpsSerial.write(UBXmsg[i]);
    gpsSerial.flush();
  }
  gpsSerial.println();
  gpsSerial.flush();
}

byte getUBX_ACK(byte *msgID) {
  byte CK_A = 0, CK_B = 0;
  byte incoming_char;
  boolean headerReceived = false;
  unsigned long ackWait = millis();
  byte ackPacket[10] = {0xB5, 0x62, 0x05, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00};
  int i = 0;
  while (1) {
    if (gpsSerial.available()) {
      incoming_char = gpsSerial.read();
      if (incoming_char == ackPacket[i]) {
        i++;
      } else if (i > 2) {
        ackPacket[i] = incoming_char;
        i++;
      }
    }
    if (i > 9)
      break;
    if ((millis() - ackWait) > 1500) {
      Serial.println("ACK Timeout");
      return 5;
    }
    if (i == 4 && ackPacket[3] == 0x00) {
      Serial.println("NAK Received");
      return 1;
    }
  }

  for (i = 2; i < 8; i++) {
    CK_A = CK_A + ackPacket[i];
    CK_B = CK_B + CK_A;
  }
  if (msgID[0] == ackPacket[6] && msgID[1] == ackPacket[7] &&
      CK_A == ackPacket[8] && CK_B == ackPacket[9]) {
    Serial.println("Success!");
    Serial.print("ACK Received! ");
    printHex(ackPacket, sizeof(ackPacket));
    return 10;
  } else {
    Serial.print("ACK Checksum Failure: ");
    printHex(ackPacket, sizeof(ackPacket));
    delay(1000);
    return 1;
  }
}

void printHex(uint8_t *data, uint8_t length) // prints 8-bit data in hex
{
  char tmp[length * 2 + 1];
  byte first;
  int j = 0;
  for (byte i = 0; i < length; i++) {
    first = (data[i] >> 4) | 48;
    if (first > 57)
      tmp[j] = first + (byte)7;
    else
      tmp[j] = first;
    j++;

    first = (data[i] & 0x0F) | 48;
    if (first > 57)
      tmp[j] = first + (byte)7;
    else
      tmp[j] = first;
    j++;
  }
  tmp[length * 2] = 0;
  for (byte i = 0, j = 0; i < sizeof(tmp); i++) {
    Serial.print(tmp[i]);
    if (j == 1) {
      Serial.print(" ");
      j = 0;
    } else
      j++;
  }
  Serial.println();
}

void setBaud(byte baudSetting) {
  if (baudSetting == 0x12)
    gpsSerial.begin(4800);
  if (baudSetting == 0x4B)
    gpsSerial.begin(19200);
  if (baudSetting == 0x96)
    gpsSerial.begin(38400);
  if (baudSetting == 0xE1)
    gpsSerial.begin(57600);
  if (baudSetting == 0xC2)
    gpsSerial.begin(115200);
  if (baudSetting == 0x84)
    gpsSerial.begin(230400);
}

void configureUblox(byte *settingsArrayPointer) {
  byte gpsSetSuccess = 0;
  byte setNav[] = {
      0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, *settingsArrayPointer,
      0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00,
      0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C,
      0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  calcChecksum(&setNav[2], sizeof(setNav) - 4);
  byte setDataRate[] = {0xB5,
                        0x62,
                        0x06,
                        0x08,
                        0x06,
                        0x00,
                        settingsArrayPointer[1],
                        settingsArrayPointer[2],
                        0x01,
                        0x00,
                        0x01,
                        0x00,
                        0x00,
                        0x00};
  calcChecksum(&setDataRate[2], sizeof(setDataRate) - 4);
  byte setPortRate[] = {0xB5,
                        0x62,
                        0x06,
                        0x00,
                        0x14,
                        0x00,
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0xD0,
                        0x08,
                        0x00,
                        0x00,
                        settingsArrayPointer[3],
                        settingsArrayPointer[4],
                        settingsArrayPointer[5],
                        0x00,
                        0x07,
                        0x00,
                        0x03,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00};
  calcChecksum(&setPortRate[2], sizeof(setPortRate) - 4);
  byte setGLL[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B};
  byte setGSA[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32};
  byte setGSV[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39};
  byte setRMC[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x40};
  byte setVTG[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46};

  delay(2500);

  while (gpsSetSuccess < 3) {
    logFile.print("Setting Navigation Mode... ");
    sendUBX(&setNav[0], sizeof(setNav));     // Send UBX Packet
    gpsSetSuccess += getUBX_ACK(&setNav[2]); // Passes Class ID and Message ID
                                             // to the ACK Receive function
    if (gpsSetSuccess == 5) {
      gpsSetSuccess -= 4;
      setBaud(settingsArrayPointer[4]);
      delay(1500);
      byte lowerPortRate[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01,
                              0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00,
                              0x80, 0x25, 0x00, 0x00, 0x07, 0x00, 0x03,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0xA2, 0xB5};
      sendUBX(lowerPortRate, sizeof(lowerPortRate));
      gpsSerial.begin(9600);
      delay(2000);
    }
    if (gpsSetSuccess == 6)
      gpsSetSuccess -= 4;
    if (gpsSetSuccess == 10)
      gpsStatus[0] = true;
  }
  if (gpsSetSuccess == 3)
    Serial.println("Navigation mode configuration failed.");
  gpsSetSuccess = 0;
  while (gpsSetSuccess < 3) {
    Serial.print("Setting Data Update Rate... ");
    sendUBX(&setDataRate[0], sizeof(setDataRate)); // Send UBX Packet
    gpsSetSuccess += getUBX_ACK(&setDataRate[2]); // Passes Class ID and Message
                                                  // ID to the ACK Receive
                                                  // function
    if (gpsSetSuccess == 10)
      gpsStatus[1] = true;
    if (gpsSetSuccess == 5 | gpsSetSuccess == 6)
      gpsSetSuccess -= 4;
  }
  if (gpsSetSuccess == 3)
    Serial.println("Data update mode configuration failed.");
  gpsSetSuccess = 0;

  while (gpsSetSuccess < 3 && settingsArrayPointer[6] == 0x00) {
    Serial.print("Deactivating NMEA GLL Messages ");
    sendUBX(setGLL, sizeof(setGLL));
    gpsSetSuccess += getUBX_ACK(&setGLL[2]);
    if (gpsSetSuccess == 10)
      gpsStatus[2] = true;
    if (gpsSetSuccess == 5 | gpsSetSuccess == 6)
      gpsSetSuccess -= 4;
  }
  if (gpsSetSuccess == 3)
    Serial.println("NMEA GLL Message Deactivation Failed!");
  gpsSetSuccess = 0;

  while (gpsSetSuccess < 3 && settingsArrayPointer[7] == 0x00) {
    Serial.print("Deactivating NMEA GSA Messages ");
    sendUBX(setGSA, sizeof(setGSA));
    gpsSetSuccess += getUBX_ACK(&setGSA[2]);
    if (gpsSetSuccess == 10)
      gpsStatus[3] = true;
    if (gpsSetSuccess == 5 | gpsSetSuccess == 6)
      gpsSetSuccess -= 4;
  }
  if (gpsSetSuccess == 3)
    Serial.println("NMEA GSA Message Deactivation Failed!");
  gpsSetSuccess = 0;

  while (gpsSetSuccess < 3 && settingsArrayPointer[8] == 0x00) {
    Serial.print("Deactivating NMEA GSV Messages ");
    sendUBX(setGSV, sizeof(setGSV));
    gpsSetSuccess += getUBX_ACK(&setGSV[2]);
    if (gpsSetSuccess == 10)
      gpsStatus[4] = true;
    if (gpsSetSuccess == 5 | gpsSetSuccess == 6)
      gpsSetSuccess -= 4;
  }
  if (gpsSetSuccess == 3)
    Serial.println("NMEA GSV Message Deactivation Failed!");
  gpsSetSuccess = 0;

  while (gpsSetSuccess < 3 && settingsArrayPointer[9] == 0x00) {
    Serial.print("Deactivating NMEA RMC Messages ");
    sendUBX(setRMC, sizeof(setRMC));
    gpsSetSuccess += getUBX_ACK(&setRMC[2]);
    if (gpsSetSuccess == 10)
      gpsStatus[5] = true;
    if (gpsSetSuccess == 5 | gpsSetSuccess == 6)
      gpsSetSuccess -= 4;
  }
  if (gpsSetSuccess == 3)
    Serial.println("NMEA RMC Message Deactivation Failed!");
  gpsSetSuccess = 0;

  while (gpsSetSuccess < 3 && settingsArrayPointer[10] == 0x00) {
    Serial.print("Deactivating NMEA VTG Messages ");
    sendUBX(setVTG, sizeof(setVTG));
    gpsSetSuccess += getUBX_ACK(&setVTG[2]);
    if (gpsSetSuccess == 10)
      gpsStatus[6] = true;
    if (gpsSetSuccess == 5 | gpsSetSuccess == 6)
      gpsSetSuccess -= 4;
  }
  if (gpsSetSuccess == 3)
    Serial.println("NMEA VTG Message Deactivation Failed!");

  gpsSetSuccess = 0;
  if (settingsArrayPointer[4] != 0x25) {
    Serial.print("Setting Port Baud Rate... ");
    sendUBX(&setPortRate[0], sizeof(setPortRate));
    setBaud(settingsArrayPointer[4]);
    Serial.println("Success!");
    delay(500);
  }
}

inline void readSensorsData(SensorsData &data) {
  data.timestamp = millis();
  data.temperature = readTemperature();
  data.pressure = readPressure();
  data.attitude = pressureToAttitude(data.pressure);
  data.GPS = readGPS();
}

// SETUP/LOOP
void setup() {
  initLog();
  initSensors();
  byte settingsArray[] = {0x03, 0xFA, 0x00, 0x00, 0xE1, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00};
  configureUblox(settingsArray);
}

void loop() {
  readSensorsData(sensorsData);
  logWriteSensorsData(sensorsData);
}
