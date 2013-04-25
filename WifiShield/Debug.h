#ifndef __DEBUG_H__
#define __DEBUG_H__

static const char sChar = 0x2;
static const char eChar = 0x3;
static const char dStringStart = ':';

//#define DEBUG
#define DEBUG_LEVEL 0

#define DEBUG_LOG(level, message) \
  if (DEBUG_LEVEL >= level) {\
	Serial.print(sChar);\
	Serial.print(dStringStart);\
    Serial.print(message);\
	Serial.print(eChar);\
  };
#endif
