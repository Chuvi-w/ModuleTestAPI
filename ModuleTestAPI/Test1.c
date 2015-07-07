#include <stdio.h>

#include "TestAPI.h"
#include "Test1.h"
#include "Test2.h"
#include <process.h>

void TExit(void *p)
{
   exit(0);
}

MenuItem_t MainMenuItems[]=
{
   MenuItemLink('1',&Test1)
   MenuItemLink('2',&Test2)
   MenuItemFunc('z',TExit,"Exit")
};

DeclareMenu(MainMenu,"Main Test",MainMenuItems,TApi_TRUE);



MenuItem_t Test1Menu[]=
{
   MenuItemFunc('1',NULL,"Func1")
   MenuItemFunc('2',NULL,"Func2")
   MenuItemLink('0',&Test2)
   MenuItemFunc('3',NULL,"Func3")
   MenuItemFunc('4',NULL,"Func4")
};
DeclareMenu(Test1,"Test #1",Test1Menu,TApi_TRUE);

