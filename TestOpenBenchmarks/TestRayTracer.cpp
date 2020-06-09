//+------------------------------------------------------------------+
//|                                                                  |
//|                   Copyright 2001-2011, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include <windows.h>
#include <stdio.h>
#include <math.h>

int ExtLevels=6;            // levels count
int ExtChilds=9;            // childs count

#define RESOLUTION 2048
#define M_PI       3.14159265358979323846   // pi as in <correct_math_defines.h>
#define ss         2
#define ss_sqr     4

//--- our rotated grid
const double grid[ss_sqr][2]=
  {
     {-3/3.0,-1/3.0},{+1/3.0,-3/3.0},
     {-1/3.0,+3/3.0},{+3/3.0,+1/3.0}
  };

const double infinity=1.7E+308;    //1./0,
//+------------------------------------------------------------------+
//| 3D point                                                         |
//+------------------------------------------------------------------+
struct Point
  {
   double            x;
   double            y;
   double            z;

   void              Zero(void)
     {
      x=0.0;
      y=0.0;
      z=0.0;
     }

   void              Set(const double _x,const double _y,const double _z)
     {
      x=_x;
      y=_y;
      z=_z;
     }

   void              Sum(const Point &pt)
     {
      x+=pt.x;
      y+=pt.y;
      z+=pt.z;
     }

   void              Sub(const Point &pt)
     {
      x-=pt.x;
      y-=pt.y;
      z-=pt.z;
     }

   void              Mul(const double val)
     {
      x*=val;
      y*=val;
      z*=val;
     }

   void              Minus(void)
     {
      x=-x;
      y=-y;
      z=-z;
     }

   double            Distance(const Point &pt) const
     {
      return(x*pt.x+y*pt.y+z*pt.z);
     }

   double            DistanceSqr(void) const
     {
      return(x*x+y*y+z*z);
     }

   void              Cross(const Point &pt)
     {
      Point po;
      po.Set(y*pt.z-z*pt.y,z*pt.x-x*pt.z,x*pt.y-y*pt.x);
      Set(po.x,po.y,po.z);
     }

   void              Normal(void)
     {
      Mul(1.0/sqrt(DistanceSqr()));
     }
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
struct Ray
  {
   Point             o;
   Point             d;

   void              Set(Point &_pt)             { o=_pt;         }
   void              Set(Point &_pt,Point &_dis) { o=_pt; d=_dis; }
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
struct Hit
  {
   Point             n;
   double            t;

                     Hit() { n.Zero(); t=infinity; }
  };
//+------------------------------------------------------------------+
//| Sphere description                                               |
//+------------------------------------------------------------------+
struct Sphere
  {
   Point             pt;
   double            radius;

   void              Set(Point &_pt,const double _radius)
     {
      pt=_pt;
      radius=_radius;
     }

   Point             GetNormal(Point &_pt)
     {
      Point p=_pt;
      p.Sub(pt);
      p.Mul(1.0/radius);
      return(p);
     }

   double            Intersect(const Ray &ray) const
     {
      Point  v=pt;
      v.Sub(ray.o);

      double b=ray.d.Distance(v);
      double disc=b*b-v.DistanceSqr()+radius*radius;
      //--- branch away from the square root?
      if(disc<0.0)
         return(infinity);
      //--- cond. move
      double d=sqrt(disc);
      double t2=b+d;
      double t1=b-d;

      if(t2<0.0)
         return(infinity);
      else
         return(t1>0.0 ? t1 : t2);
     }
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
struct Node
  {
   Sphere            bound;
   Sphere            leaf;
   UINT              diff;

   void              Set(Sphere &_bound,Sphere &_leaf,UINT jump)
     {
      bound=_bound;
      leaf=_leaf;
      diff=jump;
     }

   void              Intersect(const Ray &ray,Hit &hit) const;
  };
//---
Point ExtLight;                      // Point of light
Node *ExtNodesPool=NULL;             // common nodes pool
UINT  ExtNodesPoolTotal=0;           // size of nodes pool
//+------------------------------------------------------------------+
//| Intersections research                                           |
//+------------------------------------------------------------------+
void Node::Intersect(const Ray &ray,Hit &hit) const
  {
   UINT index=0;
//--- list all nodes
   while(index<ExtNodesPoolTotal)
     {
      //--- не попали в бокс? пропустим поддерево
      if(ExtNodesPool[index].bound.Intersect(ray)>=hit.t)
        {
         index+=ExtNodesPool[index].diff;
         continue;
        }
      //--- проверяем детально
      double t=ExtNodesPool[index].leaf.Intersect(ray);
      if(t<hit.t)
        {
         hit.t=t;
         Point temp=ray.d;
         temp.Mul(t);
         temp.Sum(ray.o);

         hit.n=ExtNodesPool[index].leaf.GetNormal(temp);
        }
      //--- идем дальше
      index++;
     }
//---
  }
//+------------------------------------------------------------------+
//| Сама трассировка                                                 |
//+------------------------------------------------------------------+
double TraceRay(const Node &scene,const Ray &ray)
  {
   Hit    hit;
   double diffuse=0.0;
//---
   scene.Intersect(ray,hit); // trace primary
   if(hit.t!=infinity)
      diffuse=-hit.n.Distance(ExtLight);
//---
   return(diffuse>0.0 ? diffuse : 0.0);
  }
//+------------------------------------------------------------------+
//| Трассировка по сцене                                             |
//+------------------------------------------------------------------+
void Trace(const int width,const int height,UINT* uint_array)
  {
   double w=width;
   double h=height;
   double rcp=1.0/ss;
   double scale=256.0/ss_sqr;
//--- eye, looking into Z
   Ray    ray;
   ray.o.Set(0,0,-4.5);
//--- precomp
   Point  rgss[ss_sqr];
   for(int i=0; i<ss_sqr; i++)
      rgss[i].Set(grid[i][0]*rcp-w/2.0,grid[i][1]*rcp-h/2.0,0);
//--- scan line
   Point scan;
   scan.Set(0,w-1,w);
//--- пройдемся по всей поверхности
   int pos=0;
   for(int i=height; i; --i)
     {
      for(int j=width; j; --j)
        {
         double g=0;
         for(int idx=0; idx<ss_sqr; ++idx)
           {
            ray.d=scan;
            ray.d.Sum(rgss[idx]);
            ray.d.Normal();
            //--- trace
            g+=TraceRay(ExtNodesPool[0],ray);
           }
         //--- выведем результат
         uint_array[pos++]=(UINT)(scale*g);
         scan.x+=1; // next pixel
        }
      //--- next line
      scan.x=0;
      scan.y-=1;
     }
//---
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
struct Basis
  {
   Point             up;
   Point             b1;
   Point             b2;

   void              Set(const Point &v)
     {
      Point n=v;
      n.Normal();
      //---
      if((n.x*n.x!=1.)&(n.y*n.y!=1.)&(n.z*n.z!=1.))
        {
         //cough
         b1=n;
         if(n.y*n.y>n.x*n.x)
           {
            if(n.y*n.y>n.z*n.z)
               b1.y=-b1.y;
            else
               b1.z=-b1.z;
           }
         else
           {
            if(n.z*n.z>n.x*n.x)
               b1.z=-b1.z;
            else
               b1.x=-b1.x;
           }
        }
      else
        {
         b1.Set(n.z,n.x,n.y);
        }
      //---
      up=n;
      b2=up;
      b2.Cross(b1);
      b1=up;
      b1.Cross(b2);
     }
  };
//+------------------------------------------------------------------+
//| Заполняем все ноды                                               |
//+------------------------------------------------------------------+
UINT CreateScene(UINT index,int total,const int lvl,Point &c,Point &d,double radius)
  {
  if(index>=ExtNodesPoolTotal)
      return(index);
//--- заполним текущую точку
   ExtNodesPool[index].bound.Set(c,2.0*radius);
   ExtNodesPool[index].leaf.Set(c,radius);
   ExtNodesPool[index].diff=(lvl>1 ? total : 1);
   index++;
//--- это конец?
   if(lvl<=1)
      return(index);
//---
   total=(total-ExtChilds)/ExtChilds;
   if(total<1)
      total=1;

   Basis  b;
   b.Set(d);
   double nr =radius*1/3.0;
   double daL=2.0*M_PI/6.0;
   double daU=2.0*M_PI/3.0;
   double a  =0.0;
//--- lower ring
   Point ndir,temp;

   for(int i=0; i<6; ++i)
     {
      ndir=b.b2;
      ndir.Mul(cos(a));

      temp=b.b1;
      temp.Mul(sin(a));
      ndir.Sum(temp);

      temp=d;
      temp.Mul(-0.2);
      ndir.Sum(temp);

      ndir.Normal();
      //---
      temp=ndir;
      temp.Mul(radius+nr);
      temp.Sum(c);

      index=CreateScene(index,total,lvl-1,temp,ndir,nr);
      a+=daL;
     }
//--- upper ring
   a-=daL/3.0;
   for(int i=0; i<3; ++i)
     {
      ndir=b.b2;
      ndir.Mul(cos(a));

      temp=b.b1;
      temp.Mul(sin(a));
      ndir.Sum(temp);

      temp=d;
      temp.Mul(0.6);
      ndir.Sum(temp);

      ndir.Normal();
      //---
      temp=ndir;
      temp.Mul(radius+nr);
      temp.Sum(c);

      index=CreateScene(index,total,lvl-1,temp,ndir,nr);
      a+=daU;
     }
//--- вернем следующую позицию
   return(index);
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
UINT64 TestRayTracer(INT64 &result)
  {
   UINT  *uint_array=NULL;
   int    levels=ExtLevels;
   UINT64 t0=::GetTickCount64();
//--- подсчитаем количество детей и выделим массив
   ExtNodesPoolTotal=ExtChilds;
   while(--levels>1)
      ExtNodesPoolTotal=(ExtNodesPoolTotal*ExtChilds)+ExtChilds;
   ExtNodesPoolTotal++;

   ExtNodesPool=new Node[ExtNodesPoolTotal];
//--- создадим сцену
   Point c,d;
   c.Set(0.0,0.0,0.0);
   d.Set(+.25,+1,-.5);
   d.Normal();
   CreateScene(0,ExtNodesPoolTotal,ExtLevels,c,d,1.0);
//---
   ExtLight.Set(-0.5,-0.65,0.9);
   ExtLight.Normal();

   int image_size=RESOLUTION*RESOLUTION;
   uint_array=new UINT[image_size];
      Trace(RESOLUTION,RESOLUTION,uint_array);
//---
   UINT64 elapsed=::GetTickCount64()-t0;
//--- write bmp
   write_bitmap32b("raytrace.bmp",uint_array,RESOLUTION,RESOLUTION);
   delete[] uint_array;
//---
   result=image_size*sizeof(UINT);
   return(elapsed);
  }
//+------------------------------------------------------------------+
