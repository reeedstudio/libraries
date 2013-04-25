/*
  moto_4wd.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.
 
  Author: Loovee
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
*/
#ifndef __MOTORDRIVER_4WD_H__
#define __MOTORDRIVER_4WD_H__

#include "motordriver_4wd_dfs.h"

class motor_4wd
{
    private:
    
    unsigned char motorSpeed1;      // pwm: 0-100
    unsigned char motorSpeed2;      // pwm: 0-100
    
    unsigned char motorDir1;
    unsigned char motorDir2;
    
    unsigned char motorState1;               
    unsigned char motorState2;
    
    public:
    
    void init();
    
    void setRun1();
    void setRun2();
    
    void setStop1();
    void setStop2();
    
    void setSpeed1(int ispeed);       // pwm, 0-100
    void setSpeed2(int ispeed);       // pwm, 0-100
    
    void setDir1(unsigned char dir);
    void setDir2(unsigned char dir);
    
    void setSpeedDir(int ispeed, unsigned char dir);
    void setSpeedDir1(int ispeed, unsigned char dir);
    void setSpeedDir2(int ispeed, unsigned char dir);
    
};

extern motor_4wd MOTOR;
#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/


