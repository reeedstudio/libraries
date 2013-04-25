// Paint application - Demonstate both TFT and Touch Screen

#include <stdint.h>
#include <TouchScreen.h> 
#include <TFT_v2.h>

//#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
//#endif

/*#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif */

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1

#define TS_MINX 140 
#define TS_MAXX 900
#define TS_MINY 120
#define TS_MAXY 940

int color = WHITE;  //Paint brush color

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //init TouchScreen port pins

void setup()
{

Tft.init();  //init TFT library
pinMode(0,OUTPUT);
//Draw the pallet
Tft.fillRectangle(0,0,30,30,BLACK); 
Tft.fillRectangle(30,0,30,30,RED);
Tft.fillRectangle(60,0,30,30,GREEN);
Tft.fillRectangle(90,0,30,30,BLUE);
Tft.fillRectangle(120,0,30,30,CYAN);
Tft.fillRectangle(150,0,30,30,YELLOW);
Tft.fillRectangle(180,0,30,30,WHITE);
Tft.fillRectangle(210,0,30,30,GRAY1);

}

void loop()
{
  
  // a point object holds x y and z coordinates.
  Point p = ts.getPoint();

  //map the ADC value read to into pixel co-ordinates

  p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
  p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);
  
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > ts.pressureThreshhold) {
  
 
 // Detect  paint brush color change
 if(p.y < 35)
  {
    if(p.x >= 0 && p.x < 30)
    {
      color = BLACK;
    }
    if(p.x >= 30 && p.x < 60)
    {
      color = RED;
      digitalWrite(0,HIGH);
    }
    if(p.x >= 60 && p.x < 90)
    {
      color = GREEN;
    }
    if(p.x >= 90 && p.x < 110)
    {
      color = BLUE;
      digitalWrite(0,LOW);
    }
    if(p.x >= 120 && p.x < 150)
    {
      color = CYAN;
    }
    if(p.x >= 150 && p.x < 180)
    {
      color = YELLOW;
    }
    if(p.x >= 180 && p.x < 210)
    {
      color = WHITE;
    }
    if(p.x >= 210 && p.x < 240)
    {
      color = GRAY1;
    }    
  }
  else    
  {
      Tft.init_io();
      Tft.fillCircle(p.x,p.y,2,color);
  }
     
 }

}
