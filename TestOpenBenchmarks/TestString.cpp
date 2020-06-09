//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>
#include <string>

#define MAX_SIZE_STRING   20000000
//+------------------------------------------------------------------+
//| 12. String                                                       |
//+------------------------------------------------------------------+
void TestString(std::string &str)
  {
   for(int i=0; i<MAX_SIZE_STRING; i++)
     {
      str=">>>>>>>>>>>>>>>>>...>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
      str+="Nice ";
      str+="red ";
      str+="apple";
      str+="\n";
     }
  }
//+------------------------------------------------------------------+
//| 12. String                                                       |
//+------------------------------------------------------------------+
UINT64 TestString(INT64 &result)
  {
   std::string str;
   UINT64 ticks=GetTickCount64();
//--- тест
   TestString(str);
   result=(INT64)str.length();
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
