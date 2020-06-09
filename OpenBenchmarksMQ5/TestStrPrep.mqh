//+------------------------------------------------------------------+
//|                                                  TestStrPrep.mqh |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#define MAX_SIZE_STR_PREP 50000000

//+------------------------------------------------------------------+
//| 17. Str Prep                                                     |
//+------------------------------------------------------------------+
int TestStrPrep(string &str)
  {
   int  i,c,nl,nw,nc;
   bool space;
//---
   space=true;
   nl=nw=nc=0;
//---
   nc+=StringLen(str);
   for(i=0; i<nc; i++)
     {
      c=StringGetCharacter(str,i);
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
ulong TestStrPrep(long &result)
  {
   ulong      ticks=GetTickCount64();
   string str="Print(\"Nice red apple \",res,\" ms.\");";
//---
   result=0;
   for(int i=0;i<MAX_SIZE_STR_PREP;i++)
      result+=TestStrPrep(str);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
