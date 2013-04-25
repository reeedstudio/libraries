/*
Copyright 2011 Niels Brouwers

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.#include <string.h>
*/

/**
 *
 * Library for the max3421e USB host controller shield produced by circuitsathome and Sparkfun.
 * This is a low-level interface that provides access to the internal registers and polls the
 * controller for state changes.
 *
 * This library is based on work done by Oleg Mazurov, but has been heavily restructured.
 *
 * Control over the GPIO pins has been stripped.
 *
 * http://www.circuitsathome.com/
 */
#ifndef __max3421e_h__
#define __max3421e_h__

#include "max3421e_constants.h"
#include "pins_arduino.h"

/**
 * Max3421e registers in host mode.
 */
typedef enum
{
	MAX_REG_RCVFIFO = 0x08,
	MAX_REG_SNDFIFO = 0x10,
	MAX_REG_SUDFIFO = 0x20,
	MAX_REG_RCVBC = 0x30,
	MAX_REG_SNDBC = 0x38,
	MAX_REG_USBIRQ = 0x68,
	MAX_REG_USBIEN = 0x70,
	MAX_REG_CPUCTL = 0x80,
	MAX_REG_USBCTL = 0x78,
	MAX_REG_PINCTL = 0x88,
	MAX_REG_REVISION = 0x90,
	MAX_REG_FNADDR = 0x98,
	MAX_REG_GPINIRQ = 0xb0,
	MAX_REG_HIRQ = 0xc8,
	MAX_REG_HIEN = 0xd0,
	MAX_REG_MODE = 0xd8,
	MAX_REG_PERADDR = 0xe0,
	MAX_REG_HCTL = 0xe8,
	MAX_REG_HXFR = 0xf0,
	MAX_REG_HRSL = 0xf8
} max_registers;

/*
#define PIN_MAX_SS 10
#define PIN_MAX_INT 9
#define PIN_MAX_GPX 8
#define PIN_MAX_RESET 7

#define MAX_SS(x) digitalWrite(SS, x)
#define MAX_INT() digitalRead(PIN_MAX_INT)
#define MAX_GPX() digitalRead(PIN_MAX_INT)
#define MAX_RESET(x) digitalWrite(PIN_MAX_RESET, x)
*/

#define ADK_REF_BOARD

// Completely untested guesswork for the ADK reference board


#define PIN_MAX_SS 53
#define PIN_MAX_INT 9
#define PIN_MAX_GPX 8
#define PIN_MAX_RESET 7

#define MAX_SS(x) digitalWrite(SS, x)
#define MAX_INT() ((PORTE & 0x40) >> 6)
#define MAX_GPX() ((PORTJ & 0x08) >> 3)
#define MAX_RESET(x) { if (x) PORTJ |= 0x04; else PORTJ &= ~0x04; }



void max3421e_init();
void max3421e_write(uint8_t reg, uint8_t val);
uint8_t * max3421e_writeMultiple(uint8_t reg, uint8_t count, uint8_t * values);
void max3421e_gpioWr(uint8_t val);
uint8_t max3421e_read(uint8_t reg);
uint8_t * max3421e_readMultiple(uint8_t reg, uint8_t count, uint8_t * values);
uint8_t max3421e_gpioRd(void);
boolean max3421e_reset();
boolean max3421e_vbusPwr(boolean action);
void max3421e_busprobe(void);
void max3421e_powerOn();
uint8_t max3421e_getVbusState();

uint8_t max3421e_poll(void);

uint8_t max3421e_interruptHandler(void);
uint8_t max3421e_gpxInterruptHandler(void);

#endif //_MAX3421E_H_
