//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>
#include <string>

#define MAX_SIZE_STR_PREP 50000000
//+------------------------------------------------------------------+
//| 13. Str Prep                                                     |
//+------------------------------------------------------------------+
int StrPrep(std::string &str)
  {
   int  i,c,nl,nw,nc;
   bool space;
//---
   space=true;
   nl=nw=nc=0;
//---
   nc+=strlen(str.c_str());
   for(i=0; i<nc; i++)
     {
      c=str.at(i);
      if(c=='\n')
         ++nl;
      if(c==' ' || c=='\r' || c=='\n' || c=='\t' || c=='\"')
         space=true;
      else
        {
         if(space)
           {
            space=false;
            ++nw;
           }
        }
     }
//---
   return(nl+nw);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestStrPrep(INT64 &result)
  {
   UINT64      ticks=GetTickCount64();
   std::string str="Print(\"Nice red apple \",res,\" ms.\");";
//--- тест
   result=0;
   for(int i=0;i<MAX_SIZE_STR_PREP;i++)
      result+=StrPrep(str);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
