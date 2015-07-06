#include "TestAPI.h"

static TestMenu_t *CurrMenu;
static TestMenu_t *MainMenu;
static TestMenu_t *BaseMenu;

#define TApi_AssertNull(Item,ret)\
if(!Item)\
{\
   TApi_Error(#Item);TApi_OutStr(" is NULL!\r\n");\
   ret;\
}

const char *GetMenuTitle(TestMenu_t *Menu)
{
   if (!Menu || !Menu->Title)
   {
      return "No title";
   }

   return Menu->Title;
}

void PrintMenuTitle(TestMenu_t *Menu)
{
   TApi_OutStr(GetMenuTitle(Menu));
}


TApi_BOOL PrintMenuItems(TestMenu_t *Menu)
{
   int i = 0;
   TApi_AssertNull(Menu, return TApi_FALSE);
   for (i = 0; i < Menu->NumItems; i++)
   {
      TApi_OutStr("*         '");
      TApi_OutChar(Menu->Items[i].Key);
      TApi_OutStr("' -  ");
      if (Menu->Items[i].IsMenu)
      {
         TApi_OutStr(" ["); PrintMenuTitle((TestMenu_t*)Menu->Items[i].Ptr.Func); TApi_OutStr("]");
      }
      else
      {
         TApi_OutStr(Menu->Items[i].Description);
      }
      if (!Menu->Items[i].Ptr.Func)
      {
         TApi_OutStr(" (not implemented)");
      }
      TApi_OutStr("\r\n");
   }
   return TApi_TRUE;
}

TApi_BOOL PrintMenu(TestMenu_t *Menu)
{
   TApi_AssertNull(Menu, return TApi_FALSE);
   TApi_OutStr("***************"); PrintMenuTitle(Menu); TApi_OutStr("***************\r\n");
   PrintMenuItems(Menu);
   if (BaseMenu&&Menu->SpewBaseItems)
   {
      TApi_OutStr("- - - - - - - -");
      PrintMenuItems(BaseMenu);
   }
   TApi_OutStr("******************************\r\n");
   return TApi_TRUE;
}
   
   


TApi_BOOL SetActiveMenu(TestMenu_t *Menu)
{
   TApi_AssertNull(Menu, return TApi_FALSE);

   Menu->PrevTest = CurrMenu;
   CurrMenu = Menu;
   PrintMenu(CurrMenu);
   return TApi_TRUE;
}

TestMenu_t *GetPrevTest(TestMenu_t *Menu)
{
   TApi_AssertNull(Menu, return TApi_FALSE);
   return Menu->PrevTest;
}


TApi_BOOL ValidateMenu(TestMenu_t *Test)
{
   int i = 0, j = 0;
   TApi_BOOL TestValid = TApi_TRUE;
   if (!Test)
   {
      TApi_Error("Test is NULL\r\n");
      return TApi_FALSE;
   }
   if (Test->IncLevel > 0)
   {
      return TApi_TRUE;
   }
   Test->IncLevel++;
   TApi_OutStr("Validating "); PrintMenuTitle(Test); TApi_Endl();
   for (i = 0; i < Test->NumItems; i++)
   {
      for (j = i + 1; j < Test->NumItems; j++)
      {
         if (Test->Items[i].Key == Test->Items[j].Key)
         {
            TApi_Error("Key '"); TApi_OutChar(Test->Items[i].Key); TApi_OutStr("' used in \"");
            if (Test->Items[i].IsMenu)
            {
               PrintMenuTitle((TestMenu_t*)Test->Items[i].pAction);
            }
            else
            {
               TApi_OutStr(Test->Items[i].Description);
            }
            TApi_OutStr("\" ["); TApi_OutDec(i); TApi_OutStr("] ("); PrintMenuTitle(Test); TApi_OutStr(") and in \"");
            if (Test->Items[j].IsMenu)
            {
               PrintMenuTitle((TestMenu_t*)Test->Items[j].pAction);
            }
            else
            {
               TApi_OutStr(Test->Items[j].Description);
            }
            TApi_OutStr("\" ["); TApi_OutDec(j); TApi_OutStr("] ("); PrintMenuTitle(Test); TApi_OutStr(")\r\n");
            TestValid = TApi_FALSE;
         }
      }

      for (j = 0; j < BaseMenu.NumItems; j++)
      {
         if (Test->Items[i].Key == BaseMenu.Items[j].Key)
         {
            TApi_Error("Key '"); TApi_OutChar(Test->Items[i].Key); TApi_OutStr("' used in \"");
            if (Test->Items[i].IsMenu)
            {
               PrintMenuTitle((TestMenu_t*)Test->Items[i].pAction);
            }
            else
            {
               TApi_OutStr(Test->Items[i].Description);
            }
            TApi_OutStr("\" ["); TApi_OutDec(i); TApi_OutStr("] ("); PrintMenuTitle(Test); TApi_OutStr(") and in \"");
            if (BaseMenu.Items[j].IsMenu)
            {
               PrintMenuTitle((TestMenu_t*)BaseMenu.Items[j].pAction);
            }
            else
            {
               TApi_OutStr(BaseMenu.Items[j].Description);
            }
            TApi_OutStr("\" ["); TApi_OutDec(j); TApi_OutStr("] ("); PrintMenuTitle(&BaseMenu); TApi_OutStr(")\r\n");
            TestValid = TApi_FALSE;
         }
      }
      if (Test->Items[i].IsMenu)
      {
         TestValid &= ValidateMenu((TestMenu_t*)Test->Items[i].pAction);
      }
   }
   Test->IncLevel--;
   return TestValid;
}

TApi_BOOL TApi_Init(TestMenu_t *MainMenu, TestMenu_t *BaseMenu)
{
   if (!MainMenu)
   {
      TApi_Error("Test is NULL\r\n");
      return TApi_FALSE;
   }
   if (MainMenu)
   {
      TApi_Warning("Main test already set! ("); PrintMenuTitle(MainMenu); TApi_OutStr(")\r\n");
   }
   if (!MainMenu->Title)
   {
      TApi_Warning("Main test has no title!\r\n");
   }
   MainMenu = MainMenu;
   ValidateMenu(MainMenu);
   SetActiveTest(MainMenu);
   return TApi_TRUE;
}


void PrintCharNumTimes(char c, int num)
{
   static int i = 0;
   for (i = 0; i < num; i++)
   {
      TApi_OutChar(c);
   }
}



void PrintTestMap(TestMenu_t *Test, int IncLevel)
{
   int i;
   if (!Test)
   {
      TApi_Error("Test is NULL\r\n");
      return;
   }
   if (Test->IncLevel>0)
   {
      //TApi_Warning("Test \"");PrintTestTitle(Test);TApi_OutStr("\" already described\r\n");
      return;
   }
   Test->IncLevel++;
   if (!IncLevel)
   {
      TApi_OutStr("\r\n**************"); PrintMenuTitle(Test); TApi_OutStr("**************\r\n");
   }
   for (i = 0; i < Test->NumItems; i++)
   {
      TApi_OutStr("*         ");
      PrintCharNumTimes(' ', (IncLevel + 1) * 4);
      TApi_OutChar(Test->Items[i].Key);
      TApi_OutStr(" -  ");

      if (Test->Items[i].IsMenu)
      {
         TApi_OutStr(" ["); PrintMenuTitle((TestMenu_t*)Test->Items[i].pAction); TApi_OutStr("]");
      }
      else
      {
         TApi_OutStr(Test->Items[i].Description);
      }
      if (!Test->Items[i].pAction)
      {
         TApi_OutStr(" (not implemented)");
      }
      TApi_OutStr("\r\n");
      if (Test->Items[i].IsMenu)
      {
         PrintTestMap((TestMenu_t*)Test->Items[i].pAction, IncLevel + 1);
      }
   }
   if (/*!IsMainTest(Test)*/TApi_TRUE)
   {
      for (i = 0; i < BaseMenu.NumItems; i++)
      {

         TApi_OutStr("*         ");
         PrintCharNumTimes(' ', (IncLevel + 1) * 4);
         TApi_OutChar(BaseMenu.Items[i].Key);
         TApi_OutStr(" -  ");
         TApi_OutStr(BaseMenu.Items[i].Description);
         if (BaseMenu.Items[i].IsMenu)
         {
            TApi_OutStr(" ["); GetMenuTitle((TestMenu_t*)Test->Items[i].pAction); TApi_OutStr("]");
         }
         if (!BaseMenu.Items[i].pAction)
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
   int i = 0;
   char TestKey;
   TApi_BOOL bKeyFound = TApi_FALSE;
   char *Description;
   TApi_BOOL IsMenu = TApi_FALSE;
   if (!CurrMenu)
   {
      TApi_Warning("Test not set!\r\n");
   }

   if (Key >= 'a' && Key <= 'z')
   {
      Key -= ('a' - 'A');
   }

   for (i = 0; i < CurrMenu->NumItems; i++)
   {
      TestKey = CurrMenu->Items[i].Key;
      if (TestKey >= 'a' && TestKey <= 'z')
      {
         TestKey -= ('a' - 'A');
      }

      if (TestKey == Key)
      {
         bKeyFound = TApi_TRUE;
         if (!CurrMenu->Items[i].pAction)
         {
            TApi_OutStr("Not implemented!\r\n");
            break;
         }
         IsMenu = CurrMenu->Items[i].IsMenu;

         if (IsMenu)
         {
            TApi_OutStr("\r\nTest switched to "); PrintMenuTitle((TestMenu_t*)CurrMenu->Items[i].pAction); TApi_OutStr("\r\n");
            SetActiveTest((TestMenu_t*)CurrMenu->Items[i].pAction);
         }
         else
         {
            Description = CurrMenu->Items[i].Description;
            ((pTestFunc)CurrMenu->Items[i].pAction)(CurrMenu->Items[i].Params);
            TApi_OutStr("\r\n _____\""); TApi_OutStr(Description); TApi_OutStr("\" end______\r\n");
         }
         break;
      }
   }
   if (/*!IsMainTest(Test)*/TApi_TRUE)
   {
      for (i = 0; i < BaseMenu.NumItems; i++)
      {
         TestKey = BaseMenu.Items[i].Key;
         if (TestKey >= 'a' && TestKey <= 'z')
         {
            TestKey -= ('a' - 'A');
         }

         if (TestKey == Key)
         {
            bKeyFound = TApi_TRUE;
            if (!BaseMenu.Items[i].pAction)
            {
               TApi_OutStr("Not implemented!\r\n");
               break;
            }
            IsMenu = BaseMenu.Items[i].IsMenu;

            if (IsMenu)
            {
               TApi_OutStr("\r\nTest switched to "); PrintMenuTitle((TestMenu_t*)BaseMenu.Items[i].pAction); TApi_OutStr("\r\n");
               SetActiveTest((TestMenu_t*)BaseMenu.Items[i].pAction);
            }
            else
            {
               Description = BaseMenu.Items[i].Description;
               ((pTestFunc)BaseMenu.Items[i].pAction)(BaseMenu.Items[i].Params);
               TApi_OutStr("\r\n _____\""); TApi_OutStr(Description); TApi_OutStr("\" end______\r\n");
            }
            break;
         }
      }
   }
   if (!IsMenu)
   {
      PrintMenu(CurrMenu);
   }
}

void TGoToMain(void *p)
{
   SetActiveTest(MainMenu);
}

void TGoToPrev(void *p)
{
   if (!CurrMenu)
   {
      TApi_Error("Curren test is NULL!\r\n");
      return;
   }
   if (!CurrMenu->PrevTest)
   {
      TApi_Warning("Prev test is NULL!\r\n");
      return;
   }
   /*Пользоваться SetActiveTest тут нельзя, она сделает текущий тест предыдущим, в итоге всё зациклится*/
   CurrMenu = CurrMenu->PrevTest;
   PrintMenu(CurrMenu);
}

void TPrintCurMap(void *p)
{
   if (!CurrMenu)
   {
      TApi_Error("Curren test is NULL!\r\n");
      return;
   }
   PrintTestMap(CurrMenu, 0);
}
MenuItem_t BaseTestMenu[] =
{
   MenuItemFunc('m', TPrintCurMap, "Print map")
   MenuItemFunc('p', TGoToPrev, "Prev menu")
   MenuItemFunc('q', TGoToMain, "Main menu")

};
DeclareTest(BaseMenu, "Base test menu", BaseTestMenu);
