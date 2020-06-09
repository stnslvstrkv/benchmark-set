//+------------------------------------------------------------------+
//|                                           TestOpenbenchmarks.mq5 |
//|                        Copyright 2020, MetaQuotes Software Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#include "TestAckermann.mqh"
#include "TestArrays.mqh"
#include "TestBubbleSort.mqh"
#include "TestCall.mqh"
#include "TestFibo.mqh"
#include "TestMandelbrot.mqh"
#include "TestMath.mqh"
#include "TestMatrix.mqh"
#include "TestMoments.mqh"
#include "TestNestedLoops.mqh"
#include "TestPathTracer.mqh"
#include "TestPiDigits.mqh"
#include "TestRandom.mqh"
#include "TestRayTracer.mqh"
#include "TestSieve.mqh"
#include "TestString.mqh"
#include "TestStrPrep.mqh"
#include "TestStrRev.mqh"
#include "TestStrSum.mqh"


#define INT64  long
#define UINT64 ulong

enum EnBenchMarks
  {
   AllBenchmarks=0,
   Ackermann    =1,
   Arrays       =2,
   BubbleSort   =3,
   Call         =4,
   Fibo         =5,
   Mandelbrot   =6,
   Matrix       =7,
   Moments      =8,
   NestedLoops  =9,
   PathTracer   =10,
   PiDigits     =11,
   Random       =12,
   RayTracer    =13,
   Sieve        =14,
   SimpleMath   =15,
   String       =16,
   StringPrep   =17,
   StringReverse=18,
   StringSum    =19
  };
  
input EnBenchMarks InpBenchmark=AllBenchmarks;
//+------------------------------------------------------------------+
//| Benchmark output                                                 |
//+------------------------------------------------------------------+
void BenchOutput(string bench_name,const ulong elapsed,const long result)
  {
//--- write results
   string file_name="BenchResults\\"+bench_name+"MQ5.csv";
   int    file_handle=FileOpen(file_name,FILE_ANSI|FILE_CSV|FILE_COMMON|FILE_READ|FILE_WRITE|FILE_SHARE_READ|FILE_SHARE_WRITE,'\t');
   if(file_handle!=INVALID_HANDLE)
     {
      FileSeek(file_handle,0,SEEK_END);
      FileWrite(file_handle,TimeLocal(),elapsed);
      FileClose(file_handle);
     }
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool TestBenchmark(const int test_num,UINT64& time_test)
  {
   INT64  result_test;
   double result_double;
   bool   passed=true;
//---
   switch(test_num)
     {
      case 1 :
         time_test=TestAckermann(result_test);
         PrintFormat("   Ackermann:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestAckermann",time_test,result_test);
         break;
      case 2 :
         time_test=TestArrays(result_test);
         PrintFormat("      Arrays:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestArrays",time_test,result_test);
         passed=true;
         break;
      case 3 :
         time_test=TestBubbleSort(result_test);
         PrintFormat(" Bubble Sort:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestBubblesort",time_test,result_test);
         break;
      case 4 :
         time_test=TestCall(result_test);
         PrintFormat("        Call:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestCall",time_test,result_test);
         break;
      case 5 :
         time_test=TestFibo(result_test);
         PrintFormat("        Fibo:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestFibo",time_test,result_test);
         break;
      case 6 :
         time_test=TestMandelbrot(result_test);
         PrintFormat("  Mandelbrot:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestMandelbrot",time_test,result_test);
         break;
      case 7 :
         time_test=TestMatrix(result_test);
         PrintFormat("      Matrix:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestMatrix",time_test,result_test);
         break;
      case 8 :
         time_test=TestMoments(result_test);
         PrintFormat("     Moments:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestMoments",time_test,result_test);
         break;
      case 9 :
         time_test=TestNestedLoop(result_test);
         PrintFormat("Nested Loops:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestNestedLoops",time_test,result_test);
         break;
      case 10 :
         time_test=TestPathTracer(result_test);
         PrintFormat(" Path Tracer:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestPathTracer",time_test,result_test);
         break;
      case 11 :
         time_test=TestPiDigits(result_test);
         PrintFormat("   Pi Digits:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestPiDigits",time_test,result_test);
         break;
      case 12 :
         time_test=TestRandom(result_test);
         PrintFormat("      Random:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestRandom",time_test,result_test);
         break;
      case 13 :
         time_test=TestRayTracer(result_test);
         PrintFormat("  Ray Tracer:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestRayTracer",time_test,result_test);
         break;
      case 14 :
         time_test=TestSieve(result_test);
         PrintFormat("       Sieve:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestSieve",time_test,result_test);
         passed=true;
         break;
      case 15 :
         time_test=TestMath(result_test,result_double);
         PrintFormat(" Simple Math:  %I64u ms, %I64d %.0f",time_test,result_test,result_double);
         BenchOutput("TestMath",time_test,result_test);
         break;
      case 16 :
         time_test=TestString(result_test);
         PrintFormat("      String:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestString",time_test,result_test);
         break;
      case 17 :
         time_test=TestStrPrep(result_test);
         PrintFormat("    Str Prep:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestStringPrep",time_test,result_test);
         break;
      case 18 :
         time_test=TestStrRev(result_test);
         PrintFormat("     Str Rev:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestStringRev",time_test,result_test);
         break;
      case 19 :
         time_test=TestStrSum(result_test);
         PrintFormat("     Str Sum:  %I64u ms, %I64d",time_test,result_test);
         BenchOutput("TestStringSum",time_test,result_test);
         break;
      default :
         passed=false;
     }
//---
   return(passed);
  }
//+------------------------------------------------------------------+
//| Script program start function                                    |
//+------------------------------------------------------------------+
void OnStart()
  {
   UINT64 time_test;
   int    test_num=(int)InpBenchmark;
//---
   if(TestBenchmark(test_num,time_test))
     {
      int file_handle=FileOpen("result.txt",FILE_ANSI|FILE_WRITE|FILE_TXT);
      if(file_handle!=INVALID_HANDLE)
        {
         FileWrite(file_handle,"Elapsed ",time_test," ms");
         FileClose(file_handle);
        }
      TerminalClose(0);
     }
   else
     {
      Print("benchmark set start");
      for(test_num=1; test_num<=19; test_num++)
         TestBenchmark(test_num,time_test);
      Print("benchmark set finish");
     }
  }
//+------------------------------------------------------------------+
