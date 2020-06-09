//+------------------------------------------------------------------+
//|                                                   TestStrRev.mqh |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#define MAX_SIZE_STR_REV  50000000

//+------------------------------------------------------------------+
//| 18. Str Rev                                                      |
//+------------------------------------------------------------------+
string strr="Hello, world!!!\n";
string rev=strr;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int TestStrRevX(void)
  {
   uint len=StringLen(strr);
   StringReserve(rev,len+1);

   for(uint i=0; i<len; i++)
      StringSetCharacter(rev,i,StringGetCharacter(strr,len-i-1));
//---
   return(StringGetCharacter(rev,len-1));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
ulong TestStrRev(long &result)
  {
   ulong ticks=GetTickCount64();
//---
   result=0;
   for(int i=0; i<MAX_SIZE_STR_REV; i++)
      result+=TestStrRevX();
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
