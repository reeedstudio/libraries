/*
  Queue.cpp
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee

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

#include "Queue.h"

#if __Debug
#include <Arduino.h>
#endif

/*********************************************************************************************************
** Function name: init
** Descriptions:  set queue to null
*********************************************************************************************************/
void QueueLvc::init()
{
    ucHead = 0;
    count  = 0;
}

/*********************************************************************************************************
** Function name: getLength
** Descriptions:  get length of queue
*********************************************************************************************************/
INT8U QueueLvc::getLength()
{
    return count;
}

/*********************************************************************************************************
** Function name: getAt
** Descriptions:  get the specified element
*********************************************************************************************************/
INT8U QueueLvc::getAt(INT8U index)
{
    if(index < count)
    {
        return ucArray[ucHead + index];
    }
    else 
    {
        return 0;
    }
}

/*********************************************************************************************************
** Function name: getHead
** Descriptions:  get head of queue
*********************************************************************************************************/
INT8U QueueLvc::getHead()
{
    return ucHead;
}

/*********************************************************************************************************
** Function name: push
** Descriptions:  push x to queue
*********************************************************************************************************/
bool QueueLvc::push(INT8U x)
{
    if(count < MAXSIZE)
	{
		ucArray[(ucHead + count)%MAXSIZE] = x;
		count++;
		return 1;
	}
	else
	{
#if __Debug

        Serial.println("Queue is full£¡");
#endif        
		return 0;
	}
}

/*********************************************************************************************************
** Function name: pop
** Descriptions:  return 
*********************************************************************************************************/
INT8U QueueLvc::pop()
{
	INT8U temp;
	if(count <= 0)
    {
        return 0;
	}
    temp = ucArray[ucHead++];
	ucHead %= MAXSIZE;
	count--;
	return temp;
}

#if __Debug
/*********************************************************************************************************
** Function name: dispHeadCnt
** Descriptions:  display head and count 
*********************************************************************************************************/
void QueueLvc::dispHeadCnt()
{
    Serial.print("head = ");
    Serial.print(ucHead);
    Serial.print("\tcount = ");
    Serial.print(count);
    Serial.println();
}

/*********************************************************************************************************
** Function name: dispQueue
** Descriptions:  display all queue 
*********************************************************************************************************/
void QueueLvc::dispQueue()
{
    for(int i = 0; i<MAXSIZE; i++)
    {
        Serial.print(ucArray[i]);
        Serial.print("\t");
        if((i+1)%5 == 0)
        {
            Serial.println();
        }
    }
    Serial.println();
}
#endif

//QueueLvc Queue;

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
