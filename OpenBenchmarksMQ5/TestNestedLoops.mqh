//+------------------------------------------------------------------+
//|                                                      Bench Marks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#define MAX_SIZE_NESTED_LOOPS   38

//+------------------------------------------------------------------+
//| 8. Nested Loops                                                  |
//+------------------------------------------------------------------+
ulong TestNestedLoop(long &result)
  {
   ulong ticks=GetTickCount64();
//---
   int   a,b,c,d,e,f;
   long x=0;

   for(a=0; a<MAX_SIZE_NESTED_LOOPS; a++)
      for(b=0; b<MAX_SIZE_NESTED_LOOPS; b++)
         for(c=0; c<MAX_SIZE_NESTED_LOOPS; c++)
            for(d=0; d<MAX_SIZE_NESTED_LOOPS; d++)
               for(e=0; e<MAX_SIZE_NESTED_LOOPS; e++)
                  for(f=0; f<MAX_SIZE_NESTED_LOOPS; f++)
                    {
                     x+=(a+b+c+d+e+f)>>4;
                    }
   result=x;
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
