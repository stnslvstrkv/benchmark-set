//+------------------------------------------------------------------+
//|                                                   OpenBenchmarks |
//|                   Copyright 2000-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#define MAX_SIZE_BUBBLE_SORT 25000
extern int ExtBubbleArray[MAX_SIZE_BUBBLE_SORT];

UINT64 TestAckermann(INT64 &result);
UINT64 TestArrays(INT64 &result);
UINT64 TestBubbleSort(int array[],int left,int right);
UINT64 TestCall(INT64 &result);
UINT64 TestFibo(INT64 &result);
UINT64 TestMatrix(INT64 &result);
UINT64 TestMoments(INT64 &result);
UINT64 TestNestedLoop(INT64 &result);
UINT64 TestPiDigits(INT64 &result);
UINT64 TestRandom(INT64 &result);
UINT64 TestSieve(INT64 &result);
UINT64 TestString(std::string &str);
UINT64 TestStrPrep(INT64 &result);
UINT64 TestStrRev(INT64 &result);
UINT64 TestStrSum(INT64 &result);
UINT64 TestMath(INT64 &result_int,double &result_double);
//+------------------------------------------------------------------+
