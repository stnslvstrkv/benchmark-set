//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>

#define MAX_SIZE_CALL    500000000
//+------------------------------------------------------------------+
//| 4. Call                                                          |
//+------------------------------------------------------------------+
int TestCall(int n)
  {
   return(n%100);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestCall(INT64 &result)
  {
   UINT64 ticks=GetTickCount64();
//---
   result=0;
   for(int i=0; i<MAX_SIZE_CALL; i++)
      result+=TestCall(i);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+