#include "SDWriter.hpp"

SDWriter::SDWriter(int _SPI_CS) : SPI_CS(_SPI_CS) {
  SD.begin(SPI_CS);
  logFile = SD.open("log.txt", FILE_WRITE);
}

SDWriter::~SDWriter() { logFile.close(); }

void SDWriter::writeData(SensorsData *data) {
  logFile.print("[DATA] ");
  logFile.println(data->getRawData());
  logFile.flush();
}

void SDWriter::writeData(const char *text) {
  logFile.print("[TEXT] ");
  logFile.print(millis());
  logFile.print(" - ");
  logFile.println(text);
  logFile.flush();
}
