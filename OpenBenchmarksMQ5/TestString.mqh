//+------------------------------------------------------------------+
//|                                                   TestString.mqh |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#define MAX_SIZE_STRING   50000000

//+------------------------------------------------------------------+
//| 16. String                                                       |
//+------------------------------------------------------------------+
int TestString(string &str)
  {
   str+=">>>>>>>>>>>>>>>>>...>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
   str+="Nice ";
   str+="red ";
   str+="apple";
   str+="\n";
//---
   return(StringLen(str));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
ulong TestString(long &result)
  {
   string str;
   string init_str="0";
   ulong  ticks=GetTickCount64();
//--- тест
   result=0;
   for(int i=0; i<MAX_SIZE_STRING; i++)
     {
      StringSetCharacter(init_str,0,'0'+i%10);
      str=init_str;
      result+=TestString(str);
     }
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
