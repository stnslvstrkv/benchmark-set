//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>

#define MAX_SIZE_FIBO           40
//+------------------------------------------------------------------+
//| 5. Fibo                                                          |
//+------------------------------------------------------------------+
unsigned long fib(unsigned long n)
  {
   if(n<2)
      return(1);
   else
      return(fib(n-2)+fib(n-1));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestFibo(INT64 &result)
  {
   int           i;
   unsigned long f=0;
   UINT64        ticks=GetTickCount64();
//---
   result=0;
   for(i=0; i<MAX_SIZE_FIBO; i++)
     {
      f=fib(i);
      result+=f;
     }
//---
   return(GetTickCount64() - ticks);
  }
//+------------------------------------------------------------------+
