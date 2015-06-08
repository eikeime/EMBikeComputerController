#ifndef PrefEditor_h

#define PrefEditor_h
class PrefEditor
{
  private:
    int8_t gearPosition[2];

  public:

    PrefEditor();
    int16_t gap=0;
    void initGearPref();
    uint8_t * getGear();
    int16_t * getServoPositionList();
    int8_t * getGearPosition();
    bool init();
    void init(bool b);
    void read();
    void save();
    void print();
    void readGearPosition();
    void saveGearPosition();
    void trim(bool b);
    bool trim();
    void sync(bool b);
    bool sync();
    void tuneAll();
    
};
#endif


