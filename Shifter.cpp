#include <Arduino.h>
#include <Servo.h>
#include "PrefEditor.h"
#include "Derailleur.h"
#include "Shifter.h"
#include "ShifterSmart.h"


float Shifter::ratio[4][16];
PrefEditor*  Shifter::_pe;

Shifter::Shifter() {}
void Shifter::Setup(Derailleur * derailleur, uint8_t *g, int8_t *gp, PrefEditor *pe) {
  d = derailleur;
  //setup gear position
  gearPosition = gp;
  //setup gear count
  gear = g;
  _pe = pe;
  isSync = _pe->sync();
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
      ratio[f][r] = (float) * (gear + 2 + f) / (float) * (gear + 6 + * (gear + 1) - r - 1); // *gear == gearCount
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
void Shifter::sync(bool b) {
  isSync = b;
  if (b) {
    Serial.println("Sync:On");
  } else {
    Serial.println("Sync:Off");
  }
}


