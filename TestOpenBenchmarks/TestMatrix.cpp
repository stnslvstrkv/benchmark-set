//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>

#define MAX_SIZE_MATRIX        168
//--- matrixes
int m1[MAX_SIZE_MATRIX][MAX_SIZE_MATRIX];
int m2[MAX_SIZE_MATRIX][MAX_SIZE_MATRIX];
int mm[MAX_SIZE_MATRIX][MAX_SIZE_MATRIX];
//+------------------------------------------------------------------+
//| 6. Matrix                                                        |
//+------------------------------------------------------------------+
void TestMatrixX(INT64 &result)
  {
   int i,j,k,val=0;
//--- перемножение матриц
   for(i=0; i<MAX_SIZE_MATRIX; i++)
     {
      for(j=0;j<MAX_SIZE_MATRIX;j++)
        {
         val=0;
         for(k=0; k<MAX_SIZE_MATRIX; k++)
           {
            val+=m1[i][k]*m2[k][j];
           }
         mm[i][j]=val;
        }
      result+=val;
     }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestMatrix(INT64 &result)
  {
   int    i,j,n=1;
   UINT64 ticks=GetTickCount64();
//--- тест
   for(i=0; i<MAX_SIZE_MATRIX; i++)
      for(j=0; j<MAX_SIZE_MATRIX; j++)
        {
         m1[i][j]=n;
         m2[i][j]=n;
         n++;
        }
   result=0;
   for(i=0; i<MAX_SIZE_MATRIX; i++)
      TestMatrixX(result);
//---
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
