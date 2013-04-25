/*************************   2012 Seeedstudio   **************************
* File Name          : Collision sensor.pde
* Author             : Seeedteam
* Version            : V1.0
* Date               : 8/2/2012
* Description        : The program be use to detect the collison
*************************************************************************/

const unsigned char interrupt_num = 0;// use enternal interrupt 0
const unsigned char interrupt_pinnum = 2;//when change interrupt_num,this variance must be changed
const unsigned char LED_pin = 13;
unsigned int LED_status = 0;
//unsigned char number;
unsigned char counter= 0;

void setup()
{
	//Serial.begin(9600);
	pinMode(LED_pin,OUTPUT);
	attachInterrupt(interrupt_num,led_status,RISING); //set external interrupt 0
	//Serial.println("start~~");
}
void led_status()
{
	//detachInterrupt(0);//stop external interrupt 0
	//EIMSK|=1<<INT0;//clear interrupt 0 flag bit
	//number=++number;
	time_interrupt_init();//init timer 2
	//Serial.println(number,DEC);
}
void loop()
{

}

void time_interrupt_init()
{
	//TCCR2A|=(1<<COM2A1)|(1<<WGM21);//set compare mode
	//TCCR2B|=(1<<CS21)|(1<<CS20);//set 1/32 prescaler
	//OCR2A  =0xFA;//set compare register A as dec 250
	//TIMSK2|=(1<<OCIE2A);
	
	//following code is more effective

	TCCR2A=0x82;//set compare mode
	TCCR2B=0x03;//set 1/32 prescaler
	OCR2A=0xFA;//set compare register A as dec 250
	TIMSK2=0x02;//Timer/Counter2 Output Compare Match A Interrupt Enable
}

ISR(TIMER2_COMPA_vect)
{
	++counter;
	if(counter==100)// set duty to 50ms(0.5*100)
	{
		counter=0;
		if(digitalRead(interrupt_pinnum)==0)//check interrupt pin voltage level
		{
			//you can set the flag bit here
			LED_status=~LED_status;
			digitalWrite(LED_pin,LED_status);
			TCCR2B=0x00; //stop timer 2
			//attachInterrupt(interrupt_num,led_status,RISING);
		}
	}
}