


#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Servo.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <Button.h>
#include <MicroLCD.h>

#include "config.h"
#include "PrefEditor.h"
#include "Derailleur.h"
#include "Shifter.h"
#include "ShifterSmart.h"
#include "ShifterManual.h"


LCD_SSD1306 lcd;
PrefEditor pe;

Shifter *shifter;
Derailleur derailleur = Derailleur();
ShifterManual shifterManual = ShifterManual();


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
  Serial.begin(9600);
  Serial.println("Listening...");
  pe = PrefEditor();
  if (pe.init()) {
    mode = 1;
  }
  pe.print();
  derailleur.setPref(&pe);
  shifterManual.Setup(&derailleur, pe.getGear(), pe.getGearPosition());
  //shifterManual.Refresh();
  shifter = &shifterManual;
  delay(200);
  derailleur.ServoOn();
  lcd.begin();
  lcd.clear();
}

void loop()
{


  lcd.setCursor(0, 0);
  lcd.print("F:");
  lcd.setFontSize(FONT_SIZE_LARGE);
  lcd.printInt(*pe.getGearPosition(), 2);
  lcd.print(":");
  lcd.printInt(derailleur.getServoSeconds(0), 4);

  lcd.setCursor(0, 3);
  lcd.setFontSize(FONT_SIZE_LARGE);
  lcd.print("R:");
  lcd.printInt(*(pe.getGearPosition() + 1), 2);
  lcd.print(":");
  lcd.printInt(derailleur.getServoSeconds(1), 4);

  lcd.setCursor(0, 6);
  lcd.setFontSize(FONT_SIZE_LARGE);
  lcd.print("MODE:");
  lcd.printInt(mode, 2);

  if (bSet.check()) {
    tone(SPEAKER_PIN, SPEAKER_FREQ, SPEAKER_DUR);
    if (mode < 3) {
      mode++;

    } else {
      mode = 0;
      pe.init(false);
      pe.save();
    }
    
    if (mode == 0) {
      derailleur.trim(true);
      shifter->setSync(true);
      bLeftUp.setLatency(BUTTON_NORMAL_LATENCY);
      bLeftDown.setLatency(BUTTON_NORMAL_LATENCY);
    } else if (mode == 1 ) {
      //shifter->Refresh();
      derailleur.trim(false);
      shifter->setSync(false);
    } else if (mode == 2 ) {
      shifter->Refresh();
      derailleur.trim(false);
    }
    else if ( mode == 3) {
      derailleur.trim(true);
      shifter->setSync(false);
      bLeftUp.setLatency(BUTTON_SHORT_LATENCY);
      bLeftDown.setLatency(BUTTON_SHORT_LATENCY);

    }else if(mode ==4){
    
    
    }


    pe.print();
    shifter->print();
    Serial.print("MODE:");
    Serial.println(mode);
  }
  if (mode == 0) {
    Mode0();
  } else if (mode == 1) {
    Mode1();
  } else if (mode == 2) {
    Mode2();
  } else if (mode == 3) {
    Mode3();
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



void Mode1() {
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
void Mode2() {
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
void Mode3() {
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
    shifter->rightUp();
    Serial.println("Button:RIGHT UP");
  }
  if (bRightDown.check()) {
    shifter->rightDown();
    Serial.println("Button:RIGHT DOWN");
  }

}

void Mode4() {
  if (millis() % 1000 > 100) {
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
    shifter->rightUp();
    Serial.println("Button:RIGHT UP");
  }
  if (bRightDown.check()) {
    shifter->rightDown();
    Serial.println("Button:RIGHT DOWN");
  }




}



