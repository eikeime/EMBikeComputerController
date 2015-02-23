#include <Arduino.h>
#include <Servo.h>
#include "PrefEditor.h"
#include "Derailleur.h"
#include "Shifter.h"
#include "ShifterSmart.h"


//ShifterSmart::ShifterSmart(Derailleur * derailleur,uint8_t *gear,uint8_t *gp)
//{
//
//}
void ShifterSmart::rightUp()
{
  uint8_t front;
  if (*gearPosition < *gear - 2)
  {
    //(*d).shiftTo();//<<========================mission

  }

}
void ShifterSmart::rightDown()
{




}

