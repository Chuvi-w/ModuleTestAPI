#include <stdio.h>

#include "TestAPI.h"
#include "Test1.h"
#include "Test2.h"


MenuItem_t Test2Menu[]=
{
   MenuItemFunc('1',NULL,"Func2_1")
   MenuItemFunc('2',NULL,"Func2_2")
   MenuItemLink('0',&Test1)
   MenuItemFunc('3',NULL,"Func2_3")
   MenuItemFunc('4',NULL,"Func2_4")
};

DeclareMenu(Test2,"Test #2",Test2Menu,TApi_TRUE);
