

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Servo.h>
//#include <Mirf.h>
//#include <nRF24L01.h>
//#include <MirfHardwareSpiDriver.h>
#include <Button.h>
#include <MicroLCD.h>


#include "config.h"
#include "PrefEditor.h"
#include "Derailleur.h"
#include "Shifter.h"
//#include "ShifterSmart.h"
#include "ShifterManual.h"




PrefEditor pe = PrefEditor();
Derailleur derailleur = Derailleur(&pe);

Shifter *shifter;
ShifterManual shifterManual = ShifterManual();
LCD_SSD1306 lcd;
bool lcd_exist = false;
bool shifter_exist = false;
Button bLeftUp = Button(BUTTON_LEFT_UP, BUTTON_NORMAL_LATENCY, true);
Button bLeftDown = Button(BUTTON_LEFT_DOWN, BUTTON_NORMAL_LATENCY, true);
Button bRightUp = Button(BUTTON_RIGHT_UP, BUTTON_NORMAL_LATENCY, true);
Button bRightDown = Button(BUTTON_RIGHT_DOWN, BUTTON_NORMAL_LATENCY, true);
Button bSet = Button(BUTTON_SET, BUTTON_NORMAL_LATENCY, true);


uint8_t mode = 0;//working mode init,normal,setup front,setup rear

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.begin();

  Serial.begin(9600);


  if (pe.init()) {
    mode = 4;
  }

  pe.print();



  shifterManual.Setup(&derailleur, pe.getGear(), pe.getGearPosition(), &pe);

  shifter = &shifterManual;

//  Wire.beginTransmission(0x3C);
//  if (Wire.endTransmission() == 0) {
//    Serial.println("LCD OK");
//    lcd_exist = true;
//    lcd.begin();
//    lcd.clear();
//  }
//  Wire.endTransmission(0x3C);
  
  Wire.beginTransmission(I2C_ADDR_SHIFTER);
  if (Wire.endTransmission() == 0) {
    Serial.println("Shifter OK");
    shifter_exist = true;
  }
  Wire.endTransmission(I2C_ADDR_SHIFTER);
  tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  delay(1000);
  tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  derailleur.ServoOn();

}

void loop()
{

  if (lcd_exist) {
    lcd.setFontSize(FONT_SIZE_LARGE);
    lcd.setCursor(0, 0);
    if (mode == 3) {
      lcd.print("T:");
    } else {
      lcd.print("F:");
    }
    if (mode == 4) {
      lcd.printInt(*pe.getGear(), 2);
    } else if (mode == 3) {
      lcd.printInt(pe.trim(), 2);
    } else {
      lcd.printInt(*pe.getGearPosition(), 2);
    }
    lcd.print(":");
    lcd.printInt(derailleur.getServoMicroSeconds(0), 4);
    lcd.setCursor(0, 3);
    if (mode == 3) {
      lcd.print("S:");
    } else {
      lcd.print("R:");
    }
    if (mode == 4) {
      lcd.printInt(*(pe.getGear() + 1), 2);
    } else if (mode == 3) {
      lcd.printInt(pe.sync(), 2);
    } else {
      lcd.printInt(*(pe.getGearPosition() + 1), 2);
    }
    lcd.print(":");
    lcd.printInt(derailleur.getServoMicroSeconds(1), 4);
    lcd.setCursor(0, 6);
    lcd.print("MODE:");
    lcd.printInt(mode, 2);
  }
//  if (shifter_exist) {
    Wire.beginTransmission(I2C_ADDR_SHIFTER);
    Wire.requestFrom(I2C_ADDR_SHIFTER, 1);    // request 6 bytes from slave device #2
    while (Wire.available())
    {
      byte incomingByte = Wire.read();
      if (incomingByte != 0) {
        switch (incomingByte) {
          case 1:
            bLeftUp.Trigger();
            break;
          case 2:
            bLeftDown.Trigger();
            break;
          case 3:
            bRightUp.Trigger();
            break;
          case 4:
            bRightDown.Trigger();
            break;
          case 5:
            bSet.Trigger();
            break;

        }
        Serial.print("I2C::");
        Serial.println(incomingByte);
      }
    }
    Wire.endTransmission(I2C_ADDR_SHIFTER);
//  }



  if (bSet.check()) {
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
    if (mode < 4) {
      mode++;

    } else {
      mode = 0;
      pe.init(false);
      pe.tuneAll();
      pe.save();
    }

    if (mode == 0) {
      shifter->Refresh();
      derailleur.trim(pe.trim());
      shifter->sync(pe.sync());
      bLeftUp.setLatency(BUTTON_NORMAL_LATENCY);
      bLeftDown.setLatency(BUTTON_NORMAL_LATENCY);
    } else if (mode == 1 ) {
      derailleur.trim(false);
      shifter->sync(false);
      bLeftUp.setLatency(BUTTON_NORMAL_LATENCY);
      bLeftDown.setLatency(BUTTON_NORMAL_LATENCY);
    } else if (mode == 2 ) {
      derailleur.trim(true);
      shifter->sync(false);
      bLeftUp.setLatency(BUTTON_SHORT_LATENCY);
      bLeftDown.setLatency(BUTTON_SHORT_LATENCY);
    }
    else if ( mode == 3) {

      bLeftUp.setLatency(BUTTON_NORMAL_LATENCY);
      bLeftDown.setLatency(BUTTON_NORMAL_LATENCY);
    } else if (mode == 4) {
      derailleur.trim(false);
      shifter->sync(false);
      bLeftUp.setLatency(BUTTON_NORMAL_LATENCY);
      bLeftDown.setLatency(BUTTON_NORMAL_LATENCY);

    }


    pe.print();
    shifter->print();
    Serial.print("MODE:");
    Serial.println(mode);
    Serial.print("LCD:");
    Serial.print(lcd_exist);
    Serial.print("   Shifter:");
    Serial.println(shifter_exist);
  }
  if (mode == 0) {
    Mode0();
  } else if (mode == 1) {
    Mode1();
  } else if (mode == 2) {
    Mode2();
  } else if (mode == 3) {
    Mode3();
  } else if (mode == 4) {
    Mode4();
  }

  derailleur.trim();

  Command();
  delay(LOOP_DELAY);
}



