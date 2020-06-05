//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>

#define MAX_SIZE_SIEVE       22500
//+------------------------------------------------------------------+
//| 11. Sieve                                                        |
//+------------------------------------------------------------------+
void TestSieveX(INT64 &result)
  {
   static int flags[MAX_SIZE_SIEVE];
   int        i,k;
   int        count=0;
//---
   for(i=1; i<MAX_SIZE_SIEVE; i++)
      flags[i]=1;
//---
   for(i=1; i<MAX_SIZE_SIEVE; i++)
     {
      if(flags[i])
        {
         for(k=i+i; k<MAX_SIZE_SIEVE; k+=i)
            flags[k]=0;
         count++;
        }
     }
//---
   result+=count;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestSieve(INT64 &result)
  {
   UINT64 ticks=GetTickCount64();
//---
   result=0;
   for(int i=0; i<MAX_SIZE_SIEVE; i++)
      TestSieveX(result);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
