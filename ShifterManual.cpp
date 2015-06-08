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
  if (*(gearPosition + 1) < * (gear + 1) - 1)
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
  if (*gearPosition < *gear - 1 - d->isTrim())
  {
    (*d).shiftTo(*gearPosition + 1, *(gearPosition + 1));
    sync(1);
  }
}
void ShifterManual::leftDown() {
  if (*gearPosition > 0)
  {
    (*d).shiftTo(*gearPosition - 1, *(gearPosition + 1));
    sync(-1);
  }
}

void ShifterManual::sync(int8_t direction) {
  if (isSync) {
    if(direction==-1){
    delay(500);
    }

    int g = *gearPosition;
    if (g % 2 == 1) {
      g -= 1;

    }
    g /= 2;

    int i = 0;
    while (i < * (gear + 1) - 1 && ratio[g][i] < ratio[g - direction][*(gearPosition + 1)]) {

      i++;
    }
    if (i == 0) {
      (*d).shiftTo(*gearPosition, i);

    } else {
      if ((ratio[g - direction][*(gearPosition + 1)] - ratio[g][i - 1]) > ratio[g][i ] - ratio[g - direction][*(gearPosition + 1)]) {
        (*d).shiftTo(*gearPosition, i  );
      } else {
        (*d).shiftTo(*gearPosition, i - 1);
      }
    }
  }
}

