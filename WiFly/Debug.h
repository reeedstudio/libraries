
#ifndef __DEBUG_H__
#define __DEBUG_H__

//#define DEBUG

#ifdef DEBUG
#define DBG(message)    Serial.print(message)
#else
#define DBG(message)
#endif // DEBUG

#endif // __DEBUG_H__
