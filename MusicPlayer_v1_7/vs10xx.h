//  File vs10xx.h
//
//  For more details about the product please check http://www.seeedstudio.com/depot/
//  Copyright (c) 2012 seeed technology inc.
//  Author: Frankie.Chu
//  Version: 2.0
//  Time: June 2, 2012
//  Changing records:
//    
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


#ifndef VS10XX_H
#define VS10XX_H
#include <NewSPI.h>
#include "pins_config.h"

/** VS10xx SCI Write Command byte is 0x02 */
#define VS_WRITE_COMMAND 0x02
/** VS10xx SCI Read Command byte is 0x03 */
#define VS_READ_COMMAND 0x03


#define SPI_MODE	0x0   /**< VS10xx register */
#define SPI_STATUS	0x1   /**< VS10xx register */
#define SPI_BASS	0x2   /**< VS10xx register */
#define SPI_CLOCKF	0x3   /**< VS10xx register */
#define SPI_DECODE_TIME	0x4   /**< VS10xx register */
#define SPI_AUDATA	0x5   /**< VS10xx register */
#define SPI_WRAM	0x6   /**< VS10xx register */
#define SPI_WRAMADDR	0x7   /**< VS10xx register */
#define SPI_HDAT0	0x8   /**< VS10xx register */
#define SPI_HDAT1	0x9   /**< VS10xx register */
#define SPI_AIADDR	0xa   /**< VS10xx register */
#define SPI_VOL		0xb   /**< VS10xx register */
#define SPI_AICTRL0	0xc   /**< VS10xx register */
#define SPI_AICTRL1	0xd   /**< VS10xx register */
#define SPI_AICTRL2	0xe   /**< VS10xx register */
#define SPI_AICTRL3	0xf   /**< VS10xx register */

#define SM_DIFF		0x01   /**< VS10xx register */
#define SM_JUMP		0x02   /**< VS10xx register */
#define SM_RESET	0x04   /**< VS10xx register */
#define SM_OUTOFWAV	0x08   /**< VS10xx register */
#define SM_PDOWN	0x10   /**< VS10xx register */
#define SM_TESTS	0x20   /**< VS10xx register */
#define SM_STREAM	0x40   /**< VS10xx register */
#define SM_PLUSV	0x80   /**< VS10xx register */
#define SM_DACT		0x100   /**< VS10xx register */
#define SM_SDIORD	0x200   /**< VS10xx register */
#define SM_SDISHARE	0x400   /**< VS10xx register */
#define SM_SDINEW	0x800   /**< VS10xx register */
#define SM_ADPCM        0x1000   /**< VS10xx register */
#define SM_ADPCM_HP     0x2000   /**< VS10xx register */
class VS10XX : public newSPIClass
{
  public:
    void selectControlBus()   {digitalWrite(VS_XCS,LOW);}
    void deselectControlBus() {digitalWrite(VS_XCS,HIGH);}
    void selectDataBus()      {digitalWrite(VS_XDCS,LOW);}
    void deselectDataBus()    {digitalWrite(VS_XDCS,HIGH);}
    void putInReset()         {digitalWrite(VS_XRESET,LOW);}
    void releaseFromReset()   {digitalWrite(VS_XRESET,HIGH);}
    char readDREQ(){return digitalRead(VS_DREQ);}
    void init();
    void initIO();
    void writeRegister(unsigned char addressbyte,unsigned char highbyte,unsigned char lowbyte);
    void writeData(unsigned char *databuf,unsigned char = 32);//write 32 bytes to vs1053
    unsigned int readRegister(unsigned char addressbyte);/** Read the 16-bit value of a VS10xx register */
    void reset();//reset for vs10xx
    void softReset();/* Soft Reset of VS10xx (Between songs) */
    void setVolume(unsigned char leftchannel,unsigned char rightchannel);/* Set VS10xx Volume Register */
    void sendZerosToVS10xx();/** Send 2048 zeros. \todo Timeouts for all DREQ busy loop waits! */  
  private:
  
};
extern VS10XX vs1053;
#endif
