//+------------------------------------------------------------------+
//|                                               TestMandelbrot.mqh |
//|                        Copyright 2020, MetaQuotes Software Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#include <Files\FileBMP.mqh>

double ExtX0=-2.0;
double ExtY0=-2.0;
double ExtX1=-1.0;
double ExtY1=-1.0;

#define RESOLUTION 2048
//+------------------------------------------------------------------+
//| Mandelbrot calculation for pixel                                 |
//+------------------------------------------------------------------+
uint MFractal(double x0,double y0,double x1,double y1,int gx,int gy,int w,int h)
  {
   double dx = x0 + gx * (x1-x0) / (double)w * 4;
   double dy = y0 + gy * (y1-y0) / (double)h * 4;
   double x  = 0;
   double y  = 0;
   double xx = 0;
   double yy = 0;
   double xy = 0;
   uint   i=0;
//---
   while((xx+yy)<4 && i<20000)
     {
      xx = x*x;
      yy = y*y;
      xy = x*y;
      y = xy+xy+dy;
      x = xx-yy+dx;
      i++;
     }

   if(i==20000)
      return(0);
//---
   return((uint)(0xFFFFFF/20000.0)*i);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
ulong TestMandelbrot(long& result)
  {
   uint uint_array[];
   uint t0=GetTickCount();
//--- check parameters
   if(ExtResolution<1)
     {
      result=0;
      return(0);
     }
//--- ensure output buffer
   int count=ExtResolution*ExtResolution;
   if(ArrayResize(uint_array,count)<0)
     {
      result=0;
      return(0);
     }
//--- calculate result
   for(int i=0; i<count; i++)
      uint_array[i]=MFractal(ExtX0,ExtY0,ExtX1,ExtY1,i%RESOLUTION,i/RESOLUTION,RESOLUTION,RESOLUTION);
   uint elapsed=GetTickCount()-t0;
//---
   CFileBMP file_bmp;
   int file_handle=file_bmp.OpenWrite("pictures\\mandelbrot.bmp",true);
   if(file_handle!=INVALID_HANDLE)
     {
      int written=file_bmp.Write32BitsArray(uint_array,RESOLUTION,RESOLUTION);
      file_bmp.Close();
      result=written;
     }
   else
      result=0;
//---
   return(elapsed);
  }
//+------------------------------------------------------------------+
