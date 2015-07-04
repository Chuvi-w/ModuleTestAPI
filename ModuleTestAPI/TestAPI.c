#include "TestAPI.h"
static TestData_t *CurrentTest=(TestData_t*)TApi_NULL;
static TestData_t *MainTest=(TestData_t*)TApi_NULL;
extern TestData_t BaseTest;


const char *GetTestTitle(TestData_t *Test)
{
   if(!Test||!Test->Title)
   {
      return "No title";
   }
   
   return Test->Title;
}

void PrintTestTitle(TestData_t *Test)
{
   TApi_OutStr(GetTestTitle(Test));
}

TApi_BOOL IsMainTest(TestData_t *Test)
{
   if(!Test)
   {
      TApi_Error("Test is NULL\r\n");
      return TApi_FALSE;
   }

   if(!MainTest)
   {
      TApi_Error("Main test not set\r\n");
      return TApi_FALSE;
   }
   return (TApi_BOOL)(Test==MainTest);
}



TApi_BOOL PrintTestMenu(TestData_t *Test)
{
   int i=0;
   if(!Test)
   {
      TApi_Error("Test is NULL\r\n");
      return TApi_FALSE;
   }
   TApi_OutStr("\r\n ***********************************************\r\n");
   for(i=0;i<Test->NumItems;i++)
   {
      TApi_OutStr("*         ");
      TApi_OutChar(Test->Menu[i].Key);
      TApi_OutStr(" -  ");
      if(Test->Menu[i].IsMenu)
      {
         TApi_OutStr(" [");PrintTestTitle((TestData_t*)Test->Menu[i].pNewTest);TApi_OutStr("]");
      }
      else
      {
         TApi_OutStr(Test->Menu[i].Description);
      }
      if(!Test->Menu[i].pNewTest)
      {
         TApi_OutStr(" (not implemented)");
      }
      TApi_OutStr("\r\n");
   }
   if(/*!IsMainTest(Test)*/TApi_TRUE)
   {
      for(i=0;i<BaseTest.NumItems;i++)
      {
         TApi_OutStr("*         ");
         TApi_OutChar(BaseTest.Menu[i].Key);
         TApi_OutStr(" -  ");
         
         if(BaseTest.Menu[i].IsMenu)
         {
            TApi_OutStr(" [");GetTestTitle((TestData_t*)Test->Menu[i].pNewTest);TApi_OutStr("]");
         }
         else
         {
            TApi_OutStr(BaseTest.Menu[i].Description);
         }
         if(!BaseTest.Menu[i].pNewTest)
         {
            TApi_OutStr(" (not implemented)");
         }
         TApi_OutStr("\r\n");
      }
   }
   TApi_OutStr("\r\n ***********************************************\r\n");   
   return TApi_TRUE;
}

TApi_BOOL SetActiveTest(TestData_t *Test)
{
   if(!Test)
   {
      TApi_Error("Test is NULL\r\n");
      return TApi_FALSE;
   }
   Test->PrevTest=CurrentTest;
   CurrentTest=Test;
   PrintTestMenu(CurrentTest);
   return TApi_TRUE;
}

TestData_t *GetPrevTest(TestData_t *Test)
{
   if(!Test)
   {
      TApi_Error("Test is NULL\r\n");
      return TApi_NULL;
   }
   return Test->PrevTest;
}


