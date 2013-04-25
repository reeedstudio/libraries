/*
  Queue.h
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

#ifndef _QUEUELVC_H_
#define _QUEUELVC_H_

#define MAXSIZE     100                 // the max size of queue
#define __Debug     1                   // debug mode

#ifndef INT8U
#define INT8U unsigned char
#endif

class QueueLvc{

    private:
    INT8U ucArray[MAXSIZE];             // storage queue
    INT8U ucHead;                       // head of queue
    INT8U count;                        // count of queue
    
    public:
    void init();                        // init queue
    INT8U getLength();                  // get length of queue
    INT8U getAt(INT8U index);           // get the specified element
    INT8U getHead();                    // get head of queue
    bool push(INT8U x);                 // push data to queue
    INT8U pop();                        // pop data from queue
#if __Debug
    void dispHeadCnt();                 // debug only, print head and count
    void dispQueue();                   // debug only, print data of queue
#endif

};

//extern QueueLvc Queue;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
