#include <stdio.h>

#include "TestAPI.h"
#include "Test1.h"
#include "Test2.h"
#include <process.h>

void TExit(void *p)
{
   exit(0);
}

MenuItem_t MainMenu[]=
{
   MenuItemLink('1',&Test1)
   MenuItemLink('2',&Test2)
   MenuItemFunc('z',TExit,"Exit")
};

DeclareTest(MainMenu,"Main Test",MainMenu);



MenuItem_t Test1Menu[]=
{
   MenuItemFunc('1',NULL,"Func1")
   MenuItemFunc('2',NULL,"Func2")
   MenuItemLink('0',&Test2)
   MenuItemFunc('m',NULL,"Func3")
   MenuItemFunc('4',NULL,"Func4")
};
DeclareTest(Test1,"Test #1",Test1Menu);

