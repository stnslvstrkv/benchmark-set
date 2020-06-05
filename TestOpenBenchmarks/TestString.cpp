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
UINT64 TestString(std::string &str)
  {
   UINT64 ticks=GetTickCount64();
//--- тест
   for(int i=0; i<MAX_SIZE_STRING; i++)
     {
      str=">>>>>>>>>>>>>>>>>...>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
      str+="Nice ";
      str+="red ";
      str+="apple";
      str+="\n";
     }
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
