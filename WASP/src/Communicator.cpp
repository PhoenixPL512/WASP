#include "Communicator.hpp"

Communicator::Communicator(int _SPI_CE, int _SPI_CS)
    : SPI_CE(_SPI_CE), SPI_CS(_SPI_CS) {
  radio = new RF24(SPI_CE, SPI_CS);
  radio->begin();
  radio->setPALevel(RF24_PA_HIGH);
  radio->setDataRate(RF24_250KBPS);
  radio->setCRCLength(RF24_CRC_16);
  radio->openWritingPipe(pipe);
  radio->powerDown();
}

Communicator::~Communicator() { delete radio; }
