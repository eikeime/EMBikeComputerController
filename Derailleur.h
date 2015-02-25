#ifndef Derailleur_h

#define Derailleur_h
class Derailleur
{
  protected:

     uint8_t *gear;
    int8_t * gearPosition;
    int16_t *servoPositionList;
    uint32_t shiftTimeStamp = 0;
    PrefEditor *pe;

    Servo servo[2];
    int8_t servoStep[2];
    boolean needTrim = false;
    boolean trim_switch = true;

  public:
    Derailleur();
    Derailleur(PrefEditor * pref_editor);
    void setPref(PrefEditor * pref_editor);
//    void prepare(uint8_t  *gear);
//    void setupGear(uint8_t *gear, uint8_t *gp); //setup gear teeth & ratio

    void setupServo(uint16_t *servoPosition);
    void ServoOn();
    void ServoOff();
    void setupServo(int id, uint8_t sStep);
    void tune(int id,int direction);
    void trim(bool b);
    void shiftTo(uint8_t front, uint8_t rear);
    void trim();
    bool isTrim();
    int getServoSeconds(int i);
};


#endif





