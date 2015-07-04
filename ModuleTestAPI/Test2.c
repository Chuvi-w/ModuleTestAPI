#include <stdio.h>

#include "TestAPI.h"
#include "Test1.h"
#include "Test2.h"


TestMenu_t Test2Menu[]=
{
   AddTestFunc('1',NULL,"Func2_1")
   AddTestFunc('2',NULL,"Func2_2")
   AddTestMenu('0',&Test1)
   AddTestFunc('3',NULL,"Func2_3")
   AddTestFunc('4',NULL,"Func2_4")
};
DeclareTest(Test2,"Test #2",Test2Menu);
