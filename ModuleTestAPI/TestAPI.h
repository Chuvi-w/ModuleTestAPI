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
typedef union ItemPointer_u
{
   pTestFunc Func;
   struct TestMenu_s *Menu;
}ItemPointer_t;

typedef struct MenuItem_s
{
   char Key;         //������
   ItemPointer_t Ptr;    //�������, ������� ������ ����������� ��� ������� �� ������ ��� ������ �� ������ ����.
   void *Params;     //���������, ������������ �������
   char *Description;//��������.
   TApi_BOOL IsMenu;//TRUE-�������, FALSE-�������.
}MenuItem_t;

typedef struct TestMenu_s
{
   char *Title;         //��������� ����.
   MenuItem_t *Items;
   int IncLevel; //��� ���������� ������������ ��� ������� �� ����� ������ ������, ���� ����� ��������� ���� �� �����.
   int NumItems;//���-�� �������
   struct TestMenu_s *PrevTest;
   TApi_BOOL SpewBaseItems;
}TestMenu_t;


#define MenuItemFunc(key, func, description) {key, func, TApi_NULL, description, TApi_FALSE},               //��������� ������� � ����  ��� ���������� (��������� 0)
#define MenuItemPFun(key, func, param, description) {key,func,(void*)param,description,TApi_FALSE}, //��������� ������� � ���� � ����������. (Fun ������ Func ����� ������� ������� ���������� � ����������)
#define MenuItemLink(key, menu) {key,(void*)menu,TApi_NULL,TApi_NULL,TApi_TRUE},      //������ �� ������ ����.


#define SetTestData(Title,Descript) {Title,Descript,TApi_NULL,sizeof(Descript)/sizeof(Descript[0]),TApi_NULL}
#define DeclareTest(Name,Title,Descript) TestData_t Name ={Title,Descript,TApi_NULL,sizeof(Descript)/sizeof(Descript[0]),TApi_NULL}

typedef struct  TApi_Data_s TApi_Data_t;
#ifdef __cplusplus
extern "C" 
{
#endif
   TApi_Data_t *TApi_Init(TestMenu_t *MainMenu, TestMenu_t *BaseMenu); //MainMenu - �������� �������, BaseMenu - ���������� � ������ �������.
   void PrintTestMap(TestMenu_t *Test, int IncLevel);
   void ParseTestKey(char Key);
#ifdef __cplusplus
}
#endif


#endif // TestAPI_h__
