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
void BenchOutput(LPCSTR bench_name,const UINT64 elapsed,INT64 *presult)
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
     {
      _fprintf_p(out,"YYYY.MM.DD HH:MI:SS\tELAPSED");
      if(presult)
         _fprintf_p(out,"\tVALUE\n");
      else
         _fprintf_p(out,"\n");
     }
   _fprintf_p(out,"%d.%02d.%02d %02d:%02d:%02d\t%I64u",
                   time_str.tm_year+1900,time_str.tm_mon,time_str.tm_mday,time_str.tm_hour,time_str.tm_min,time_str.tm_sec,
                   elapsed);
   if(presult)
      _fprintf_p(out,"\t%I64d\n",*presult);
   else
      _fprintf_p(out,"\n");
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
//| Global tests                                                     |
//+------------------------------------------------------------------+
int main(int argc, char **argv)
  {
   int    test_num=0;
   INT64  result_test;
   UINT64 time_test;
//---
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
//--- 1. Ackermann
   if(test_num<=0 || test_num>19 || test_num==1)
     {
      time_test=TestAckermann(result_test);
      if(test_num==1)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("   Ackermann:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestAckermann",time_test,&result_test);
     }
//--- 2. Arrays
   if(test_num<=0 || test_num>19 || test_num==2)
     {
      time_test=TestArrays(result_test);
      if(test_num==2)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("      Arrays:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestArrays",time_test,&result_test);
     }
//--- 3. Bubble sort
   if(test_num<=0 || test_num>19 || test_num==3)
     {
      int    size=_countof(ExtBubbleArray);
      time_test=TestBubbleSort(ExtBubbleArray,0,size-1);
      result_test=ExtBubbleArray[size-1];
      if(test_num==3)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf(" Bubble Sort:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestBubblesort",time_test,&result_test);
     }
//--- 4. Call
   if(test_num<=0 || test_num>19 || test_num==4)
     {
      time_test=TestCall(result_test);
      if(test_num==4)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("        Call:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestCall",time_test,&result_test);
     }
//--- 5. Fibo
   if(test_num<=0 || test_num>19 || test_num==5)
     {
      time_test=TestFibo(result_test);
      if(test_num==5)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("        Fibo:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestFibo",time_test,&result_test);
     }
//--- 6. Mandelbrot
   if(test_num<=0 || test_num>19 || test_num==6)
     {
      time_test=TestMandelbrot(result_test);
      if(test_num==6)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("  Mandelbrot:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestMandelbrot",time_test,&result_test);
     }
//--- 7. Matrix
   if(test_num<=0 || test_num>19 || test_num==7)
     {
      time_test=TestMatrix(result_test);
      if(test_num==7)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("      Matrix:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestMatrix",time_test,&result_test);
     }
//--- 8. Moments
   if(test_num<=0 || test_num>19 || test_num==8)
     {
      time_test=TestMoments(result_test);
      if(test_num==8)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("     Moments:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestMoments",time_test,&result_test);
     }
//--- 9. Nested Loops
   if(test_num<=0 || test_num>19 || test_num==9)
     {
      time_test=TestNestedLoop(result_test);
      if(test_num==9)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("Nested Loops:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestNestedLoops",time_test,&result_test);
     }
//--- 10. Path Tracer
   if(test_num<=0 || test_num>19 || test_num==10)
     {
      time_test=TestPathTracer(result_test);
      if(test_num==10)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf(" Path Tracer:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestPathTracer",time_test,&result_test);
     }
//--- 11. PI
   if(test_num<=0 || test_num>19 || test_num==11)
     {
      time_test=TestPiDigits(result_test);
      if(test_num==11)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("   Pi Digits:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestPiDigits",time_test,&result_test);
     }
//--- 12. Random
   if(test_num<=0 || test_num>19 || test_num==12)
     {
      time_test=TestRandom(result_test);
      if(test_num==12)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("      Random:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestRandom",time_test,&result_test);
     }
//--- 13. Ray Tracer
   if(test_num<=0 || test_num>19 || test_num==13)
     {
      time_test=TestRayTracer(result_test);
      if(test_num==13)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("  Ray Tracer:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestRayTracer",time_test,&result_test);
     }
//--- 14. Sieve
   if(test_num<=0 || test_num>19 || test_num==14)
     {
      time_test=TestSieve(result_test);
      if(test_num==14)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("       Sieve:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestSieve",time_test,&result_test);
     }
//--- 15. Math
   if(test_num<=0 || test_num>19 || test_num==15)
     {
      INT64  result_int=0;
      double result_double=0.0;
      time_test=TestMath(result_int,result_double);
      if(test_num==15)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf(" Simple Math:  %I64u ms, %I64d %.0f\n",time_test,result_int,result_double);
      BenchOutput("TestMath",time_test,&result_int);
     }
//--- 16. String
   if(test_num<=0 || test_num>19 || test_num==16)
     {
      std::string str;
      time_test=TestString(str);
      if(test_num==16)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("      String:  %I64u ms, %d\n",time_test,(int)str.length());
      BenchOutput("TestString",time_test,NULL);
     }
//--- 17. Str Prep
   if(test_num<=0 || test_num>19 || test_num==17)
     {
      time_test=TestStrPrep(result_test);
      if(test_num==17)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("    Str Prep:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestStringPrep",time_test,&result_test);
     }
//--- 18. Str Rev
   if(test_num<=0 || test_num>19 || test_num==18)
     {
      time_test=TestStrRev(result_test);
      if(test_num==18)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("     Str Rev:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestStringRev",time_test,NULL);
     }
//--- 19. Str Sum
   if(test_num<=0 || test_num>19 || test_num==19)
     {
      result_test=0;
      time_test=TestStrSum(result_test);
      if(test_num==19)
         printf("Elapsed %I64u ms\n",time_test);
      else
         printf("     Str Sum:  %I64u ms, %I64d\n",time_test,result_test);
      BenchOutput("TestStringSum",time_test,&result_test);
     }
//---
   return(0);
  }
//+------------------------------------------------------------------+
