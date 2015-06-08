////#include <Mirf.h>
////#include <MirfHardwareSpiDriver.h>
////#include <MirfSpiDriver.h>
////#include <nRF24L01.h>
//
//
//
//#include "config.h"
//#include "Transmitter.h"
//
//
//Transmitter::Transmitter(uint8_t cePin, uint8_t csnPin,bool master=true)
//{
//  Mirf.cePin = cePin;
//  Mirf.csnPin = csnPin;
//  Mirf.spi = &MirfHardwareSpi;
//  Mirf.init();
//}
//
//
//
//void Transmitter::tunnelPairMaster()
//{
//  Mirf.payload = sizeof(byte);
//  Mirf.channel = 0;
//  Mirf.setTADDR((byte *)1);
//  Mirf.setRADDR((byte *)2);
//  Mirf.config();
//}
//void Transmitter::tunnelPairSlave()
//{
//  Mirf.payload = sizeof(byte);
//  Mirf.channel = 0;
//  Mirf.setTADDR((byte *)2);
//  Mirf.setRADDR((byte *)1);
//  Mirf.config();
//}
//
//void Transmitter::tunnelSaved(){
//
//
//}
//
//

