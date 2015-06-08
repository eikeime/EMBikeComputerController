#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>
#include <Wire.h>
#include "config.h"
#include "PrefEditor.h"
#include "Derailleur.h"



Derailleur::Derailleur()
{

  //setup parameter
  servoStep[0] = -10;
  servoStep[1] = 5;

}
Derailleur::Derailleur(PrefEditor * pref_editor) // init  data
{
  pe = pref_editor;
  setPref(pref_editor);
  //setup parameter

  servoStep[0] = -10;
  servoStep[1] = 5;

}

void Derailleur::setPref(PrefEditor * pref_editor) {
  pe = pref_editor;
  gear = pref_editor->getGear();
  //setup gear position
  gearPosition = pref_editor->getGearPosition();
  //setup servopositionlist
  servoPositionList = pref_editor->getServoPositionList();
  trim_switch = pe->trim();

}


void Derailleur::ServoOn()
{
  Serial.println("===servo ON===");
  servo[0].attach(DERAILLEUR_SERVO_FRONT);
  if (servo[0].attached() && *gear > 0)
  {
    if (isTrim()) {
      servo[0].writeMicroseconds((int16_t)(*(servoPositionList + *gearPosition) + (( * (servoPositionList + *gearPosition + 1) -  * (servoPositionList + *gearPosition)) * ((float)(*(gearPosition + 1) + 1) / * (gear + 1)))));

    } else {
      servo[0].writeMicroseconds(*(servoPositionList + *gearPosition) );
    }
    delay(500);
  }
  else
  {
    Serial.println("error servo:0 is off");
  }


  servo[1].attach(DERAILLEUR_SERVO_REAR);

  if (servo[1].attached() && *(gear + 1) > 0)
  {

    servo[1].writeMicroseconds(*(servoPositionList + 8 + * (gearPosition + 1)));


  }
  else
  {
    Serial.println("error servo:1 is off");
  }

  Serial.print("front:");
  Serial.print(*gearPosition);
  Serial.print(" > ")  ;
  Serial.println(servo[0].readMicroseconds());
  Serial.print("rear:");
  Serial.print(*(gearPosition + 1));
  Serial.print(" > ");
  Serial.println(servo[1].readMicroseconds());
  Serial.println("----------------");
}
void Derailleur::ServoOff()
{
  servo[0].detach();
  servo[1].detach();
}
void Derailleur::setupServo(int id, uint8_t sStep)
{
  servoStep[id] = sStep;
}
void Derailleur::tune(int id, int direction)
{

  if (servo[id].attached())
  {
    Serial.print("tune: ");
    Serial.print(servo[id].readMicroseconds());
    servo[id].writeMicroseconds(servo[id].readMicroseconds() + servoStep[id]*direction);

    Serial.print(" to ");
    Serial.print(servo[id].readMicroseconds());
    Serial.print(" save ");
    if (id == 0) {

      *(servoPositionList +  * (gearPosition + id)) = servo[id].readMicroseconds();

      Serial.println( *(servoPositionList +  * (gearPosition + id)));
    } else if (id == 1) {
      *(servoPositionList + * (gearPosition + id) + 8) = servo[id].readMicroseconds();
      Serial.println( *(servoPositionList + * (gearPosition + id) + 8));


    }



  }
  else
  {
    Serial.print("error servo:");
    Serial.print(id);
    Serial.println(" is off");
  }
}
void Derailleur::trim(bool b) {
  trim_switch = b;

  if (b) {
    trim();
    Serial.println("Trim:On");
  } else {
    Serial.println("Trim:Off");
    if (servo[0].attached())
    {
      servo[0].writeMicroseconds(*(servoPositionList +  *gearPosition));
    }
  }





}
bool Derailleur::isTrim() {
  return trim_switch;
}


void Derailleur::trim()
{

  if (needTrim && trim_switch)
  {
    if (millis() - DERAILLEUR_TRIM_DELAY > shiftTimeStamp) {
      if (servo[0].attached())
      {

        servo[0].writeMicroseconds((int16_t)(*(servoPositionList + *gearPosition) + (( * (servoPositionList + *gearPosition + 1) -  * (servoPositionList + *gearPosition)) * ((float)(*(gearPosition + 1) + 1) / * (gear + 1)))));
      }
      else
      {
        Serial.println("error servo:0 is off");
      }

      if (servo[1].attached())
      {
        servo[1].writeMicroseconds(*(servoPositionList + 8 + * (gearPosition + 1)));

      }
      else
      {
        Serial.println("error servo:1 is off");
      }
      Serial.println("----------------");
      Serial.println("trim:");
      Serial.println(servo[0].readMicroseconds());
      Serial.println(servo[1].readMicroseconds());
      Serial.println("----------------");
      needTrim = false;
    }
  }

}

void Derailleur::shiftTo(uint8_t front, uint8_t rear)
{
  if (servo[0].attached())
  {
    if (trim_switch) {
      if (*gearPosition % 2 == 1) {
        *gearPosition -= 1;

      }
      if (front > *gearPosition)
      {
        if (*gearPosition + 2 < *gear) {
          servo[0].writeMicroseconds(*(servoPositionList + *gearPosition + 2));
          *gearPosition += 2;
          shiftTimeStamp = millis();
          needTrim = true;
        }
      }
      else if (front < *gearPosition)
      {
        servo[0].writeMicroseconds(*(servoPositionList + *gearPosition - 1 ));
        *gearPosition -= 2;
        shiftTimeStamp = millis();
        needTrim = true;

      }


    } else {
      servo[0].writeMicroseconds(*(servoPositionList + front ));
      *gearPosition = front;



    }
  }
  else
  {
    Serial.println("error servo:0 is off");
  }
  if (servo[1].attached())
  {

    if (rear > *(gearPosition + 1))
    {
      servo[1].writeMicroseconds(*(servoPositionList + 8 + rear) + (int16_t)(pe->gap*DERAILLEUR_TRIM_RATE));
      *(gearPosition + 1) = rear;
      shiftTimeStamp = millis();
      needTrim = true;

    }
    else if (rear < * (gearPosition + 1))
    {
      servo[1].writeMicroseconds(*(servoPositionList + 8 + rear) - (int16_t)(pe->gap*DERAILLEUR_TRIM_RATE));
      *(gearPosition + 1) = rear;
      shiftTimeStamp = millis();
      needTrim = true;

    }

  }
  else
  {
    Serial.println("error servo:1 is off");
  }
  Serial.println("----------------");
  Serial.print("front:");
  Serial.print(*gearPosition);
  Serial.print(" > ")  ;
  Serial.println(servo[0].readMicroseconds());
  Serial.print("rear:");
  Serial.print(*(gearPosition + 1));
  Serial.print(" > ");
  Serial.println(servo[1].readMicroseconds());
  tone(SPEAKER_PIN, SPEAKER_FREQ, 50);
  sendData(9);
  Serial.println("----------------");
  pe->saveGearPosition();
}


int Derailleur::getServoMicroSeconds(int i) {
  return servo[i].readMicroseconds();

}

void Derailleur::sendData(byte data) {
  Wire.beginTransmission(I2C_ADDR_SHIFTER); // transmit to device #9
  Wire.write(data); // sends x
  Wire.endTransmission(); // stop transmitting
}







