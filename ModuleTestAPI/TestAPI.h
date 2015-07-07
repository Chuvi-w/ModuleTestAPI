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

typedef void(*pTestFunc)(void *Params);
typedef struct TestMenu_s TestMenu_t;
typedef union ItemPointer_u
{
   void *Common;
   pTestFunc Func;
   TestMenu_t *Menu;
}ItemPointer_t;

typedef struct MenuItem_s
{
   char Key;         //Кнопка
   ItemPointer_t Ptr;    //Функция, которая должна выполняться при нажатии на кнопку или ссылка на другое меню.
   void *Params;     //Параметры, передаваемые функции
   char *Description;//Описание.
   TApi_BOOL IsMenu;//TRUE-менюшка, FALSE-функция.
}MenuItem_t;

struct TestMenu_s
{
   char *Title;         //Заголовок меню.
   MenuItem_t *Items;
   int IncLevel; //Для исключения зацикливания при проходе по всему дереву тестов, если тесты ссылаются друг на друга.
   int NumItems;//Кол-во пунктов
   struct TestMenu_s *PrevMenu;
   TApi_BOOL SpewBaseItems;
};


#define MenuItemFunc(key, func, description) {key, func, TApi_NULL, description, TApi_FALSE},               //Добавляет функцию в меню  без параметров (передаётся 0)
#define MenuItemPFun(key, func, param, description) {key,func,(void*)param,description,TApi_FALSE}, //Добавляет функцию в меню с параметром. (Fun вместо Func чтобы менюшки красиво смотрелись в исходниках)
#define MenuItemLink(key, menu) {key,menu,TApi_NULL,TApi_NULL,TApi_TRUE},      //Ссылка на другое меню.


#define SetTestData(Title,Descript) {Title,Descript,TApi_NULL,sizeof(Descript)/sizeof(Descript[0]),TApi_NULL}
#define DeclareMenu(Name,Title,Descript,SpewBaseItems) TestMenu_t Name ={Title,Descript,TApi_NULL,sizeof(Descript)/sizeof(Descript[0]),TApi_NULL,SpewBaseItems}

typedef struct  TApi_Data_s TApi_Data_t;
#ifdef __cplusplus
extern "C" 
{
#endif
   void TApi_ParseTestKey(char Key);
   TApi_BOOL TApi_Init(TestMenu_t *Main, TestMenu_t *Base);
   TApi_BOOL TApi_PrevMenu();
   TApi_BOOL TApi_Reset();
   void TApi_DisableEndMsg();
   void TApi_PrintMap();
#ifdef __cplusplus
}
#endif


#endif // TestAPI_h__