TApi_BOOL ValidateMenu(TestData_t *Test)
{
   int i=0,j=0;
   TApi_BOOL TestValid=TApi_TRUE;
   if(!Test)
   {
      TApi_Error("Test is NULL\r\n");
      return TApi_FALSE;
   }
   if(Test->IncLevel>0)
   {
      return TApi_TRUE;
   }
   Test->IncLevel++;
   TApi_OutStr("Validating ");PrintTestTitle(Test);TApi_Endl();
   for(i=0;i<Test->NumItems;i++)
   {
      for(j=i+1;j<Test->NumItems;j++)
      {
         if(Test->Menu[i].Key==Test->Menu[j].Key)
         {
            TApi_Error("Key '");TApi_OutChar(Test->Menu[i].Key);TApi_OutStr("' used in \"");
            if(Test->Menu[i].IsMenu)
            {
               PrintTestTitle((TestData_t*)Test->Menu[i].pNewTest);
            }
            else
            {
               TApi_OutStr(Test->Menu[i].Description);
            }
            TApi_OutStr("\" [");TApi_OutDec(i);TApi_OutStr("] (");PrintTestTitle(Test);TApi_OutStr(") and in \"");
            if(Test->Menu[j].IsMenu)
            {
               PrintTestTitle((TestData_t*)Test->Menu[j].pNewTest);
            }
            else
            {
               TApi_OutStr(Test->Menu[j].Description);
            }
            TApi_OutStr("\" [");TApi_OutDec(j);TApi_OutStr("] (");PrintTestTitle(Test);TApi_OutStr(")\r\n");
            TestValid=TApi_FALSE;
         }
      }

      for(j=0;j<BaseTest.NumItems;j++)
      {
         if(Test->Menu[i].Key==BaseTest.Menu[j].Key)
         {
            TApi_Error("Key '");TApi_OutChar(Test->Menu[i].Key);TApi_OutStr("' used in \"");
            if(Test->Menu[i].IsMenu)
            {
               PrintTestTitle((TestData_t*)Test->Menu[i].pNewTest);
            }
            else
            {
               TApi_OutStr(Test->Menu[i].Description);
            }
            TApi_OutStr("\" [");TApi_OutDec(i);TApi_OutStr("] (");PrintTestTitle(Test);TApi_OutStr(") and in \"");
            if(BaseTest.Menu[j].IsMenu)
            {
               PrintTestTitle((TestData_t*)BaseTest.Menu[j].pNewTest);
            }
            else
            {
               TApi_OutStr(BaseTest.Menu[j].Description);
            }
            TApi_OutStr("\" [");TApi_OutDec(j);TApi_OutStr("] (");PrintTestTitle(&BaseTest);TApi_OutStr(")\r\n");
            TestValid=TApi_FALSE;
         }
      }
      if(Test->Menu[i].IsMenu)
      {
         TestValid&=ValidateMenu((TestData_t*)Test->Menu[i].pNewTest);
      }
   }
   Test->IncLevel--;
   return TestValid;
}

TApi_BOOL SetMainTest(TestData_t *Test)
{
   if(!Test)
   {
      TApi_Error("Test is NULL\r\n");
      return TApi_FALSE;
   }
   if(MainTest)
   {
      TApi_Warning("Main test already set! (");PrintTestTitle(MainTest);TApi_OutStr(")\r\n");
   }
   if(!Test->Title)
   {
      TApi_Warning("Main test has no title!\r\n");
   }
   MainTest=Test;
   ValidateMenu(Test);
   SetActiveTest(Test);
   return TApi_TRUE;
}


void PrintCharNumTimes(char c,int num)
{
   static int i=0;
   for(i=0;i<num;i++)
   {
      TApi_OutChar(c);
   }
}



void PrintTestMap(TestData_t *Test, int IncLevel)
{
   int i;
   if(!Test)
   {
      TApi_Error("Test is NULL\r\n");
      return;
   }
   if(Test->IncLevel>0)
   {
      //TApi_Warning("Test \"");PrintTestTitle(Test);TApi_OutStr("\" already described\r\n");
      return;
   }
   Test->IncLevel++;
   if(!IncLevel)
   {
      TApi_OutStr("\r\n**************");PrintTestTitle(Test);TApi_OutStr("**************\r\n");
   }
   for(i=0;i<Test->NumItems;i++)
   {
      TApi_OutStr("*         ");
      PrintCharNumTimes(' ',(IncLevel+1)*4);
      TApi_OutChar(Test->Menu[i].Key);
      TApi_OutStr(" -  ");
      
      if(Test->Menu[i].IsMenu)
      {
         TApi_OutStr(" [");PrintTestTitle((TestData_t*)Test->Menu[i].pNewTest);TApi_OutStr("]");
      }
      else
      {
         TApi_OutStr(Test->Menu[i].Description);
      }
      if(!Test->Menu[i].pNewTest)
      {
         TApi_OutStr(" (not implemented)");
      }
      TApi_OutStr("\r\n");
      if(Test->Menu[i].IsMenu)
      {
         PrintTestMap((TestData_t*)Test->Menu[i].pNewTest,IncLevel+1);
      }
   }
   if(/*!IsMainTest(Test)*/TApi_TRUE)
   {
      for(i=0;i<BaseTest.NumItems;i++)
      {
          
         TApi_OutStr("*         ");
         PrintCharNumTimes(' ',(IncLevel+1)*4);
         TApi_OutChar(BaseTest.Menu[i].Key);
         TApi_OutStr(" -  ");
         TApi_OutStr(BaseTest.Menu[i].Description);
         if(BaseTest.Menu[i].IsMenu)
         {
            TApi_OutStr(" [");GetTestTitle((TestData_t*)Test->Menu[i].pNewTest);TApi_OutStr("]");
         }
         if(!BaseTest.Menu[i].pNewTest)
         {
            TApi_OutStr(" (not implemented)");
         }
         TApi_OutStr("\r\n");
      }
   }
   Test->IncLevel--;
}


