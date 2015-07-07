
#include "TestAPI.h"

static TestMenu_t *CurrMenu;
static TestMenu_t *MainMenu;
static TestMenu_t *BaseMenu;
static TApi_BOOL SpewFinalMessage = TApi_TRUE;
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
      TApi_OutStr("- - - - - - - -\r\n");
      PrintMenuItems(BaseMenu);
   }
   TApi_OutStr("******************************\r\n");
   return TApi_TRUE;
}
   



TApi_BOOL SetActiveMenu(TestMenu_t *Menu)
{
   TApi_AssertNull(Menu, return TApi_FALSE);

   Menu->PrevMenu = CurrMenu;
   CurrMenu = Menu;
   PrintMenu(CurrMenu);
   return TApi_TRUE;
}

TestMenu_t *GetPrevTest(TestMenu_t *Menu)
{
   TApi_AssertNull(Menu, return TApi_NULL);
   return Menu->PrevMenu;
}

MenuItem_t *GetItem(TestMenu_t *Menu, int ItemNum)
{
   TApi_AssertNull(Menu, return TApi_NULL);
   TApi_AssertNull((ItemNum >= 0), return TApi_NULL);
   if (ItemNum < Menu->NumItems)
   {
      return &Menu->Items[ItemNum];
   }
   else
      if (Menu->SpewBaseItems&&BaseMenu&&ItemNum < (Menu->NumItems + BaseMenu->NumItems))
      {
         return &BaseMenu->Items[ItemNum - Menu->NumItems];
      }
      else
      {
         return NULL;
      }
}


char NormalizeKey(char Key)
{
   if (Key >= 'a' && Key <= 'z')
   {
      Key -= ('a' - 'A');
   }
   return Key;
}

const char *GetItemDescription(MenuItem_t *Item)
{
   TApi_AssertNull(Item, return TApi_NULL);
   if (Item->IsMenu)
   {
      return GetMenuTitle(Item->Ptr.Menu);
   }
   else
   {
      return Item->Description;
   }
   return TApi_NULL;
}

TApi_BOOL ValidateMenu(TestMenu_t *Menu)
{
   int i = 0, j = 0;
   MenuItem_t *CurItem, *TestItem;
   char CurKey, TestKey;
   TApi_BOOL TestValid = TApi_TRUE;
   TApi_AssertNull(Menu, return TApi_FALSE);
   if (Menu->IncLevel > 0)
   {
      return TApi_TRUE;
   }
   Menu->IncLevel++;
   TApi_OutStr("Validating "); PrintMenuTitle(Menu); TApi_Endl();
   do
   {
      CurItem = GetItem(Menu, i++);
      if (!CurItem)
      {
         break;
      }
      CurKey = NormalizeKey(CurItem->Key);
      j = i;
      do
      {
         TestItem = GetItem(Menu, j++);
         if (!TestItem)
         {
            break;
         }
         TestKey = NormalizeKey(TestItem->Key);
         if (CurKey == TestKey)
         {
            TApi_Error("Key '"); TApi_OutChar(CurKey); TApi_OutStr("' used in \""); TApi_OutStr(GetItemDescription(CurItem));
            TApi_OutStr("\" and in \"");
            TApi_OutStr(GetItemDescription(TestItem));
            TApi_OutStr("\" [");PrintMenuTitle(Menu);TApi_OutStr("]\r\n");
            TestValid = TApi_FALSE;
         }
      } while (TestItem);
      if(CurItem->IsMenu)
      {
         ValidateMenu(CurItem->Ptr.Menu);
      }
   } while (CurItem);
   Menu->IncLevel--;
   return TestValid;
}




void PrintCharNumTimes(char c, int num)
{
   static int i = 0;
   for (i = 0; i < num; i++)
   {
      TApi_OutChar(c);
   }
}



