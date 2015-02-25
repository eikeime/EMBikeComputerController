#ifndef Shifter_h

#define Shifter_h
class Shifter
{
  protected:
  
    Derailleur * d;
    static float ratio[4][16];
    uint8_t *gear;
  
    int8_t * gearPosition;
    bool isSync = true;
  public:
    Shifter();
    //Shifter(Derailleur * derailleur, uint8_t *gear, int8_t *gp);
    void Setup(Derailleur * derailleur, uint8_t *g, int8_t *gp);
    void Refresh();
    void print();
    void setSync(bool b);
    virtual void rightUp()=0;
    virtual void rightDown()=0;
    virtual void leftUp()=0;
    virtual void leftDown()=0;
    virtual void sync(int8_t direction)=0;

};

#endif

