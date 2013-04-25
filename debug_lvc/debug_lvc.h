// debug file.  uart
#ifndef _DEBUG_LVC_H_
#define _DEBUG_LVC_H_

#define __printf DBG_PRINT
#define __SerialInit DBG_SetBaudRate
void DBG_SetBaudRate(unsigned long ulBaudRate);    // set baudrate
int DBG_PRINT(const char* format, ...);            // printf ??

#endif
