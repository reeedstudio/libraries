#include "pwm_lvc.h"

void pwm_lvc::init()
{
    TCCR1A = 0;                                 // clear control register A
    TCCR1B = _BV(WGM13);                        // set mode 8: phase and frequency correct pwm, stop the timer
}

void pwm_lvc::setFreq(long freq)                // AR modified for atomic access
{

    long cycles = (F_CPU / 2000000) * (1000000/freq);                               // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
    if(cycles < TIMER1COUNT)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
    else if((cycles >>= 3) < TIMER1COUNT) clockSelectBits = _BV(CS11);              // prescale by /8
    else if((cycles >>= 3) < TIMER1COUNT) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
    else if((cycles >>= 2) < TIMER1COUNT) clockSelectBits = _BV(CS12);              // prescale by /256
    else if((cycles >>= 2) < TIMER1COUNT) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
    else        cycles = TIMER1COUNT - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum

    char tmp = SREG;
    cli();                                                                          // Disable interrupts for 16 bit register access
    ICR1 = pwmPeriod = cycles;                                                      // ICR1 is TOP in p & f correct setPwm mode
    SREG = tmp;
    TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
    TCCR1B |= clockSelectBits;                                                      // reset clock select register, and starts the clock
}

// duty, 0-100
void pwm_lvc::setPwmDuty(char pin, int duty)
{
    if(duty >100)duty = 100;
    if(duty < 0) duty = 0;
    unsigned long dutyCycle = pwmPeriod*10;
    dutyCycle *= duty;
    dutyCycle >>= 10;
    char tmp = SREG;
    cli();
    if(pin == 1 || pin == 9)       OCR1A = dutyCycle;
    else if(pin == 2 || pin == 10) OCR1B = dutyCycle;
    SREG = tmp;
}

// setPwm, pin: 9, 10
void pwm_lvc::setPwm(char pin, int duty, long freq)                 // expects duty cycle to be 10 bit (1024)
{
    if((pin != 9 && pin != 10) || freq <= 0)return ;                // error parament

    setFreq(freq);
    
    if(pin == 9) {
        DDRB |= _BV(PORTB1);                                        // sets data direction register for setPwm output pin
        TCCR1A |= _BV(COM1A1);                                      // activates the output pin
    }
    else
    {
        DDRB |= _BV(PORTB2);
        TCCR1A |= _BV(COM1B1);
    }
    setPwmDuty(pin, duty);

}

void pwm_lvc::disablePwm(char pin)
{
    if(pin == 1 || pin == 9)       TCCR1A &= ~_BV(COM1A1);          // clear the bit that enables setPwm on PB1
    else if(pin == 2 || pin == 10) TCCR1A &= ~_BV(COM1B1);          // clear the bit that enables setPwm on PB2
}

pwm_lvc PWM;                                                        // preinstatiate

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
