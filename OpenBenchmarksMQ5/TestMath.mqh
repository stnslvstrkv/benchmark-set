//+------------------------------------------------------------------+
//|                                                     TestMath.mqh |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
//| Simple math test                                                 |
//+------------------------------------------------------------------+
ulong TestMath(long &result_int,double &result_double)
  {
   long   res_int=0;
   double res_double=0;
   ulong  ticks=GetTickCount64();
//--- тест
   for(int i=0; i<=100000000; i++)
     {
      res_int+=i*i;
      res_int++;
      res_double+=i*i;
      res_double++;
     }
//--- output results
   result_int   =res_int;
   result_double=res_double;
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
