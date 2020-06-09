//+------------------------------------------------------------------+
//|                                                TestAckermann.mqh |
//|                        Copyright 2020, MetaQuotes Software Corp. |
//|                                              http://www.mql5.com |
//+------------------------------------------------------------------+
#define MAX_SIZE_ACKERMANN   12000

//+------------------------------------------------------------------+
//| 1. Ackermann                                                     |
//+------------------------------------------------------------------+
int Ack(int M,int N)
  {
//---
   if(M==0)
      return(N+1);

   if(N==0)
      return(Ack(M-1,1));
//---
   return Ack(M-1,Ack(M,(N-1)));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
ulong TestAckermann(long &result)
  {
   ulong ticks=GetTickCount64();
//---
   result=0;
   for(int i=0; i<MAX_SIZE_ACKERMANN; i++)
      result+=Ack(3,i%6+1);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
