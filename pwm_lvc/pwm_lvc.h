#ifndef _PWM_LVC_H_
#define _PWM_LVC_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER1COUNT 65536    // Timer1 is 16 bit

class pwm_lvc
{
    private:
    unsigned int pwmPeriod;
    unsigned char clockSelectBits;
    
    public:
    
    void init();
    void setPwm(char pin, int duty, long freq);     // Hz
    void disablePwm(char pin);                      // pin = 9 or 10
    void setFreq(long freq);                        // Hz
    void setPwmDuty(char pin, int duty);            // duty: 0-100 %
};

extern pwm_lvc PWM;
#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

