#include <Sleep_n0m1.h>
#include <Wire.h>
#include <I2C_Uart.h>

Sleep sleep;

#define PINSYSPWR                   8                   // system power pin
#define PINGROVEPWR                 7                   // grove power pin

#if defined(__AVR_ATmega32U4__)
#define power_adc_enable()      (PRR0 &= (uint8_t)~(1 << PRADC))
#define power_adc_disable()     (PRR0 |= (uint8_t)(1 << PRADC))

#define power_usart0_enable()   (PRR0 &= (uint8_t)~(1 << PRUSART0))
#define power_usart0_disable()  (PRR0 |= (uint8_t)(1 << PRUSART0))

#define power_spi_enable()      (PRR0 &= (uint8_t)~(1 << PRSPI))
#define power_spi_disable()     (PRR0 |= (uint8_t)(1 << PRSPI))

#define power_twi_enable()      (PRR0 &= (uint8_t)~(1 << PRTWI))
#define power_twi_disable()     (PRR0 |= (uint8_t)(1 << PRTWI))

#define power_timer0_enable()   (PRR0 &= (uint8_t)~(1 << PRTIM0))
#define power_timer0_disable()  (PRR0 |= (uint8_t)(1 << PRTIM0))

#define power_timer1_enable()   (PRR0 &= (uint8_t)~(1 << PRTIM1))
#define power_timer1_disable()  (PRR0 |= (uint8_t)(1 << PRTIM1))

#define power_timer2_enable()   (PRR0 &= (uint8_t)~(1 << PRTIM2))
#define power_timer2_disable()  (PRR0 |= (uint8_t)(1 << PRTIM2))

#define power_timer3_enable()   (PRR1 &= (uint8_t)~(1 << PRTIM3))
#define power_timer3_disable()  (PRR1 |= (uint8_t)(1 << PRTIM3))

#define power_usart1_enable()   (PRR1 &= (uint8_t)~(1 << PRUSART1))
#define power_usart1_disable()  (PRR1 |= (uint8_t)(1 << PRUSART1))

#define power_usb_enable()      (PRR1 &= (uint8_t)~(1 << PRUSB))
#define power_usb_disable()     (PRR1 |= (uint8_t)(1 << PRUSB))

#endif

#define grovePwrOn()    digitalWrite(PINGROVEPWR, LOW)
#define grovePwrOff()   digitalWrite(PINGROVEPWR, HIGH)

void setup()
{
   //Serial.begin(9600);
   I2C_Uart_Init();                                     // For debug, I2c -> Uart
   pinMode(PINSYSPWR, OUTPUT);
   digitalWrite(PINSYSPWR, HIGH);                       // turn system
   __println("init over");
  // pinMode(13, OUTPUT);
   pinMode(PINGROVEPWR, OUTPUT);
   grovePwrOn();

}

void pwrDown(unsigned long tSleep)
{

    grovePwrOff();
    
#if defined(__AVR_ATmega32U4__)

    power_adc_disable();

    power_usart0_disable();

    power_spi_disable();
    
    power_twi_disable();

    power_timer1_disable();

    power_timer2_disable();

    power_timer3_disable();

    power_usart1_disable();

    power_usb_disable();
#endif

    sleep.pwrDownMode();        //set sleep mode
    sleep.sleepDelay(tSleep);   //sleep for: sleepTime
}

void wakeUp()
{
#if defined(__AVR_ATmega32U4__)
    power_adc_enable();

    power_usart0_enable();

    power_spi_enable();

    power_twi_enable();

    power_timer1_enable();

    power_timer2_enable();

    power_timer3_enable();

    power_usart1_enable();

    power_usb_enable();
#endif
    grovePwrOn();
}

void loop()
{

   // Serial.println("awake for 3s");
   
  //  digitalWrite(13, HIGH);
    delay(3000);
   // Serial.println("power down for 3s");
   
   // digitalWrite(13, LOW);
  //  delay(100);
    
    pwrDown(3000);
    //delay(1);
    wakeUp();
}