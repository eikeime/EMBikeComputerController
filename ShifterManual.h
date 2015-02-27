#ifndef ShifterManual_h

#define ShifterManual_h

class ShifterManual : public Shifter
{
  protected:



  public:
    ShifterManual():Shifter(){};
    void rightUp();
    void rightDown();
    void leftUp();
    void leftDown();
    void sync(int8_t direction);







};
#endif

