#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>

#include "config.h"
#include "PrefEditor.h"


PrefEditor::PrefEditor()   // init  data
{
  Serial.println("peconstruct");
  if (init()) {
    Serial.println("initing");
    initGearPref();
  } else {
    Serial.println("reading");
    read();
  }



}


union
{
  struct
  {

    uint8_t gear[22];//frontcount,rearcount,2-5 front 1-4 teethcount,6-21 rear 1-16 teethcount
    int16_t servoPositionList[24];//front servo position 0-7 two for each,rear servo position 8-23 one for each;

  } gp;
  uint8_t storage[sizeof(gp)];
} gearPref;


void PrefEditor::initGearPref()
{

  gearPref.gp.gear[0] = 0;
  gearPref.gp.gear[1] = 0;
  gearPref.gp.gear[2] = 34;
  gearPref.gp.gear[3] = 50;

  gearPref.gp.gear[6] = 12;
  gearPref.gp.gear[7] = 13;
  gearPref.gp.gear[8] = 14;
  gearPref.gp.gear[9] = 15;
  gearPref.gp.gear[10] = 16;
  gearPref.gp.gear[11] = 17;
  gearPref.gp.gear[12] = 18;
  gearPref.gp.gear[13] = 19;
  gearPref.gp.gear[14] = 21;
  gearPref.gp.gear[15] = 23;
  gearPref.gp.gear[16] = 25;

  gearPref.gp.gear[4] = 1;
  gearPref.gp.gear[5] = 1;
  gearPref.gp.gear[17] = 1;
  gearPref.gp.gear[18] = 1;
  gearPref.gp.gear[19] = 1;
  gearPref.gp.gear[20] = 1;
  gearPref.gp.gear[21] = 1;

  int i = 0;
  while (i < 24) {

    gearPref.gp.servoPositionList[i] = 1500;
    i++;
  }
  gearPosition[0] = 0;
  gearPosition[1] = 0;
  saveGearPosition();
  save();


}


uint8_t *  PrefEditor::getGear()
{
  return gearPref.gp.gear;
}

int16_t * PrefEditor::getServoPositionList()
{
  return gearPref.gp.servoPositionList;
}

int8_t * PrefEditor::getGearPosition()
{
  return gearPosition;
}

bool PrefEditor::init() {
  return EEPROM.read(EEPROM_ADDR_INIT);

}

void PrefEditor::init(bool b) {
  EEPROM.write(EEPROM_ADDR_INIT, b);
}
void PrefEditor::save() {
  int size = sizeof(gearPref.storage);
  int i = 0;
  while (i < size) {
    EEPROM.write(EEPROM_ADDR_GEARPREF_START + i, gearPref.storage[i]);
    i++;
  }
  Serial.println("SAVE");
}
void PrefEditor::read() {
  int size = sizeof(gearPref.storage);
  int i = 0;
  while (i < size) {
    gearPref.storage[i] = EEPROM.read(EEPROM_ADDR_GEARPREF_START + i);
    i++;
  }
  gearPosition[0] = EEPROM.read(EEPROM_ADDR_GEARPOSITION_FRONT);
  gearPosition[1] = EEPROM.read(EEPROM_ADDR_GEARPOSITION_REAR);
}

void PrefEditor::print() {
  int i = 0;
  while (i < 22) {
    Serial.print(i);
    Serial.print(":");
    Serial.print(gearPref.gp.gear[i]);
    Serial.print(" ");

    i++;
  }
  Serial.println();
  i = 0;
  while (i < 24) {
    Serial.print(i);
    Serial.print(":");
    Serial.print(gearPref.gp.servoPositionList[i]);
    Serial.print(" ");

    i++;
  }
  Serial.println();

}
void PrefEditor::readGearPosition() {
  gearPosition[0] = EEPROM.read(EEPROM_ADDR_GEARPOSITION_FRONT);
  gearPosition[1] = EEPROM.read(EEPROM_ADDR_GEARPOSITION_REAR);
}
void PrefEditor::saveGearPosition() {
  Serial.println("save triggered");
  if (gearPosition[0] != EEPROM.read(EEPROM_ADDR_GEARPOSITION_FRONT)) {
    EEPROM.write(EEPROM_ADDR_GEARPOSITION_FRONT, gearPosition[0]);
    Serial.println("front saved");
  }
  if (gearPosition[1] != EEPROM.read(EEPROM_ADDR_GEARPOSITION_REAR)) {
    EEPROM.write(EEPROM_ADDR_GEARPOSITION_REAR, gearPosition[1]);
    Serial.println("rear saved");
  }

}
void PrefEditor::tuneAll() {

  int16_t gap =  (int16_t)((float)( gearPref.gp.servoPositionList[gearPref.gp.gear[1] + 6] - gearPref.gp.servoPositionList[9] ) / (gearPref.gp.gear[1] - 3));

  int i = 2;
  while (i < (gearPref.gp.gear[1] - 2)) {

    gearPref.gp.servoPositionList[i + 8] = gearPref.gp.servoPositionList[9] + gap * (i - 1);

    i++;
  }

  gearPref.gp.servoPositionList[8] = gearPref.gp.servoPositionList[9] - gap * 1.20;

  gearPref.gp.servoPositionList[gearPref.gp.gear[1] + 7] = gearPref.gp.servoPositionList[gearPref.gp.gear[1] + 6] + gap * 1.20;

}

