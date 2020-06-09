//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>
#include <string>
#include <time.h>
#include <ShlObj.h>
#include "TestOpenBenchmarks.h"

bool ExtCommonFolder=true;
//+------------------------------------------------------------------+
//| Вывод бенчмарок                                                  |
//+------------------------------------------------------------------+
void BenchOutput(LPCSTR bench_name,const UINT64 elapsed,INT64 result)
  {
//--- write results
   char       path[260];
   __time64_t ctm;
   tm         time_str;
   FILE      *out;
   _time64(&ctm);
   _localtime64_s(&time_str,&ctm);
   strcpy_s(path,bench_name);
   strcat_s(path,".csv");
   fopen_s(&out,path,"at");
   fseek(out,0,SEEK_END);
   if(ftell(out)==0)
      _fprintf_p(out,"YYYY.MM.DD HH:MI:SS\tELAPSED\tVALUE\n");
   _fprintf_p(out,"%d.%02d.%02d %02d:%02d:%02d\t%I64u\t%I64d\n",
              time_str.tm_year+1900,time_str.tm_mon,time_str.tm_mday,time_str.tm_hour,time_str.tm_min,time_str.tm_sec,
              elapsed,result);
   fclose(out);
//--- write results to common terminal folder
   if(ExtCommonFolder)
     {
      ::SHGetFolderPathA(NULL,CSIDL_APPDATA,NULL,SHGFP_TYPE_CURRENT,path);
      strcat_s(path,"\\MetaQuotes\\Terminal\\Common\\Files\\BenchResults");
      if(::GetFileAttributesA(path)==INVALID_FILE_ATTRIBUTES)
         ::CreateDirectoryA(path,NULL);
      strcat_s(path,"\\");
      strcat_s(path,bench_name);
      strcat_s(path,"CPP.csv");
      if(fopen_s(&out,path,"at")==0)
        {
         _fprintf_p(out,"%d.%02d.%02d %02d:%02d:%02d\t%I64u\n",
                    time_str.tm_year+1900,time_str.tm_mon,time_str.tm_mday,time_str.tm_hour,time_str.tm_min,time_str.tm_sec,
                    elapsed);
         fclose(out);
        }
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
         printf("   Ackermann:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestAckermann",time_test,result_test);
         break;
      case 2 :
         time_test=TestArrays(result_test);
         printf("      Arrays:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestArrays",time_test,result_test);
         passed=true;
         break;
      case 3 :
         time_test=TestBubbleSort(result_test);
         printf(" Bubble Sort:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestBubblesort",time_test,result_test);
         break;
      case 4 :
         time_test=TestCall(result_test);
         printf("        Call:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestCall",time_test,result_test);
         break;
      case 5 :
         time_test=TestFibo(result_test);
         printf("        Fibo:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestFibo",time_test,result_test);
         break;
      case 6 :
         time_test=TestMandelbrot(result_test);
         printf("  Mandelbrot:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestMandelbrot",time_test,result_test);
         break;
      case 7 :
         time_test=TestMatrix(result_test);
         printf("      Matrix:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestMatrix",time_test,result_test);
         break;
      case 8 :
         time_test=TestMoments(result_test);
         printf("     Moments:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestMoments",time_test,result_test);
         break;
      case 9 :
         time_test=TestNestedLoop(result_test);
         printf("Nested Loops:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestNestedLoops",time_test,result_test);
         break;
      case 10 :
         time_test=TestPathTracer(result_test);
         printf(" Path Tracer:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestPathTracer",time_test,result_test);
         break;
      case 11 :
         time_test=TestPiDigits(result_test);
         printf("   Pi Digits:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestPiDigits",time_test,result_test);
         break;
      case 12 :
         time_test=TestRandom(result_test);
         printf("      Random:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestRandom",time_test,result_test);
         break;
      case 13 :
         time_test=TestRayTracer(result_test);
         printf("  Ray Tracer:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestRayTracer",time_test,result_test);
         break;
      case 14 :
         time_test=TestSieve(result_test);
         printf("       Sieve:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestSieve",time_test,result_test);
         passed=true;
         break;
      case 15 :
         time_test=TestMath(result_test,result_double);
         printf(" Simple Math:  %I64u ms, %I64d %.0f\n",time_test,result_test,result_double);
         BenchOutput("TestMath",time_test,result_test);
         break;
      case 16 :
         time_test=TestString(result_test);
         printf("      String:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestString",time_test,result_test);
         break;
      case 17 :
         time_test=TestStrPrep(result_test);
         printf("    Str Prep:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestStringPrep",time_test,result_test);
         break;
      case 18 :
         time_test=TestStrRev(result_test);
         printf("     Str Rev:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestStringRev",time_test,result_test);
         break;
      case 19 :
         time_test=TestStrSum(result_test);
         printf("     Str Sum:  %I64u ms, %I64d\n",time_test,result_test);
         BenchOutput("TestStringSum",time_test,result_test);
         break;
      default :
         passed=false;
     }
//---
   return(passed);
  }
//+------------------------------------------------------------------+
//| Global tests                                                     |
//+------------------------------------------------------------------+
int main(int argc, char **argv)
  {
   UINT64 time_test;
   int    test_num=0;
//--- collect arguments
   if(argc>1)
     {
      for(int i=1; i<argc; i++)
        {
         if(_stricmp(argv[i],"-no_common_folder")==0)
           {
            ExtCommonFolder=false;
            continue;
           }
         if(_strnicmp(argv[i],"-test:",6)==0)
           {
            test_num=atoi(argv[i]+6);
            continue;
           }
        }
     }
//---
   if(TestBenchmark(test_num,time_test))
      printf("Elapsed %I64u ms\n",time_test);
   else
      for(test_num=1; test_num<=19; test_num++)
         TestBenchmark(test_num,time_test);
//---
   return(0);
  }
//+------------------------------------------------------------------+
