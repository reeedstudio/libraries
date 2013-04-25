#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include "debug_lvc.h"
#include "hardwareserial.h"
// set baudrate
#define charPrint Serial.print

void DBG_SetBaudRate(unsigned long ulBaudRate)
{
  Serial.begin(ulBaudRate);
}

/*
 * Conver int to string based on radix (usually 2, 8, 10, and 16)
 */
 
 /* ftoa:   convert double n to string s with digits*/
 void reverse(char * s)
 {
   ;
 }
 char *_ftoa(double n, char *s, int digits)
 {
     memset(s , 0 , sizeof(s));
     int i = 0 , p;
     char sign = true;
     if( n < 0 ) 
     { 
         sign = false;
         n = -n;
     }
     long long t = (long long)n;
     n = n - t;
     if( t == 0)
     {
         s[i++] = '0';
     }
     else
     {
         while( t != 0 )
         {
             p = t % 10;
             t /= 10;
             s[i++] = p + '0';
         }
         if(!sign) s[i++] = '-';
         reverse(s);
     }
     if( fabs( n - 0.0 ) > 0.0000000000001)
     {
         s[i++] = '.';
         int cur_d = 0;
         while( cur_d < digits)
         {
             cur_d++;
             n *= 10.0;
             p = (long long)n;
             n = n - p;
             s[i++] = p + '0';
         }
     }
     s[i] = '\0';

 }

char *_itoa(long num, char *str, int radix) 
{
    char string[] = "0123456789abcdefghijklmnopqrstuvwxyz";

    char* ptr = str;
    int i;
    int j;
    do{
        *ptr++ = string[num % radix];
        num /= radix;
        if (num < radix)
        {
            if(num > 0)*ptr++ = string[num];
            *ptr = '\0';
            break;
        }
    }while (num);
    j = ptr - str - 1;
    for (i = 0; i < (ptr - str) / 2; i++)
    {
        int temp = str[i];
        str[i] = str[j];
        str[j--] = temp;
    }
    return str;
}


/*
 * A simple printf function. Only support the following format:
 * Code Format
 * %c character
 * %d signed integers
 * %i signed integers
 * %s a string of characters
 * %o octal
 * %x unsigned hexadecimal
 */

int DBG_PRINT( const char* format, ...)
{
    va_list arg;
    int done = 0;
    va_start (arg, format);
    //done = vfprintf (stdout, format, arg);

    while( *format != '\0')
    {
        char cnt  = 0;
        char flag = 0;
        
        if( *format == '%')
        {
            
            if((*(format+1) >= '0') && (*(format+1) <= '9'))
            {
              flag = 1;
              cnt = *(format+1);
              format++;
            }
            if( *(format+1) == 'c' )
            {
                char c = (char)va_arg(arg, int);
               charPrint(c);
            } else if( *(format+1) == 'd' || *(format+1) == 'i')
            {
                char store[20];
                int i = va_arg(arg, int);
                char* str = store;
                _itoa(i, store, 10);
                while( *str != '\0') 
               charPrint(*str++); 
            } else if( *(format+1) == 'l' && *(format+2) == 'd')
            {
				format++;
                char store[20];
                long i = va_arg(arg, long);
                char* str = store;
                _itoa(i, store, 10);
                while( *str != '\0') 
                charPrint(*str++); 
            }else if( *(format+1) == 'o')
            {
                char store[20];
                int i = va_arg(arg, int);
                char* str = store;
                _itoa(i, store, 8);
                while( *str != '\0') charPrint(*str++); 
            } else if( *(format+1) == 'x')
            {
                char store[20];
                int i = va_arg(arg, int);
                char* str = store;
                _itoa(i, store, 16);
                while( *str != '\0') charPrint(*str++); 
            } else if( *(format+1) == 's' )
            {
                char* str = va_arg(arg, char*);
                while( *str != '\0') charPrint(*str++); 
            } else if( *(format+1) == 'f' )
            {
                char store[20];
                double i = va_arg(arg, double);
                char* str = store;
                if(flag)
                {
                  _ftoa(i, store, cnt-'0');
                  flag = 0;
                }
                while( *str != '\0') 
               charPrint(*str++); 
            }
            format += 2;
          } else {
            charPrint(*format++);
          }
    }
    va_end (arg);
    return done;
}
//*************************End File***************************************