void PrintMenuMap(TestMenu_t *Menu, int IncLevel)
{
   int i;
   MenuItem_t *Item;
   TApi_AssertNull(Menu,return);
   if (Menu->IncLevel>0)
   {
      return;
   }
   Menu->IncLevel++;
   if (!IncLevel)
   {
      TApi_OutStr("\r\n**************"); PrintMenuTitle(Menu); TApi_OutStr("**************\r\n");
   }
   i = 0;
   do
   {
      Item = GetItem(Menu, i++);
      if (!Item)
         break;
      TApi_OutStr("*         ");
      PrintCharNumTimes(' ', (IncLevel + 1) * 4);
      TApi_OutChar(Item->Key);
      TApi_OutStr(" -  ");

      if (Item->IsMenu)
      {
         TApi_OutStr(" ["); PrintMenuTitle(Item->Ptr.Menu); TApi_OutStr("]");
      }
      else
      {
         TApi_OutStr(Item->Description);
      }
      if (!Item->Ptr.Func)//если Func==0, то Menu тоже будет ==0, ибо union/
      {
         TApi_OutStr(" (not implemented)");
      }
      TApi_OutStr("\r\n");
      if (Item->IsMenu)
      {
         PrintMenuMap(Item->Ptr.Menu, IncLevel + 1);
      }
   } while (Item);
   Menu->IncLevel--;
}


void TApi_ParseTestKey(char Key)
{
   int i = 0;
   char TestKey;
   MenuItem_t *Item;
   TApi_BOOL bKeyFound = TApi_FALSE;
   char *Description;
   TApi_BOOL IsMenu = TApi_FALSE;
   if (!CurrMenu)
   {
      TApi_Warning("CurrMenu not set!\r\n");
   }

  
   Key = NormalizeKey(Key);
   do
   {
      Item = GetItem(CurrMenu, i++);
      if (!Item)
         break;

      TestKey = NormalizeKey(Item->Key);
      if (TestKey == Key)
      {
         bKeyFound = TApi_TRUE;
         if (!Item->Ptr.Func)
         {
            TApi_OutStr("Not implemented!\r\n");
            break;
         }
         if (Item->IsMenu)
         {
            TApi_OutStr("\r\nTest switched to "); PrintMenuTitle(Item->Ptr.Menu); TApi_OutStr("\r\n");
            SetActiveMenu(Item->Ptr.Menu);
            SpewFinalMessage = TApi_FALSE;
         }
         else
         {
            Description = Item->Description;
            SpewFinalMessage = TApi_TRUE;
            Item->Ptr.Func(Item->Params);
            if (SpewFinalMessage)
            {
               TApi_OutStr("\r\n _____\""); TApi_OutStr(Description); TApi_OutStr("\" end______\r\n");
            }
         }
         break;
      }
   } while (Item);

   if (SpewFinalMessage)
   {
      PrintMenu(CurrMenu);
   }
}




TApi_BOOL TApi_Init(TestMenu_t *Main, TestMenu_t *Base)
{
   TApi_BOOL IsValid;
   TApi_AssertNull(Main,return TApi_FALSE);
   if (MainMenu)
   {
      TApi_Warning("Main test already set! ("); PrintMenuTitle(MainMenu); TApi_OutStr(")\r\n");
   }
   if (!Main->Title)
   {
      TApi_Warning("Main test has no title!\r\n");
   }
   BaseMenu = Base;//Если задано - установится, если не задано - затрёт предыдущую установку.

   MainMenu = Main;
   IsValid=ValidateMenu(Main);
   SetActiveMenu(Main);
   return IsValid;
}

TApi_BOOL TApi_PrevMenu()
{
   TApi_AssertNull(CurrMenu,return TApi_FALSE);
   TApi_AssertNull(CurrMenu->PrevMenu,return TApi_FALSE);
   /*Пользоваться SetActiveTest тут нельзя, она сделает текущий тест предыдущим, в итоге всё зациклится*/
   CurrMenu = CurrMenu->PrevMenu;
   PrintMenu(CurrMenu);
   TApi_DisableEndMsg();
   return TApi_TRUE;
}

TApi_BOOL TApi_Reset()
{
   TApi_AssertNull(MainMenu,return TApi_FALSE);
   CurrMenu=MainMenu;
   CurrMenu->PrevMenu=CurrMenu;
   PrintMenu(CurrMenu);
   TApi_DisableEndMsg();
   return TApi_TRUE;
}

void TApi_DisableEndMsg()
{
   SpewFinalMessage=TApi_FALSE;
}


void TApi_PrintMap()
{
   TApi_AssertNull(CurrMenu,return);
   PrintMenuMap(CurrMenu, 0);
}

