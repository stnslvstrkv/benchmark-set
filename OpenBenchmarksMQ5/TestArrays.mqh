//+------------------------------------------------------------------+
//|                                                   TestArrays.mqh |
//|                        Copyright 2020, MetaQuotes Software Corp. |
//|                                              http://www.mql5.com |
//+------------------------------------------------------------------+
#define MAX_SIZE_ARRAYS      32000

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
ulong TestArrays(long& result)
  {
   int   i,k;
   int   x[];
   int   y[];
//--- dynamic arrays
   ArrayResize(x,MAX_SIZE_ARRAYS);
   ArrayResize(y,MAX_SIZE_ARRAYS);
//---
   ulong ticks=GetTickCount64();

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
   ArrayFree(x);
   ArrayFree(y);

   return(ticks);
  }
//+------------------------------------------------------------------+
