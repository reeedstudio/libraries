// demo of pwm_lvc, by loovee
#include "pwm_lvc.h"
void setup()
{
    PWM.init();  // init
    PWM.setPwm(9, 20, 5000);        // pin: 9,  duty: 20%, freq: 5kHz
    PWM.setPwm(10, 80, 5000);       // pin: 10, duty: 80%, freq: 5kHz

}

void loop()
{
}
