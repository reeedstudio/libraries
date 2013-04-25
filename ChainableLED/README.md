ChainableLED
============

Arduino library compatible with Grove Chainable LED and the P9813 chip. It allows controlling a chain of LEDS individually. 
Supports both RGB and HSB color spaces for setting the color of each individual LED.

[More information on the wiki](https://github.com/pjpmarques/ChainableLED/wiki)


Installation
============
1. Grab the latest version from the download section of GitHub.
(https://github.com/pjpmarques/ChainableLED/downloads)

2. Unzip it to your Arduino "libraries" directory. 

3. It should be ready to use. Examples are included.


Library Interface
=================
```c++
    class ChainableLED {
      public:
        ChainableLED(byte clk_pin, byte data_pin, byte number_of_leds);

        void setColorRGB(byte led, byte red, byte green, byte blue);
        void setColorHSB(byte led, float hue, float saturation, float brightness);
    }
```