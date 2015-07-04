#include <stdio.h>

#include "TestAPI.h"
#include "Test1.h"
#include "Test2.h"
#include <process.h>

void TExit(void *p)
{
   exit(0);
}

TestMenu_t MainMenu[]=
{
   AddTestMenu('1',&Test1)
   AddTestMenu('2',&Test2)
   AddTestFunc('z',TExit,"Exit")
};

DeclareTest(MainTest,"Main Test",MainMenu);



TestMenu_t Test1Menu[]=
{
   AddTestFunc('1',NULL,"Func1")
   AddTestFunc('2',NULL,"Func2")
   AddTestMenu('0',&Test2)
   AddTestFunc('m',NULL,"Func3")
   AddTestFunc('4',NULL,"Func4")
};
DeclareTest(Test1,"Test #1",Test1Menu);

