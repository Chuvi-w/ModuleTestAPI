// ModuleTestAPI.cpp: определяет точку входа для консольного приложения.
//

#include "TestAPI.h"
#include "Test1.h"
#include <conio.h>

int main(int argc, char* argv[])
{
  // CallTest();
   SetMainTest(&MainTest);
   printf("_____________________________\r\n");
//   PrintTestMap(&MainTest,NULL);
   do 
   {
      if(_kbhit())
      {
         ParseTestKey(_getch());
      }
   } while (1);
	return 0;
}

