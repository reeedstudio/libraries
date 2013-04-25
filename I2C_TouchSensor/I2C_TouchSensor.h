/* I2C touch sensor lib
by SeeeedStudio--lg
2012/9/12
*/
#ifndef _I2C_TOUCHSENSOR_H_
#define _I2C_TOUCHSENSOR_H_
#include <Arduino.h>
#include <Wire.h>

class I2C_TouchSensor
{
    public:
    void initialize();
    void readTouchInputs();
    void  getTouchState();
    unsigned int touched;
    bool isTouch(int n);          // if something on no.n, return true else false
    
    private:
    int _irqPin;
    bool _touchStates[12];
    void mpr121_setup();
    bool checkInterrupt();
    void set_register(int address, unsigned char r, unsigned char v);
};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/