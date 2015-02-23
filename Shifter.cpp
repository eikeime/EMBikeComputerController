#include <Arduino.h>
#include <Servo.h>
#include "PrefEditor.h"
#include "Derailleur.h"
#include "Shifter.h"
#include "ShifterSmart.h"


 float Shifter::ratio[4][16];

Shifter::Shifter(){}
void Shifter::Setup(Derailleur * derailleur, uint8_t *g, int8_t *gp){
  d = derailleur;
  //setup gear position
  gearPosition = gp;
  //setup gear count
  gear = g;
  
}



void Shifter::Refresh(){
//setup gear ratio
  uint8_t f = 0;
  do
  {
    uint8_t r = 0;
    do
    {
      ratio[f][r] = (float) * (gear + 2 + f) / (float) * (gear + 6 + r);// *gear == gearCount
      r++;
    }
    while (r < *(gear + 1));
    f++;
  }
  while (f < *gear);
    Serial.println("Refresh");

}

