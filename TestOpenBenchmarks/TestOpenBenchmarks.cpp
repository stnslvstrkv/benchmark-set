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
   int test_num=0;
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
            //---
            FILE *out;
            fopen_s(&out,"TestOpenBenchmarks.txt","at");
            _fprintf_p(out,"%s -- %d\n",argv[i],test_num);
            fclose(out);
            continue;
           }
        }
     }
//--- 1. Ackermann
   if(test_num<=0 || test_num>16 || test_num==1)
     {
      INT64  result_ackerman;
      UINT64 time_ackermann=TestAckermann(result_ackerman);
      printf("   Ackermann:  %I64u ms, %I64d\n",time_ackermann,result_ackerman);
      BenchOutput("TestAckermann",time_ackermann,&result_ackerman);
     }
//--- 2. Arrays
   if(test_num<=0 || test_num>16 || test_num==2)
     {
      INT64  result_arrays;
      UINT64 time_arrays=TestArrays(result_arrays);
      printf("      Arrays:  %I64u ms, %I64d\n",time_arrays,result_arrays);
      BenchOutput("TestArrays",time_arrays,&result_arrays);
     }
//--- 3. Bubble sort
   if(test_num<=0 || test_num>16 || test_num==3)
     {
      int    size=_countof(ExtBubbleArray);
      UINT64 time_bubble=TestBubbleSort(ExtBubbleArray,0,size-1);
      INT64  result_bubble=ExtBubbleArray[size-1];
      printf(" Bubble Sort:  %I64u ms, %I64d\n",time_bubble,result_bubble);
      BenchOutput("TestBubblesort",time_bubble,&result_bubble);
     }
//--- 4. Call
   if(test_num<=0 || test_num>16 || test_num==4)
     {
      INT64  result_call;
      UINT64 time_call=TestCall(result_call);
      printf("        Call:  %I64u ms, %I64d\n",time_call,result_call);
      BenchOutput("TestCall",time_call,&result_call);
     }
//--- 5. Fibo
   if(test_num<=0 || test_num>16 || test_num==5)
     {
      INT64  result_fibo;
      UINT64 time_fibo=TestFibo(result_fibo);
      printf("        Fibo:  %I64u ms, %I64d\n",time_fibo,result_fibo);
      BenchOutput("TestFibo",time_fibo,&result_fibo);
     }
//--- 6. Matrix
   if(test_num<=0 || test_num>16 || test_num==6)
     {
      INT64  result_matrix;
      UINT64 time_matrix=TestMatrix(result_matrix);
      printf("      Matrix:  %I64u ms, %I64d\n",time_matrix,result_matrix);
      BenchOutput("TestMatrix",time_matrix,&result_matrix);
     }
//--- 7. Moments
   if(test_num<=0 || test_num>16 || test_num==7)
     {
      INT64  result_moments;
      UINT64 time_moments=TestMoments(result_moments);
      printf("     Moments:  %I64u ms, %I64d\n",time_moments,result_moments);
      BenchOutput("TestMoments",time_moments,&result_moments);
     }
//--- 8. Nested Loops
   if(test_num<=0 || test_num>16 || test_num==8)
     {
      INT64  result_nested_loops;
      UINT64 time_nested_loops=TestNestedLoop(result_nested_loops);
      printf("Nested Loops:  %I64u ms, %I64d\n",time_nested_loops,result_nested_loops);
      BenchOutput("TestNestedLoops",time_nested_loops,&result_nested_loops);
     }
//--- 9. PI
   if(test_num<=0 || test_num>16 || test_num==9)
     {
      INT64  result_pi;
      UINT64 time_pi=TestPiDigits(result_pi);
      printf("   Pi Digits:  %I64u ms, %I64d\n",time_pi,result_pi);
      BenchOutput("TestPiDigits",time_pi,&result_pi);
     }
//--- 10. Random
   if(test_num<=0 || test_num>16 || test_num==10)
     {
      INT64  result_random;
      UINT64 time_random=TestRandom(result_random);
      printf("      Random:  %I64u ms, %I64d\n",time_random,result_random);
      BenchOutput("TestRandom",time_random,&result_random);
     }
//--- 11. Sieve
   if(test_num<=0 || test_num>16 || test_num==11)
     {
      INT64  result_sieve;
      UINT64 time_sieve=TestSieve(result_sieve);
      printf("       Sieve:  %I64u ms, %I64d\n",time_sieve,result_sieve);
      BenchOutput("TestSieve",time_sieve,&result_sieve);
     }
//--- 12. Math
   if(test_num<=0 || test_num>16 || test_num==12)
     {
      INT64  result_int=0;
      double result_double=0.0;
      UINT64 time_math=TestMath(result_int,result_double);
      printf(" Simple Math:  %I64u ms, %I64d %.0f\n",time_math,result_int,result_double);
      BenchOutput("TestMath",time_math,&result_int);
     }
//--- 13. String
   if(test_num<=0 || test_num>16 || test_num==13)
     {
      std::string str;
      UINT64 time_string=TestString(str);
      printf("      String:  %I64u ms, %d\n",time_string,(int)str.length());
      BenchOutput("TestString",time_string,NULL);
     }
//--- 14. Str Prep
   if(test_num<=0 || test_num>16 || test_num==14)
     {
      INT64  result_str_prep;
      UINT64 time_str_prep=TestStrPrep(result_str_prep);
      printf("    Str Prep:  %I64u ms, %I64d\n",time_str_prep,result_str_prep);
      BenchOutput("TestStringPrep",time_str_prep,&result_str_prep);
     }
//--- 15. Str Rev
   if(test_num<=0 || test_num>16 || test_num==15)
     {
      INT64  result_str_rev;
      UINT64 time_str_rev=TestStrRev(result_str_rev);
      printf("     Str Rev:  %I64u ms, %I64d\n",time_str_rev,result_str_rev);
      BenchOutput("TestStringRev",time_str_rev,NULL);
     }
//--- 16. Str Sum
   if(test_num<=0 || test_num>16 || test_num==16)
     {
      INT64  result_sum=0;
      UINT64 time_str_sum=TestStrSum(result_sum);
      printf("     Str Sum:  %I64u ms, %I64d\n",time_str_sum,result_sum);
      BenchOutput("TestStringSum",time_str_sum,&result_sum);
     }
//---
   return(0);
  }
//+------------------------------------------------------------------+
