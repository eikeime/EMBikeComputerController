#include <Arduino.h>
#include <Servo.h>
#include "PrefEditor.h"
#include "Derailleur.h"
#include "Shifter.h"
#include "ShifterManual.h"


//ShifterManual::ShifterManual(Derailleur * derailleur,uint8_t *gear,uint8_t *gp)
//{
//
//}
void ShifterManual::rightUp()
{
  if (*(gearPosition + 1) < *(gear + 1) - 1)
  {
    (*d).shiftTo(*gearPosition, *(gearPosition + 1) + 1);

  }
}
void ShifterManual::rightDown()
{
  if (*(gearPosition + 1) > 0)
  {
    (*d).shiftTo(*gearPosition, *(gearPosition + 1) - 1);

  }
}
void ShifterManual::leftUp() {
  if (*gearPosition < *gear - 1)
  {
    (*d).shiftTo(*gearPosition + 1, *(gearPosition + 1));
  }
}
void ShifterManual::leftDown() {
  if (*gearPosition > 0)
  {
    (*d).shiftTo(*gearPosition - 1, *(gearPosition + 1));

  }
}

