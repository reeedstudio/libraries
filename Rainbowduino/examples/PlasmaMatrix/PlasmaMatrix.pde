/*

 Rainbowduino v3.0 Library examples : 2D Plasma

*/

#include <Rainbowduino.h>


// HSV to RGB array generated using the following perl script

/*

#HSV2RGB.pl
use Graphics::Color::HSV;
use POSIX qw(ceil floor);

$red="";
$green="";
$blue="";

for($hu = 0; $hu <256 ; $hu=$hu+4)
{
my $color = Graphics::Color::HSV->new({ hue => $hu, saturation  => 1, value  => 1,});
$red.=ceil($color->to_rgb()->red * 255) . ',';
$green.=ceil($color->to_rgb()->green * 255) . ',';
$blue.=ceil($color->to_rgb()->blue * 255) . ',';
}

print "unsigned char RED[64] = {$red};";
print "\n";
print "unsigned char GREEN[64] = {$green};";
print "\n";
print "unsigned char BLUE[64] = {$blue};";
print "\n";

*/

unsigned char RED[64] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,238,221,204,188,171,154,137,119,102,85,
68,51,34,17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,17,35,52};

unsigned char GREEN[64] = {0,17,34,51,68,85,102,119,136,153,170,187,204,221,238,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,238,221,204,188,170,154,136,120,102,86,68,52,34,18,0,0,0,0};

unsigned char BLUE[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,34,52,68,86,102,120,136,154,170,188,
204,221,238,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};

unsigned char plasma[8][8];


void setup()
{
  Rb.init(); //initialize Rainbowduino driver
  
  for(unsigned char x = 0; x < 8; x++)
  {
    for(unsigned char y = 0; y < 8; y++)
    {
        int color = int(32.0 + (32.0 * sin(x / 4.0)) + 32.0 + (32.0 * sin(y / 4.0))) / 2;
        plasma[x][y] = color;      
    }
  }
}

unsigned char x,y,colorshift=0;

void loop()
{
 for(x=0;x<8;x++)
 {
    for(y=0;y<8;y++)
    {
     Rb.setPixelXY(x,y,(RED[plasma[x][y] + colorshift]) % 256,(GREEN[plasma[x][y] + colorshift]) % 256,(BLUE[plasma[x][y] + colorshift]) % 256); //uses R, G and B color bytes
    }
 }
 delay(100);
 colorshift=  colorshift + 1;
}

