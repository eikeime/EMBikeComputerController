#ifndef ShifterManual_h

#define ShifterManual_h

class ShifterManual : public Shifter
{
  protected:



  public:
    ShifterManual():Shifter(){};
//    void Setup(Derailleur * derailleur, uint8_t *gear, int8_t *gp):Setup(derailleur,gear,gp);
    void rightUp();
    void rightDown();
    void leftUp();
    void leftDown();







};
#endif

