// ModuleTestAPI.cpp: определяет точку входа для консольного приложения.
//

#include "TestAPI.h"
#include "Test1.h"
#include <conio.h>

void PrintMap(void *p)
{
   TApi_PrintMap();
}

void GoToPrevMenu(void *p)
{
   TApi_PrevMenu();
}

void GoToMainMenu(void *p)
{
   TApi_Reset();
}

MenuItem_t BaseTestMenu[] =
{
   MenuItemFunc('m', PrintMap, "Print map")
   MenuItemFunc('p', GoToPrevMenu, "Prev menu")
   MenuItemFunc('q', GoToMainMenu, "Main menu")

};

DeclareMenu(BaseMenu, "Base test menu", BaseTestMenu,TApi_FALSE);


int main(int argc, char* argv[])
{
  // CallTest();
   TApi_Init(&MainMenu, &BaseMenu);
   printf("_____________________________\r\n");
//   PrintTestMap(&MainTest,NULL);
   do 
   {
      if(_kbhit())
      {
         TApi_ParseTestKey(_getch());
      }
   } while (1);
	return 0;
}

