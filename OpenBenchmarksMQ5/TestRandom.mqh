//+------------------------------------------------------------------+
//|                                                   TestRandom.mqh |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#define MAX_SIZE_RANDOM      10000

#define IMR 139968
#define IAR 3877
#define ICR 29573
//+------------------------------------------------------------------+
//| 12. Random                                                       |
//+------------------------------------------------------------------+
double TestRandom2(double max)
  {
   static long last=42;
//---
   last=(last*IAR+ICR)%IMR;
   return(max*last/IMR);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
ulong TestRandom(long &result)
  {
   int   i,j;
   ulong ticks=GetTickCount64();
//---
   result=0;
   for(i=0; i<MAX_SIZE_RANDOM; i++)
      for(j=0; j<MAX_SIZE_RANDOM; j++)
         result+=(long)TestRandom2(i+j);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
