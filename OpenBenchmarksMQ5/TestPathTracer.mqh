//+------------------------------------------------------------------+
//|                                               TestPathTracer.mqh |
//|                        Copyright 2020, MetaQuotes Software Corp. |
//|                                             https://www.mql5.com |
//|Article:  https://habr.com/ru/post/434528                         |
//|http://fabiensanglard.net/postcard_pathtracer/formatted_full.html |
//+------------------------------------------------------------------+
#include <Files/FileBMP.mqh>

#define XRGB(r,g,b)    (0xFF000000|(uchar(r)<<16)|(uchar(g)<<8)|uchar(b))
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
struct Vec
  {
   double            x,y,z;

                     Vec(double v=0)
     {
      x=y=z=v;
     }

                     Vec(const Vec &v)
     {
      x=v.x;
      y=v.y;
      z=v.z;
     }

                     Vec(double a,double b,double c=0)
     {
      x = a;
      y = b;
      z = c;
     }

   Vec               operator+(Vec &r)
     {
      return Vec(x+r.x,y+r.y,z+r.z);
     }

   Vec               operator+(double v)
     {
      return Vec(x+v,y+v,z+v);
     }

   Vec               operator*(Vec &r)
     {
      return Vec(x*r.x,y*r.y,z*r.z);
     }

   Vec               operator*(double v)
     {
      return Vec(x*v,y*v,z*v);
     }

   double            operator%(Vec &r)
     {
      return x*r.x+y*r.y+z*r.z;
     }

   // intnv square root
   Vec               operator!()
     {
      return this * (1 / sqrt(this % this));
     }
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
double min(double l,double r)
  {
   return(l<r ? l : r);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
double randomVal()
  {
   return rand() / 32767.0;
  }
//+------------------------------------------------------------------+
//| Rectangle CSG equation. Returns minimum signed distance from     |
//| space carved by lowerLeft vertex and opposite vertex upperRight  |
//+------------------------------------------------------------------+
double BoxTest(Vec &position,Vec &lowerLeft,Vec &upperRight)
  {
   lowerLeft=position+lowerLeft*-1;
   upperRight=upperRight+position*-1;
   return -min(
             min(
                min(lowerLeft.x, upperRight.x),
                min(lowerLeft.y, upperRight.y)),
             min(lowerLeft.z,upperRight.z));
  }

#define HIT_NONE 0
#define HIT_LETTER 1
#define HIT_WALL 2
#define HIT_SUN 3
//+------------------------------------------------------------------+
//| Sample the world using Signed Distance Fields                    |
//+------------------------------------------------------------------+
double QueryDatabase(Vec &position,int &hitType)
  {
   double distance=1e9;
   Vec f=position; // Flattened position (z=0)
   f.z=0;
   static const char letters[15*4]=                                      // 15 two points lines
   {  '5','O','5','_','5','W','9','W','5','_','9','_',                   // P (without curve)
      'A','O','E','O','C','O','C','_','A','_','E','_',                   // I
      'I','O','Q','_','I','_','Q','O',                                   // X
      'U','O','Y','_','Y','_',']','O','W','W','[','W',                   // A
      'a','O','a','_','a','W','e','W','a','_','e','_','c','W','i','O' }; // R (without curve)

   for(int i=0; i<sizeof(letters); i+=4)
     {
      Vec begin=Vec(letters[i]-79,letters[i+1]-79)*.5;
      Vec e = Vec(letters[i + 2] - 79, letters[i + 3] - 79) * .5 + begin * -1;
      Vec o = f + (begin + e * min(-min((begin + f * -1) % e / (e % e),0),1))*-1;
      distance=min(distance,o%o); // compare squared distance.
     }
   distance=sqrt(distance); // Get real distance, not square distance.

// Two curves (for P and R in PixaR) with hard-coded locations.
   Vec curves[2];
   curves[0]=Vec(-11,6);
   curves[1]=Vec(11,6);

   for(int i=0; i<2; i++)
     {
      Vec o=f+curves[i]*-1;

      if(o.x>0)
         distance=min(distance, fabs(sqrt(o%o)-2));
      else
        {
         o.y+=o.y>0 ? -2 : 2;
         distance=min(distance,sqrt(o%o));
        }
     }
   distance= pow(pow(distance,8)+pow(position.z,8),.125)-.5;
   hitType = HIT_LETTER;

   double roomDist;
   roomDist=min(// min(A,B) = Union with Constructive solid geometry
                //-min carves an empty space
               -min(// Lower room
                    BoxTest(position,Vec(-30,-.5,-30),Vec(30,18,30)),
                    // Upper room
                    BoxTest(position,Vec(-25,17,-25),Vec(25,20,25))
               ),
               BoxTest(// Ceiling "planks" spaced 8 units apart.
                  Vec(fmod(fabs(position.x),8),
                      position.y,
                      position.z),
                  Vec(1.5, 18.5, -25),
                  Vec(6.5, 20, 25)
               )
            );
   if(roomDist<distance)
     {
      distance=roomDist;
      hitType=HIT_WALL;
     }

   double sun=19.9-position.y; // Everything above 19.9 is light source.
   if(sun<distance)
     {
      distance=sun;
      hitType=HIT_SUN;
     }

   return distance;
  }
//+------------------------------------------------------------------+
//| Perform signed sphere marching                                   |
//| Returns hitType 0, 1, 2, or 3 and update hit position/normal     |
//+------------------------------------------------------------------+
int RayMarching(Vec &origin,Vec &direction,Vec &hitPos,Vec &hitNorm)
  {
   int   hitType=HIT_NONE;
   int   noHitCount=0;
   double d;                    // distance from closest object in world.
//--- Signed distance marching
   for(double total_d=0; total_d<100;)
     {
      hitPos=origin+direction*total_d;

      if((d=QueryDatabase(hitPos,hitType))<.01 || ++noHitCount>99)
        {
         hitNorm =!Vec(QueryDatabase(hitPos+Vec(.01,0),noHitCount)-d,QueryDatabase(hitPos+Vec(0,.01),noHitCount)-d,QueryDatabase(hitPos+Vec(0,0,.01),noHitCount)-d);
         return hitType;
        }

      total_d+=d;
     }
//---
   return(0);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
Vec Trace(Vec &origin,Vec &direction)
  {
   Vec sampledPosition,normal,clr,attenuation=1;
   Vec lightDirection(!Vec(.6,.6,1)); // Directional light
//---
   for(int bounceCount=0; bounceCount<3; bounceCount++)
     {
      int hitType=RayMarching(origin,direction,sampledPosition,normal);
      //--- No hit. This is over, return color.
      if(hitType==HIT_NONE)
         break;
      //--- Specular bounce on a letter. No color acc.
      if(hitType==HIT_LETTER)
        {
         direction=direction+normal *(normal%direction*-2);
         origin=sampledPosition+direction*0.1;
         attenuation=attenuation*0.2; // Attenuation via distance traveled.
        }
      //--- Wall hit uses color yellow?
      if(hitType==HIT_WALL)
        {
         double incidence=normal%lightDirection;
         double p = 6.283185 * randomVal();
         double c = randomVal();
         double s = sqrt(1 - c);
         double g = normal.z < 0 ? -1 : 1;
         double u = -1 / (g + normal.z);
         double v = normal.x * normal.y * u;
         direction=Vec(v,
                       g+normal.y*normal.y*u,
                       -normal.y) *(cos(p)*s)
                   +
                   Vec(1+g*normal.x*normal.x*u,
                       g*v,
                       -g*normal.x) *(sin(p)*s)+normal*sqrt(c);
         origin=sampledPosition+direction*.1;
         attenuation=attenuation*0.2;
         if(incidence>0 &&
            RayMarching(sampledPosition+normal*.1,
                        lightDirection,
                        sampledPosition,
                        normal)==HIT_SUN)
            clr=clr+attenuation*Vec(500,400,100)*incidence;
        }
      //--- Sun Color
      if(hitType==HIT_SUN)
        {
         clr=clr+attenuation*Vec(50,80,100);
         break;
        }
     }
//---
   return(clr);
  }

#define WIDTH  960
#define HEIGHT 540

uint ExtPixels[WIDTH*HEIGHT+16];

//+------------------------------------------------------------------+
//| Script program start function                                    |
//+------------------------------------------------------------------+
ulong TestPathTracer(long& result)
  {
   const int w=WIDTH;
   const int h=HEIGHT;
   const int samplesCount=8;

   uint t0=GetTickCount();
   int  pos=0;
   Vec  position(-22,5,25);
   Vec  goal = !(Vec(-3, 4, 0) + position * -1);
   Vec  left = !Vec(goal.z, 0, -goal.x) * (1. / w);
   Vec  up(goal.y*left.z-goal.z*left.y,goal.z * left.x - goal.x * left.z,goal.x * left.y - goal.y * left.x);  // cross-product to get the up vector
//---
   for(int y=h; y--;)
     {
      for(int x=w; x--;)
        {
         Vec clr;
         //---
         for(int p=0; p<samplesCount; p++)
            clr=clr+Trace(Vec(-22,5,25),!(goal+left *(x-w/2+randomVal())+up *(y-h/2+randomVal())));

         //--- Reinhard tone mapping
         clr = clr * (1. / samplesCount) + 14. / 241;
         Vec o = clr + 1;
         clr = Vec(clr.x / o.x, clr.y / o.y, clr.z / o.z) * 255;
         //---
         ExtPixels[pos++]=XRGB(clr.x,clr.y,clr.z);
        }
     }

   uint elapsed=GetTickCount()-t0;
//---
   CFileBMP file_bmp;
   int file_handle=file_bmp.OpenWrite("pictures\\pathtrace.bmp",true);
   if(file_handle!=INVALID_HANDLE)
     {
      int written=file_bmp.Write32BitsArray(ExtPixels,w,-h);
      file_bmp.Close();
      result=written;
     }
   else
      result=0;
//---
   return(elapsed);
  }
//+------------------------------------------------------------------+
