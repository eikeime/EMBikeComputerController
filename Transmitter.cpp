#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <nRF24L01.h>



#include "config.h"
#include "Transmitter.h"


Transmitter::Transmitter(uint8_t cePin, uint8_t csnPin)
{
  Mirf.cePin = cePin;
  Mirf.csnPin = csnPin;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
}
void Transmitter::resetAddr()
{


}


void Transmitter::modeScan()
{
  Mirf.payload = sizeof(uint8_t);
  Mirf.channel = 0;
  Mirf.setTADDR((uint8_t *)1);
  Mirf.setRADDR((uint8_t *)0);
  Mirf.config();
}
void Transmitter::modeTarget()
{
  Mirf.payload = sizeof(uint8_t);
  Mirf.channel = 0;
  Mirf.setTADDR((uint8_t *)0);
  Mirf.setRADDR((uint8_t *)1);
  Mirf.config();
}




