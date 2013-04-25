
#ifndef _RFBEEGLOBALS_H_
#define _RFBEEGLOBALS_H_
//return values 0: ok, -1: error, 1: nothing , 2: modified
#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>

#include "RFBeeCCx.h"
#include "RFBeeCCxCfg.h"
#include "RFBeeConfig.h"
#include "RFBeeCore.h"
#include "RFBeeSerial.h"
#include "RFBeeSpi.h"


#define FIRMWAREVERSION 11 // 1.1  , version number needs to fit in unsigned char (0~255) to be able to store it into config

#define OK 0
#define ERR -1 
#define NOTHING 1
#define MODIFIED 2

#endif
