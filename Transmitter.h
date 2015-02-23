#ifndef Transmitter_h

#define Transmitter_h
class Transmitter
{
  private:
  public:
    Transmitter(uint8_t cePin, uint8_t csnPin);
    void resetAddr();
    void modeWait();
    void modeScan();
    void modeTarget();

};
#endif

