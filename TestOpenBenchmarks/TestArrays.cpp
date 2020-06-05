//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>

#define MAX_SIZE_ARRAYS      32000
//+------------------------------------------------------------------+
//| 2. Arrays                                                        |
//+------------------------------------------------------------------+
UINT64 TestArrays(INT64 &result)
  {
   int    i,k;
   int   *x=new int[MAX_SIZE_ARRAYS];
   int   *y=new int[MAX_SIZE_ARRAYS];
   UINT64 ticks=GetTickCount64();
//---
   for(i=0; i<MAX_SIZE_ARRAYS; i++)
      x[i]=i+1;

   result=0;
   for(k=0; k<100000; k++)
      for(i=MAX_SIZE_ARRAYS-1; i>=0; i--)
        {
         y[i]=x[i]+k%10;
         result+=y[i];
        }

   ticks=GetTickCount64()-ticks;
//---
   delete[] x;
   delete[] y;

   return(ticks);
  }
//+------------------------------------------------------------------+
