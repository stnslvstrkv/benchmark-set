//+------------------------------------------------------------------+
//|                                               TestPathTracer.cpp |
//|                   Copyright 2001-2020, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//|                                                                  |
//| Article:  https://habr.com/ru/post/434528                        |
//|http://fabiensanglard.net/postcard_pathtracer/formatted_full.html |
//+------------------------------------------------------------------+
#include <windows.h>
#include <stdio.h>
#include <math.h>

//---
#define HIT_NONE 0
#define HIT_LETTER 1
#define HIT_WALL 2
#define HIT_SUN 3

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
struct Vec
  {
   float             x;
   float             y;
   float             z;

   Vec(float v=0)
     {
      x=y=z=v;
     }

   Vec(float a, float b, float c=0)
     {
      x=a;
      y=b;
      z=c;
     }

   Vec operator +(Vec r)
     {
      return Vec(x + r.x, y + r.y, z + r.z);
     }

   Vec operator *(Vec r)
     {
      return Vec(x * r.x, y * r.y, z * r.z);
     }

   float operator %(Vec r)
     {
      return x * r.x + y * r.y + z * r.z;
     }

   Vec operator !()   // intnv square root
     {
      return *this * (1/sqrtf(*this%*this));
     }
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
float rmin(float l, float r)
  {
   return(l < r ? l : r);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
float randomVal()
  {
   return(float) rand()/RAND_MAX;
  }
//+------------------------------------------------------------------+
//| Rectangle CSG equation. Returns minimum signed distance from     |
//| space carved by lowerLeft and opposite rectangle upperRight      |
//+------------------------------------------------------------------+
float BoxTest(Vec position, Vec lowerLeft, Vec upperRight)
  {
   lowerLeft=position + lowerLeft * -1;
   upperRight=upperRight + position * -1;
   return -rmin(
                rmin(
                      rmin(lowerLeft.x, upperRight.x),
                      rmin(lowerLeft.y, upperRight.y)),
                rmin(lowerLeft.z, upperRight.z));
  }
//+------------------------------------------------------------------+
//| Sample the world using Signed Distance Fields                    |
//+------------------------------------------------------------------+
float QueryDatabase(Vec position, int &hitType)
  {
   float distance=1e9;
   Vec f=position; // Flattened position (z=0)
   f.z=0;
   static const char letters[15*4]=                                      // 15 two points lines
   {  '5','O','5','_','5','W','9','W','5','_','9','_',                   // P (without curve)
      'A','O','E','O','C','O','C','_','A','_','E','_',                   // I
      'I','O','Q','_','I','_','Q','O',                                   // X
      'U','O','Y','_','Y','_',']','O','W','W','[','W',                   // A
      'a','O','a','_','a','W','e','W','a','_','e','_','c','W','i','O' }; // R (without curve)

   for(int i=0; i < sizeof(letters); i+=4)
     {
      Vec begin=Vec(letters[i] - 79.f, letters[i + 1] - 79.f) * .5f;
      Vec e=Vec(letters[i + 2] - 79.f, letters[i + 3] - 79.f) * .5f + begin * -1;
      Vec o=f + (begin + e * rmin(-rmin((begin + f * -1)%e/(e%e),0),1)) * -1;
      distance=rmin(distance, o%o); // compare squared distance.
     }
   distance=sqrtf(distance); // Get real distance, not square distance.

//--- Two curves (for P and R in PixaR) with hard-coded locations.
   Vec curves[]={Vec(-11, 6), Vec(11, 6)};
   for(int i=2; i--;)
     {
      Vec o=f + curves[i] * -1;
      distance=rmin(distance,
                     o.x > 0 ? fabsf(sqrtf(o%o) - 2)
                             : (o.y+=o.y > 0 ? -2 : 2, sqrtf(o%o))
                 );
     }
   distance=powf(powf(distance, 8) + powf(position.z, 8), .125) - .5f;
   hitType=HIT_LETTER;

   float roomDist;
//--- rmin(A,B) = Union with Constructive solid geometry
   roomDist=rmin(
                 //-rmin carves an empty space
                 -rmin(
                       // Lower room
                       BoxTest(position, Vec(-30, -.5, -30), Vec(30, 18, 30)),
                       // Upper room
                       BoxTest(position, Vec(-25, 17, -25), Vec(25, 20, 25))),
                 BoxTest( // Ceiling "planks" spaced 8 units apart.
                         Vec(fmodf(fabsf(position.x), 8),
                             position.y,
                             position.z),
                         Vec(1.5, 18.5, -25),
                         Vec(6.5, 20, 25)));
   if(roomDist < distance)
      distance=roomDist, hitType=HIT_WALL;
//--- Everything above 19.9 is light source.
   float sun=19.9f - position.y;
   if(sun < distance)
      distance=sun, hitType=HIT_SUN;
//---
   return distance;
  }

// Perform signed sphere marching
// Returns hitType 0, 1, 2, or 3 and update hit position/normal
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int RayMarching(Vec origin, Vec direction, Vec &hitPos, Vec &hitNorm)
  {
   int   hitType=HIT_NONE;
   int   noHitCount=0;
   float d; // distance from closest object in world.

//--- Signed distance marching
   for(float total_d=0; total_d < 100; total_d+=d)
      if((d=QueryDatabase(hitPos=origin + direction * total_d, hitType)) < .01 ||
         ++noHitCount > 99)
         return hitNorm=!Vec(QueryDatabase(hitPos + Vec(.01, 0), noHitCount) - d,
                             QueryDatabase(hitPos + Vec(0, .01), noHitCount) - d,
                             QueryDatabase(hitPos + Vec(0, 0, .01), noHitCount) - d),
                hitType; // Weird return statement where a variable is also updated.
//---
   return(0);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
Vec Trace(Vec origin, Vec direction)
  {
   Vec sampledPosition, normal, color, attenuation=1;
   Vec lightDirection(!Vec(.6, .6, 1)); // Directional light

   for(int bounceCount=3; bounceCount--;)
     {
      int hitType=RayMarching(origin, direction, sampledPosition, normal);
      //--- Wall hit uses color yellow?
      if(hitType==HIT_NONE)
         break;
       //--- Specular bounce on a letter. No color acc.
      if(hitType==HIT_LETTER)
        {
         direction=direction + normal *(normal%direction * -2);
         origin=sampledPosition + direction * 0.1;
         attenuation=attenuation * 0.2; // Attenuation via distance traveled.
        }
      //--- Wall hit uses color yellow?
      if(hitType==HIT_WALL)
        {
         float incidence=normal%lightDirection;
         float p=6.283185f * randomVal();
         float c=randomVal();
         float s=sqrtf(1 - c);
         float g=normal.z < 0 ? -1.f : 1.f;
         float u=-1/(g + normal.z);
         float v=normal.x * normal.y * u;
         direction=Vec(v,
                       g + normal.y * normal.y * u,
                       -normal.y) * (cosf(p) * s)
                  +
                   Vec(1 + g * normal.x * normal.x * u,
                       g * v,
                       -g * normal.x) * (sinf(p) * s) + normal * sqrtf(c);
         origin=sampledPosition + direction * .1;
         attenuation=attenuation * 0.2;
         if(incidence > 0 &&
            RayMarching(sampledPosition + normal * .1,
                        lightDirection,
                        sampledPosition,
                        normal)==HIT_SUN)
            color=color + attenuation * Vec(500, 400, 100) * incidence;
        }
      //--- Sun Color
      if(hitType==HIT_SUN)
        {
         color=color + attenuation * Vec(50, 80, 100);
         break;
        }
     }
//---
   return color;
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
   bmp.bi.biHeight   =-height;
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
UINT64 TestPathTracer(INT64 &result)
  {
   const int w=960;
   const int h=540;
   const int samplesCount=8;

   Vec   position(-22, 5, 25);
   Vec   goal=!(Vec(-3, 4, 0) + position * -1);
   Vec   left=!Vec(goal.z, 0, -goal.x) * (1./w);
   Vec   up(goal.y * left.z - goal.z * left.y,goal.z * left.x - goal.x * left.z,goal.x * left.y - goal.y * left.x); // cross-product to get the up vector
//--- buffer
   unsigned char *buffer=new unsigned char[w*h*sizeof(UINT)+64];
   unsigned char *buffer_save=buffer;
   unsigned int   buffer_len=0;
   UINT64         t0=::GetTickCount64();
//--- ppm header
   buffer_len=snprintf((char*)buffer,64,"P6 %d %d 255 ", w, h);
   buffer+=buffer_len;

   for(int y=h; y--;)
     {
      for(int x=w; x--;)
        {
         Vec color;
         //---
         for(int p=samplesCount; p--;)
            color=color + Trace(position, !(goal + left * (x - w/2 + randomVal()) + up * (y - h/2 + randomVal())));

         //--- Reinhard tone mapping
         color=color * (1./samplesCount) + 14./241;
         Vec o=color + 1;
         color=Vec(color.x/o.x, color.y/o.y, color.z/o.z) * 255;
         //--- printf("%c%c%c", (int) color.x, (int) color.y, (int) color.z);
         *buffer++=0xFF;
         *buffer++=(unsigned char)color.z;
         *buffer++=(unsigned char)color.y;
         *buffer++=(unsigned char)color.x;
         buffer_len+=sizeof(UINT);
        }
     }
   
   UINT64 elapsed=::GetTickCount64()-t0;
//--- write bitmap
   write_bitmap32b("pathtrace.bmp",buffer_save,w,h);
   delete[] buffer_save;
//---
   result=w*h;
   return(elapsed);
  }
//+------------------------------------------------------------------+
