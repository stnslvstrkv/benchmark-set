//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>
#include <string>

#define MAX_SIZE_STR_SUM  50000000
//+------------------------------------------------------------------+
//| 19. Str Sum                                                      |
//+------------------------------------------------------------------+
std::string str1="111";
std::string str2="999";
std::string strn[10]={ "1000",
                       "2000",
                       "3000",
                       "4000",
                       "5000",
                       "6000",
                       "7000",
                       "8000",
                       "9000",
                       "9999" };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int TestStrSumX(int num)
  {
   //int sum=std::stoi(str1)+std::stoi(str2);
   int n1=num%10;
   int n2=n1+1;
   if(n2>=10)
      n2=0;
   int sum=std::stoi(strn[n1])+std::stoi(strn[n2]);
   return(sum+num);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestStrSum(INT64 &result)
  {
   UINT64 ticks=GetTickCount64();
//---
   result=0;
   for(int i=0; i<MAX_SIZE_STR_SUM; i++)
      result+=TestStrSumX(i);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
