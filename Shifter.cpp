#include <Arduino.h>
#include <Servo.h>
#include "PrefEditor.h"
#include "Derailleur.h"
#include "Shifter.h"
#include "ShifterSmart.h"


float Shifter::ratio[4][16];

Shifter::Shifter() {}
void Shifter::Setup(Derailleur * derailleur, uint8_t *g, int8_t *gp) {
  d = derailleur;
  //setup gear position
  gearPosition = gp;
  //setup gear count
  gear = g;
  Refresh();
}



void Shifter::Refresh() {
  //setup gear ratio
  uint8_t f = 0;
  do
  {
    uint8_t r = 0;
    do
    {
      ratio[f][r] = (float) * (gear + 2 + f) / (float) * (gear + 6 + *(gear+1)-r-1);// *gear == gearCount
      r++;
    }
    while (r < * (gear + 1));
    f++;
  }
  while (f < *gear / 2);
  Serial.println("Refresh");

}

void Shifter::print() {
  uint8_t f = 0;
  do
  {
    Serial.print(f);
    Serial.print(" -- ");
    uint8_t r = 0;
    do
    {
      Serial.print(r);
      Serial.print(":");
      Serial.print(ratio[f][r]);
      Serial.print(" ");
      r++;
    }
    while (r < * (gear + 1));
    Serial.println();
    f++;
  }
  while (f < *gear / 2);
}
void Shifter::setSync(bool b) {
  isSync == b;
}

