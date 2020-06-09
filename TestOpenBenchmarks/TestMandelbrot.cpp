//+------------------------------------------------------------------+
//|                                               TestMandelbrot.cpp |
//|                   Copyright 2001-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//|                                                                  |
//+------------------------------------------------------------------+
#include <windows.h>
#include <stdio.h>

double ExtX0=-2.0;
double ExtY0=-2.0;
double ExtX1=-1.0;
double ExtY1=-1.0;

#define RESOLUTION 2048
//+------------------------------------------------------------------+
//| Mandelbrot calculation for pixel                                 |
//+------------------------------------------------------------------+
UINT MFractal(double x0,double y0,double x1,double y1,int gx,int gy,int w,int h)
  {
   double dx = x0 + gx * (x1-x0) / (double)w * 4;
   double dy = y0 + gy * (y1-y0) / (double)h * 4;
   double x  = 0;
   double y  = 0;
   double xx = 0;
   double yy = 0;
   double xy = 0;
   UINT   i=0;
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
   return((UINT)(0xFFFFFF/20000.0)*i);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
static void write_bitmap32b(const char *filename,void *data,int width,int height)
  {
#pragma pack(push,1)
   struct BMP
     {
      BITMAPFILEHEADER fh;
      BITMAPINFOHEADER bi;
     } bmp={};
#pragma pack(pop)

   bmp.fh.bfOffBits  =sizeof(bmp);
   bmp.fh.bfSize     =sizeof(bmp)+width*height*sizeof(UINT);
   bmp.fh.bfType     =0x4D42;
   bmp.bi.biBitCount =32;
   bmp.bi.biHeight   =height;
   bmp.bi.biWidth    =width;
   bmp.bi.biPlanes   =1;
   bmp.bi.biSize     =sizeof(bmp.bi);
   bmp.bi.biSizeImage=width*height*sizeof(UINT);

   FILE *out;
   fopen_s(&out,filename,"wb");
   fwrite(&bmp,sizeof(bmp),1,out);
   fwrite(data,sizeof(UINT),width*height,out);
   fclose(out);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
UINT64 TestMandelbrot(INT64 &result)
  {
   UINT  *uint_array=NULL;
   UINT64 t0=GetTickCount64();
//--- ensure output buffer
   int    image_size=RESOLUTION*RESOLUTION;
   uint_array=new UINT[image_size];
//--- calculate result
   for(int i=0; i<image_size; i++)
      uint_array[i]=MFractal(ExtX0,ExtY0,ExtX1,ExtY1,i%RESOLUTION,i/RESOLUTION,RESOLUTION,RESOLUTION);
   
   UINT64 elapsed=::GetTickCount64()-t0;
//--- write bitmap
   write_bitmap32b("mandelbrot.bmp",uint_array,RESOLUTION,RESOLUTION);
   delete[] uint_array;
//---
   result=image_size*sizeof(UINT);
   return(elapsed);
  }
//+------------------------------------------------------------------+