void Command() {
  if (Serial.available() > 0)
  {
    char comdata = Serial.read();
    if (comdata != (char) 0) {
      Serial.print("input Command:");
      Serial.println(comdata);
    }
    if (comdata == 'r')
    {
      pe.init(true);
    } else if (comdata == 'n') {
      pe.init(false);
    }
  }
}



void Mode0() {
  digitalWrite(LED_BUILTIN, HIGH);
  if (bLeftUp.check()) {
    shifter->leftUp();
    Serial.println("Button:LEFT  UP");
  }
  if (bLeftDown.check()) {
    shifter->leftDown();
    Serial.println("Button:LEFT  Down");
  }
  if (bRightUp.check()) {
    shifter->rightUp();
    Serial.println("Button:RIGHT UP");
  }
  if (bRightDown.check()) {
    shifter->rightDown();
    Serial.println("Button:RIGHT Down");
  }
}



void Mode4() {
  if (millis() % 200 > 100) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (bLeftUp.check()) {
    if (*(pe.getGear()) < 8) {
      *(pe.getGear()) = *(pe.getGear()) + 2;
    }
    Serial.print("Button:LEFT  GEAR+");
    Serial.println(*(pe.getGear()));
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
  if (bLeftDown.check()) {
    if (*(pe.getGear()) > 0) {
      *(pe.getGear()) = *(pe.getGear()) - 2;
    }
    Serial.print("Button:LEFT  GEAR-");
    Serial.println(*(pe.getGear()));
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
  if (bRightUp.check()) {
    if ( *(pe.getGear() + 1) < 16) {
      *(pe.getGear() + 1) = *(pe.getGear() + 1) + 1;
    }
    Serial.print("Button:RIGHT GEAR+");
    Serial.println( *(pe.getGear() + 1));
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
  if (bRightDown.check()) {
    if ( *(pe.getGear() + 1) > 0) {
      *(pe.getGear() + 1) = *(pe.getGear() + 1) - 1;
    }
    Serial.print("Button:RIGHT GEAR-");
    Serial.println( *(pe.getGear() + 1));
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
}
void Mode1() {
  if (millis() % 400 > 100) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (bLeftUp.check()) {
    shifter->leftUp();
    Serial.println("Button:LEFT  UP");
  }
  if (bLeftDown.check()) {
    shifter->leftDown();
    Serial.println("Button:LEFT  DOWN");
  }
  if (bRightUp.check()) {
    derailleur.tune(0, 1);
    Serial.println("Button:LEFT  TUNE+");
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
  if (bRightDown.check()) {
    derailleur.tune(0, -1);
    Serial.println("Button:LEFT  TUNE-");
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
}
void Mode2() {
  if (millis() % 800 > 100) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (bLeftUp.check()) {
    derailleur.tune(1, 1);
    Serial.println("Button:RIGHT TUNE+");
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
  if (bLeftDown.check()) {
    derailleur.tune(1, -1);
    Serial.println("Button:RIGHT TUNE-");
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
  if (bRightUp.check()) {
    if (*(pe.getGear() + 1) >= 4) {
      derailleur.shiftTo(*pe.getGearPosition(), *(pe.getGear() + 1) - 2);
    } else {
      shifter->rightUp();
    }
    Serial.println("Button:RIGHT UP");
  }
  if (bRightDown.check()) {
    if (*(pe.getGear() + 1) >= 4) {
      derailleur.shiftTo(*pe.getGearPosition(), 1);
    } else {
      shifter->rightDown();
    }
    Serial.println("Button:RIGHT DOWN");
  }
}

void Mode3() {
  if (millis() % 1200 > 100) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (bLeftUp.check()) {
    //derailleur.tune(1, 1);
    pe.trim(true);
    Serial.println("Button:Trim on");
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
  if (bLeftDown.check()) {
    //derailleur.tune(1, -1);
    pe.trim(false);
    Serial.println("Button:Trim off");
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
  }
  if (bRightUp.check()) {
    //shifter->rightUp();
    pe.sync(true);
    Serial.println("Button:SYNC ON");
  }
  if (bRightDown.check()) {
    //shifter->rightDown();
    pe.sync(false);
    Serial.println("Button:SYNC OFF");
  }
}



