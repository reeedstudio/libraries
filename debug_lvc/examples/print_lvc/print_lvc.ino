#include <debug_lvc.h>


void setup()
{
  __SerialInit(115200);
}

void loop()
{
  char str[] = "hello world!";
  __printf("a = %d, b = %d\n", 10, 15);
  __printf("%s\n", str);
  delay(500);
  
}