// ModuleTestAPI.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "TestAPI.h"
#include "Test1.h"
#include <conio.h>

int main(int argc, char* argv[])
{
  // CallTest();
   TApi_Init(&MainMenu, TODO);
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

