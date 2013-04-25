#include <SdFat.h>
#include <SdFatUtil.h>
SdFat sd;
SdFile file;
#define INCLUDE_GUARD "Sd2PinMap_h"
//------------------------------------------------------------------------------
// 168 - 328 Arduino
uint8_t i2c328[] = {
  18,  // SDA
  19   // SCL
};
uint8_t spi328[] = {
  10,  // SS
  11,  // MOSI
  12,  // MISO
  13   // SCK
};
char pins328[][3] PROGMEM = {
  // 0     1     2     3    4     5      6     7     8     9
  "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "B0", "B1",  //  0 -  9
  "B2", "B3", "B4", "B5", "C0", "C1", "C2", "C3", "C4", "C5"   // 10 - 19
};
uint8_t const nPin328 = sizeof(pins328)/sizeof(char[3]);
//------------------------------------------------------------------------------
// Mega Arduino
uint8_t i2c1280[] = {
  20,  // SDA
  21   // SCL
};
uint8_t spi1280[] = {
  53,  // SS
  51,  // MOSI
  50,  // MISO
  52   // SCK
};
char pins1280[][3] PROGMEM = {
//  0     1     2     3     4     5     6     7     8     9
  "E0", "E1", "E4", "E5", "G5", "E3", "H3", "H4", "H5", "H6",  //  0 -  9
  "B4", "B5", "B6", "B7", "J1", "J0", "H1", "H0", "D3", "D2",  // 10 - 19
  "D1", "D0", "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7",  // 20 - 29
  "C7", "C6", "C5", "C4", "C3", "C2", "C1", "C0", "D7", "G2",  // 30 - 39
  "G1", "G0", "L7", "L6", "L5", "L4", "L3", "L2", "L1", "L0",  // 40 - 49
  "B3", "B2", "B1", "B0", "F0", "F1", "F2", "F3", "F4", "F5",  // 50 - 59
  "F6", "F7", "K0", "K1", "K2", "K3", "K4", "K5", "K6", "K7",  // 60 - 69
};
uint8_t const nPin1280 = sizeof(pins1280)/sizeof(char[3]);
//------------------------------------------------------------------------------
// Sanguino
uint8_t i2c644[] = {
  17,  // SDA
  18   // SCL
};
uint8_t spi644[] = {
  4,  // SS
  5,  // MOSI
  6,  // MISO
  7   // SCK
};
char pins644[][3] PROGMEM = {  //
//  0     1     2     3     4     5     6     7     8     9
  "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "D0", "D1",  //  0 -  9
  "D2", "D3", "D4", "D5", "D6", "D7", "C0", "C1", "C2", "C3",  // 10 - 19
  "C4", "C5", "C6", "C7", "A7", "A6", "A5", "A4", "A3", "A2",  // 20 - 29
  "A1", "A0"                                                   // 30 - 31
};
uint8_t const nPin644 = sizeof(pins644)/sizeof(char[3]);
//------------------------------------------------------------------------------
// Teensy 2.0
uint8_t i2c32U4[] = {
  6,  // SDA
  5   // SCL
};
uint8_t spi32U4[] = {
  0,  // SS
  2,  // MOSI
  3,  // MISO
  1   // SCK
};
char pins32U4[][3] PROGMEM = {
//  0     1     2     3     4     5     6     7     8     9
  "B0", "B1", "B2", "B3", "B7", "D0", "D1", "D2", "D3", "C6",  //  0 -  9
  "C7", "D6", "D7", "B4", "B5", "B6", "F7", "F6", "F5", "F4",  // 10 - 19
  "F1", "F0", "D4", "D5", "E6"                                 // 20 - 24
};
uint8_t const nPin32U4 = sizeof(pins32U4)/sizeof(char[3]);
//------------------------------------------------------------------------------
// Teensy++ 1.0 & 2.0
uint8_t i2c1286[] = {
  1,  // SDA
  0   // SCL
};
uint8_t spi1286[] = {
  20,  // SS
  22,  // MOSI
  23,  // MISO
  21   // SCK
};
char pins1286[][3] PROGMEM = {
//  0     1     2     3     4     5     6     7     8     9
  "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "E0", "E1",  //  0 -  9
  "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "E6", "E7",  // 10 - 19
  "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "A0", "A1",  // 20 - 29
  "A2", "A3", "A4", "A5", "A6", "A7", "E4", "E5", "F0", "F1",  // 30 - 39
  "F2", "F3", "F4", "F5", "F6", "F7"                           // 40 - 45
};
uint8_t const nPin1286 = sizeof(pins1286)/sizeof(char[3]);
//------------------------------------------------------------------------------
#define print_P(stream, str) printPGM(stream, PSTR(str))
#define println_P(stream, str) printlnPGM(stream, PSTR(str))
//------------------------------------------------------------------------------
void printPGM(Print* stream, PGM_P str) {
    for (uint8_t c; (c = pgm_read_byte(str)); str++) stream->write(c);
}
//------------------------------------------------------------------------------
void printlnPGM(Print* stream, PGM_P str) {
  printPGM(stream, str);
  stream->println();
}
//------------------------------------------------------------------------------
void printFunctions(Print* stream) {
  print_P(stream,
    "static const uint8_t digitalPinCount = sizeof(digitalPinMap)/sizeof(pin_map_t);\r\n"
    "\r\n"
    "uint8_t badPinNumber(void)\r\n"
    "  __attribute__((error(\"Pin number is too large or not a constant\")));\r\n"
    "\r\n"
    "static inline __attribute__((always_inline))\r\n"
    "  bool getPinMode(uint8_t pin) {\r\n"
    "  if (__builtin_constant_p(pin) && pin < digitalPinCount) {\r\n"
    "    return (*digitalPinMap[pin].ddr >> digitalPinMap[pin].bit) & 1;\r\n"
    "  } else {\r\n"
    "    return badPinNumber();\r\n"
    "  }\r\n"
    "}\r\n"
    "static inline __attribute__((always_inline))\r\n"
    "  void setPinMode(uint8_t pin, uint8_t mode) {\r\n"
    "  if (__builtin_constant_p(pin) && pin < digitalPinCount) {\r\n"
    "    if (mode) {\r\n"
    "      *digitalPinMap[pin].ddr |= 1 << digitalPinMap[pin].bit;\r\n"
    "    } else {\r\n"
    "      *digitalPinMap[pin].ddr &= ~(1 << digitalPinMap[pin].bit);\r\n"
    "    }\r\n"
    "  } else {\r\n"
    "    badPinNumber();\r\n"
    "  }\r\n"
    "}\r\n"
    "static inline __attribute__((always_inline))\r\n"
    "  bool fastDigitalRead(uint8_t pin) {\r\n"
    "  if (__builtin_constant_p(pin) && pin < digitalPinCount) {\r\n"
    "    return (*digitalPinMap[pin].pin >> digitalPinMap[pin].bit) & 1;\r\n"
    "  } else {\r\n"
    "    return badPinNumber();\r\n"
    "  }\r\n"
    "}\r\n"
    "static inline __attribute__((always_inline))\r\n"
    "  void fastDigitalWrite(uint8_t pin, uint8_t value) {\r\n"
    "  if (__builtin_constant_p(pin) && pin < digitalPinCount) {\r\n"
    "    if (value) {\r\n"
    "      *digitalPinMap[pin].port |= 1 << digitalPinMap[pin].bit;\r\n"
    "    } else {\r\n"
    "      *digitalPinMap[pin].port &= ~(1 << digitalPinMap[pin].bit);\r\n"
    "    }\r\n"
    "  } else {\r\n"
    "    badPinNumber();\r\n"
    "  }\r\n"
    "}\r\n");
}
//------------------------------------------------------------------------------
void printGNU(Print* stream) {
  println_P(stream,
    "/* Arduino SdFat Library\r\n"
    " * Copyright (C) 2010 by William Greiman\r\n"
    " *\r\n"
    " * This file is part of the Arduino SdFat Library\r\n"
    " *\r\n"
    " * This Library is free software: you can redistribute it and/or modify\r\n"
    " * it under the terms of the GNU General Public License as published by\r\n"
    " * the Free Software Foundation, either version 3 of the License, or\r\n"
    " * (at your option) any later version.\r\n"
    " *\r\n"
    " * This Library is distributed in the hope that it will be useful,\r\n"
    " * but WITHOUT ANY WARRANTY; without even the implied warranty of\r\n"
    " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\r\n"
    " * GNU General Public License for more details.\r\n"
    " *\r\n"
    " * You should have received a copy of the GNU General Public License\r\n"
    " * along with the Arduino SdFat Library.  If not, see\r\n"
    " * <http://www.gnu.org/licenses/>.\r\n"
    " */\r\n"
    "// Warning this file was generated by a program.");
}
//----------------------------------------------------------------
void printMap(Print* stream, char (*list)[3], uint8_t nPin) {
  println_P(stream, "static const pin_map_t digitalPinMap[] = {");
  for (uint8_t i = 0; i < nPin; i++) {
    char cPort = pgm_read_byte(&list[i][0]);
    char cBit = pgm_read_byte(&list[i][1]);
    print_P(stream, "  {&DDR");
    stream->write(cPort);
    print_P(stream, ", &PIN");
    stream->write(cPort);
    print_P(stream, ", &PORT");
    stream->write(cPort);
    print_P(stream, ", ");
    stream->write(cBit);
    if ((i+1) != nPin) {
      print_P(stream, "},  // ");
    } else {
      print_P(stream, "}   // ");
    }
    stream->write(cPort);
    stream->write(cBit);
    stream->write(' ');
    if (i < 10) stream->write(' ');
    stream->println(i, DEC);
  }
  println_P(stream, "};");
}
//------------------------------------------------------------------------------
void printLine(Print* stream) {
  print_P(stream, "//");
  for (uint8_t i = 0; i < 78; i++) stream->write('-');
  stream->println();
}
//------------------------------------------------------------------------------
void printI2C(Print* stream, char (*list)[3], uint8_t* i2c) {
  println_P(stream, "// Two Wire (aka I2C) ports");
  print_P(stream, "uint8_t const SDA_PIN = ");
  stream->print(i2c[0], DEC);
  print_P(stream, ";  // ");
  printlnPGM(stream, list[i2c[0]]);
  print_P(stream, "uint8_t const SCL_PIN = ");
  stream->print(i2c[1], DEC);
  print_P(stream, ";  // ");
  printlnPGM(stream, list[i2c[1]]);
  stream->println();
}
//------------------------------------------------------------------------------
void printSPI(Print* stream, char (*list)[3], uint8_t* spi) {
  println_P(stream, "// SPI port");
  print_P(stream, "uint8_t const SS_PIN = ");
  stream->print(spi[0], DEC);
  print_P(stream, ";    // ");
  printlnPGM(stream, list[spi[0]]);
  print_P(stream, "uint8_t const MOSI_PIN = ");
  stream->print(spi[1], DEC);
  print_P(stream, ";  // ");
  printlnPGM(stream, list[spi[1]]);
  print_P(stream, "uint8_t const MISO_PIN = ");
  stream->print(spi[2], DEC);
  print_P(stream, ";  // ");
  printlnPGM(stream, list[spi[2]]);
  print_P(stream, "uint8_t const SCK_PIN = ");
  stream->print(spi[3], DEC);
  print_P(stream, ";   // ");
  printlnPGM(stream, list[spi[3]]);
  stream->println();
}
//-----------------------------------------------
void printType(Print* stream) {
  printLine(stream);
  print_P(stream,
    "/** struct for mapping digital pins */\r\n"
    "struct pin_map_t {\r\n"
    "  volatile uint8_t* ddr;\r\n"
    "  volatile uint8_t* pin;\r\n"
    "  volatile uint8_t* port;\r\n"
    "  uint8_t bit;\r\n"
    "};\r\n");
}
//------------------------------------------------------------------------------
void printSection(Print* stream, uint8_t* i2c,
     uint8_t* spi, char (*list)[3], uint8_t digitalPinCount) {
  stream->println();
  printI2C(stream, list, i2c);
  printSPI(stream, list, spi);
  printMap(stream, list, digitalPinCount);
}
//------------------------------------------------------------------------------
void printFile(Print* stream) {
  printGNU(stream);
  println_P(stream, "#ifndef " INCLUDE_GUARD);
  println_P(stream, "#define " INCLUDE_GUARD);
  println_P(stream, "#include <avr/io.h>");

  printType(stream);
  printLine(stream);
  print_P(stream,
    "#if defined(__AVR_ATmega1280__)\\\r\n"
    "|| defined(__AVR_ATmega2560__)\r\n");
  println_P(stream, "// Mega");
  printSection(stream, i2c1280, spi1280, pins1280, nPin1280);

  printLine(stream);
  print_P(stream,
    "#elif defined(__AVR_ATmega644P__)\\\r\n"
    "|| defined(__AVR_ATmega644__)\\\r\n"
    "|| defined(__AVR_ATmega1284P__)\r\n");
  println_P(stream, "// Sanguino");
  printSection(stream, i2c644, spi644, pins644, nPin644);

  printLine(stream);
  println_P(stream, "#elif defined(__AVR_ATmega32U4__)");
  println_P(stream, "// Teensy 2.0");
  printSection(stream, i2c32U4, spi32U4, pins32U4, nPin32U4);

  printLine(stream);
  print_P(stream,
    "#elif defined(__AVR_AT90USB646__)\\\r\n"
    "|| defined(__AVR_AT90USB1286__)\r\n");
  println_P(stream, "// Teensy++ 1.0 & 2.0");
  printSection(stream, i2c1286, spi1286, pins1286, nPin1286);
 
  printLine(stream);
  print_P(stream,
    "#elif defined(__AVR_ATmega168__)\\\r\n"
    "||defined(__AVR_ATmega168P__)\\\r\n"
    "||defined(__AVR_ATmega328P__)\r\n");
  println_P(stream, "// 168 and 328 Arduinos");
  printSection(stream, i2c328, spi328, pins328, nPin328);
  
  print_P(stream,
    "#else  // defined(__AVR_ATmega1280__)\r\n"
    "#error unknown chip\r\n"
    "#endif  // defined(__AVR_ATmega1280__)\r\n");
  printLine(stream);
  printFunctions(stream);
  println_P(stream, "#endif  // " INCLUDE_GUARD);
}
//------------------------------------------------------------------------------
#define error(x) errorPGM(PSTR(x))
void errorPGM(PGM_P msg) {
  print_P(&Serial, "Error: ");
  printlnPGM(&Serial, msg);
  while (1);
}
//------------------------------------------------------------------------------
void setup(void) {
  Serial.begin(9600);
  Serial.println(FreeRam());
//  printFile(&Serial);
  println_P(&Serial, "Type any character to make PIN_MAP.H");
  while (!Serial.available());
  if (!sd.init()) sd.initErrorHalt();
  if (!file.open("PIN_MAP.H", O_WRITE | O_CREAT | O_TRUNC)) {
    error("file.open");
  }
  printFile(&file);
  file.close();
  println_P(&Serial, "Done");
}
//------------------------------------------------------------------------------
void loop(void) {}
