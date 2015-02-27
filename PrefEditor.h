#ifndef PrefEditor_h

#define PrefEditor_h
class PrefEditor
{
  private:
    int8_t gearPosition[2];

  public:

    PrefEditor();
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
    void tuneAll();
};
#endif

