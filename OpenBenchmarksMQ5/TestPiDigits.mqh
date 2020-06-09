//+------------------------------------------------------------------+
//|                                                 TestPiNumber.mqh |
//|                        Copyright 2020, MetaQuotes Software Corp. |
//|                                              http://www.mql5.com |
//+------------------------------------------------------------------+
#define MAX_SIZE_PI          22000
//---
string PiStr;
int    PiA[(MAX_SIZE_PI/4+1)*14];
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool PiCalculate(const int digits)
  {
   int d=0,e,b,g,r;
   int c=(digits/4+1)*14;
   int f=10000;
//---
   for(int i=0; i<c; i++)
      PiA[i]=20000000;
//---
   while((b=c-=14)>0)
     {
      d=e=d%f;
      while(--b>0)
        {
         d=d * b + PiA[b];
         g=(b << 1) - 1;
         PiA[b]=(d%g)*f;
         d/=g;
        }
      r=e+d/f;
      if(r<1000)
        {
         if(r>99)
            PiStr+="0";
         else
           {
            if(r > 9)
               PiStr+="00";
            else
               PiStr+="000";
           }
        }
      PiStr+=(string)r;
     }
//---
   return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
ulong TestPiDigits(long& result)
  {
   ulong ticks=GetTickCount64();
//---
   PiCalculate(MAX_SIZE_PI);
   result=StringLen(PiStr);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
