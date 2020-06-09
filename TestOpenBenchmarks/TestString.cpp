//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>
#include <string>

#define MAX_SIZE_STRING   50000000
//+------------------------------------------------------------------+
//| 16. String                                                       |
//+------------------------------------------------------------------+
int TestString(std::string &str)
  {
   str+=">>>>>>>>>>>>>>>>>...>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
   str+="Nice ";
   str+="red ";
   str+="apple";
   str+="\n";
//---
   return(str.length());
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestString(INT64 &result)
  {
   std::string str;
   char        init_str[2]="0";
   UINT64      ticks=GetTickCount64();
//--- тест
   result=0;
   for(int i=0; i<MAX_SIZE_STRING; i++)
     {
      init_str[0]='0'+i%10;
      str=init_str;
      result+=TestString(str);
     }
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
