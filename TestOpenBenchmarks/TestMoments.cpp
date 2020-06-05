//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>
#include <math.h>

#define MAX_SIZE_MOMENTS      5750

#define IM 139968
#define IA 3877
#define IC 29573

double nums[MAX_SIZE_MOMENTS];
//+------------------------------------------------------------------+
//| 7. Moments                                                       |
//+------------------------------------------------------------------+
double TestRandom(double max)
  {
   static long last=42;
//---
   last=(last*IA+IC)%IM;
   return(max*last/IM);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void TestMomentsX(INT64 &result)
  {
   int    i,mid=0;
   double sum=0.0;
   double mean=0.0;
   double average_deviation=0.0;
   double standard_deviation=0.0;
   double variance=0.0;
   double skew=0.0;
   double kurtosis=0.0;
   double median=0.0;
   double deviation=0.0;
//---
   mean=sum/MAX_SIZE_MOMENTS;
   for(i=0; i<MAX_SIZE_MOMENTS; i++)
     {
      deviation=nums[i]-mean;
      average_deviation+=fabs(deviation);
      variance         +=pow(deviation,2);
      skew             +=pow(deviation,3);
      kurtosis         +=pow(deviation,4);
     }

   average_deviation/=MAX_SIZE_MOMENTS;
   variance         /=(MAX_SIZE_MOMENTS-1);
   standard_deviation=sqrt(variance);

   if(variance)
     {
      skew/=(MAX_SIZE_MOMENTS*variance*standard_deviation);
      kurtosis=(kurtosis/(MAX_SIZE_MOMENTS*variance*variance))-3.0;
     }
//---
   mid   =(MAX_SIZE_MOMENTS/2);
   median=(MAX_SIZE_MOMENTS%2) ? nums[mid]:(nums[mid]+nums[mid-1])/2;
//---
   result+=INT64(average_deviation+standard_deviation+skew+kurtosis+mid+median);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestMoments(INT64 &result)
  {
   int   i;
//--- тест
   for(i=0; i<MAX_SIZE_MOMENTS; i++)
      nums[i]=TestRandom(1000000.0);
//---   
   UINT64 ticks=GetTickCount64();

   result=0;
   for(i=0; i<MAX_SIZE_MOMENTS; i++)
      TestMomentsX(result);
//--- 
   return(GetTickCount64()-ticks);
  }
//+------------------------------------------------------------------+
