#ifndef TestAPI_h__
#define TestAPI_h__

#ifdef _WIN32
#include <stdio.h>
#include <conio.h>
#define TApi_OutStr(str)    printf("%s",str)
#define TApi_OutDec(num)    printf("%i",num)
#define TApi_OutHex(num)    printf("%x",num)
#define TApi_OutChar(c)     _putch(c)
#else
#include "LibDRV\rs232.h"
#define TApi_OutStr(str)    RS232_puts((char*)str)
#define TApi_OutDec(num)    RS232_putnum(num,10)
#define TApi_OutHex(num)    RS232_putnum(num,16)
#define TApi_OutChar(c)     RS232_putc(c)  
#endif

#define TApi_Error(err) TApi_OutStr("TestApi ERROR (");TApi_OutStr(__FUNCTION__);TApi_OutStr(", "); TApi_OutDec(__LINE__);TApi_OutStr("): ");TApi_OutStr(err)
#define TApi_Warning(err) TApi_OutStr("TestApi WARNING (");TApi_OutStr(__FUNCTION__);TApi_OutStr(", "); TApi_OutDec(__LINE__);TApi_OutStr("): ");TApi_OutStr(err)
#define TApi_Endl() TApi_OutStr("\r\n")
#define TApi_NULL           0
typedef enum TApi_BOOL
{
   TApi_FALSE,
   TApi_TRUE
}TApi_BOOL;

typedef void (*pTestFunc)(void *Params);
typedef struct TestMenu_s
{
   char Key;
   void *pNewTest;
   void *Params;
   char *Description;
   TApi_BOOL IsMenu;
}TestMenu_t;

typedef struct TestData_s
{
   char *Title;
   TestMenu_t *Menu;
   int IncLevel; //Для исключения зацикливания при проходе по всему дереву тестов, если тесты ссылаются друг на друга.
   int NumItems;//Кол-во пунктов
   struct TestData_s *PrevTest;
}TestData_t;


#define AddTestFunc(key, func, description) {key, func, TApi_NULL, description, TApi_FALSE},               //Добавляет функцию в меню теста без параметров (передаётся 0)
#define AddTestPFun(key, func, param, description) {key,func,(void*)param,description,TApi_FALSE}, //Добавляет функцию в меню теста с параметром. (Fun вместо Func чтобы менюшки красиво смотрелись в исходниках)
#define AddTestMenu(key, menu) {key,(void*)menu,TApi_NULL,TApi_NULL,TApi_TRUE},      //Добавляет переход на новое меню.
#define TestMenuEnd {0,0,0,0}
#define SetTestData(Title,Descript) {Title,Descript,TApi_NULL,sizeof(Descript)/sizeof(Descript[0]),TApi_NULL}
#define DeclareTest(Name,Title,Descript) TestData_t Name ={Title,Descript,TApi_NULL,sizeof(Descript)/sizeof(Descript[0]),TApi_NULL}


#ifdef __cplusplus
extern "C" 
{
#endif
   TApi_BOOL SetMainTest(TestData_t *Test);
   void PrintTestMap(TestData_t *Test, int IncLevel);
   void ParseTestKey(char Key);
#ifdef __cplusplus
}
#endif


#endif // TestAPI_h__