void ParseTestKey(char Key)
{
   int i=0;
   char TestKey;
   TApi_BOOL bKeyFound=TApi_FALSE;
   char *Description;
   TApi_BOOL IsMenu=TApi_FALSE;
   if(!CurrentTest)
   {
      TApi_Warning("Test not set!\r\n");
   }

   if (Key >= 'a' && Key <= 'z')
   {
				Key -= ('a' - 'A');
   }

   for(i=0;i<CurrentTest->NumItems;i++)
   {
      TestKey=CurrentTest->Menu[i].Key;
      if (TestKey >= 'a' && TestKey <= 'z')
      {
			      TestKey -= ('a' - 'A');
      }

      if(TestKey==Key)
      {
         bKeyFound=TApi_TRUE;
         if(!CurrentTest->Menu[i].pNewTest)
         {
            TApi_OutStr("Not implemented!\r\n");
            break;
         }
         IsMenu=CurrentTest->Menu[i].IsMenu;
         
         if(IsMenu)
         {
            TApi_OutStr("\r\nTest switched to "); PrintTestTitle((TestData_t*)CurrentTest->Menu[i].pNewTest); TApi_OutStr("\r\n");
            SetActiveTest((TestData_t*)CurrentTest->Menu[i].pNewTest);
         }
         else
         {
            Description=CurrentTest->Menu[i].Description;
            ((pTestFunc)CurrentTest->Menu[i].pNewTest)(CurrentTest->Menu[i].Params);
            TApi_OutStr("\r\n _____\""); TApi_OutStr(Description); TApi_OutStr("\" end______\r\n");
         }
         break;
      }
   }
      if(/*!IsMainTest(Test)*/TApi_TRUE)
   {
      for(i=0;i<BaseTest.NumItems;i++)
      {
         TestKey=BaseTest.Menu[i].Key;
         if (TestKey >= 'a' && TestKey <= 'z')
         {
			         TestKey -= ('a' - 'A');
         }

         if(TestKey==Key)
         {
            bKeyFound=TApi_TRUE;
            if(!BaseTest.Menu[i].pNewTest)
            {
               TApi_OutStr("Not implemented!\r\n");
               break;
            }
            IsMenu=BaseTest.Menu[i].IsMenu;
         
            if(IsMenu)
            {
               TApi_OutStr("\r\nTest switched to "); PrintTestTitle((TestData_t*)BaseTest.Menu[i].pNewTest); TApi_OutStr("\r\n");
               SetActiveTest((TestData_t*)BaseTest.Menu[i].pNewTest);
            }
            else
            {
               Description=BaseTest.Menu[i].Description;
               ((pTestFunc)BaseTest.Menu[i].pNewTest)(BaseTest.Menu[i].Params);
               TApi_OutStr("\r\n _____\""); TApi_OutStr(Description); TApi_OutStr("\" end______\r\n");
            }
            break;
         }
      }
   }
   if(!IsMenu)
   {
      PrintTestMenu(CurrentTest);
   }
}

void TGoToMain(void *p)
{
   SetActiveTest(MainTest);
}

void TGoToPrev(void *p)
{
   if(!CurrentTest)
   {
      TApi_Error("Curren test is NULL!\r\n");
      return;
   }
   if(!CurrentTest->PrevTest)
   {
      TApi_Warning("Prev test is NULL!\r\n");
      return;
   }
   /*Пользоваться SetActiveTest тут нельзя, она сделает текущий тест предыдущим, в итоге всё зациклится*/
   CurrentTest=CurrentTest->PrevTest;
   PrintTestMenu(CurrentTest);
}

void TPrintCurMap(void *p)
{
   if(!CurrentTest)
   {
      TApi_Error("Curren test is NULL!\r\n");
      return;
   }
   PrintTestMap(CurrentTest,0);
}
TestMenu_t BaseTestMenu[]=
{
   AddTestFunc('m',TPrintCurMap,"Print map")
   AddTestFunc('p',TGoToPrev,"Prev menu")
   AddTestFunc('q',TGoToMain,"Main menu")

};
DeclareTest(BaseTest,"Base test menu",BaseTestMenu);
