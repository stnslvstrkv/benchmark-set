//+------------------------------------------------------------------+
//|                                                   TestStrSum.mqh |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#define MAX_SIZE_STR_SUM  50000000

//+------------------------------------------------------------------+
//| 19. Str Sum                                                      |
//+------------------------------------------------------------------+
string str1="111";
string str2="999";
string strn[10]={ "1000",
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
   //int sum=StringToInteger(str1)+StringToInteger(str2);
   int n1=num%10;
   int n2=n1+1;
   if(n2>=10)
      n2=0;
   int sum=(int)StringToInteger(strn[n1])+(int)StringToInteger(strn[n2]);
   return(sum+num);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
ulong TestStrSum(long &result)
  {
   ulong ticks=GetTickCount64();
   long   sum=0;
//---
   for(int i=0; i<MAX_SIZE_STR_SUM; i++)
      sum+=TestStrSumX(i);

   result=sum;
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
