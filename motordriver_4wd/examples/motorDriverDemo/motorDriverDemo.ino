#include "motordriver_4wd.h"
#include <pwm_lvc.h>

void setup()
{
    MOTOR.init();
}

void loop()
{
    MOTOR.setSpeedDir(DIRF, 80);
    delay(3000);
    MOTOR.setSpeedDir(DIRR, 80);
    delay(3000);
}
/*********************************************************************************************************
 * END FILE
 *********************************************************************************************************/


