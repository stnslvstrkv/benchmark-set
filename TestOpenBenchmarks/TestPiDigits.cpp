//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>
#include <string>

#define MAX_SIZE_PI          22000
//+------------------------------------------------------------------+
//| 9. PI                                                            |
//+------------------------------------------------------------------+
std::string PiStr;
int         PiA[(MAX_SIZE_PI/4+1)*14];
//+------------------------------------------------------------------+
//| Функция тестирования                                             |
//+------------------------------------------------------------------+
INT64 PiCalculate(const int digits)
  {
   int   d=0,e,b,g,r;
   int   c=(digits/4+1)*14;
   int   f=10000;
   INT64 result=0;
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
      result+=r;
      PiStr+=std::to_string(r);
     }
//---
   return(result);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestPiDigits(INT64 &result)
  {
   UINT64 ticks=GetTickCount64();
//--- тест
   result=PiCalculate(MAX_SIZE_PI);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
