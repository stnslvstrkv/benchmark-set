//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>
#include <string>

#define MAX_SIZE_STR_REV  50000000
//+------------------------------------------------------------------+
//| 18. Str Rev                                                      |
//+------------------------------------------------------------------+
std::string strr="Hello, world!!!\n";
std::string rev(strr);
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int TestStrRevX(void)
  {
   size_t len=strr.length();
   rev.reserve(len+1);

   for(int i=0; i<len; i++)
      rev[i]=strr[len-i-1];
//---
   return(rev[len-1]);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestStrRev(INT64 &result)
  {
   UINT64 ticks=GetTickCount64();
//---
   result=0;
   for(int i=0; i<MAX_SIZE_STR_REV; i++)
      result+=TestStrRevX();
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
