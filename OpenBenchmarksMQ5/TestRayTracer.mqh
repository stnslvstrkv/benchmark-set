//+------------------------------------------------------------------+
//|                                                TestRayTracer.mqh |
//|                        Copyright 2020, MetaQuotes Software Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#include <Files/FileBMP.mqh>

int ExtLevels=6;            // levels count
int ExtChilds=9;            // childs count
int ExtResolution=2048;     // dimension in pixels

#define  ss     2
#define  ss_sqr 4

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
   Sphere            bound;                // common box
   Sphere            leaf;
   uint              diff;                 // far from optimal

   void              Set(Sphere &_bound,Sphere &_leaf,uint jump)
     {
      bound=_bound;
      leaf=_leaf;
      diff=jump;
     }

   void              Intersect(const Ray &ray,Hit &hit) const;
  };
//---
Point ExtLight;                      // light source point
Node  ExtNodesPool[];                // common pool
uint  ExtNodesPoolTotal=0;
//+------------------------------------------------------------------+
//| Intersections research                                           |
//+------------------------------------------------------------------+
void Node::Intersect(const Ray &ray,Hit &hit) const
  {
   uint index=0;
//--- walk thru the all objects
   while(index<ExtNodesPoolTotal)
     {
      //--- not in box? skip subtree
      if(ExtNodesPool[index].bound.Intersect(ray)>=hit.t)
        {
         index+=ExtNodesPool[index].diff;
         continue;
        }
      //--- check accuracy
      double t=ExtNodesPool[index].leaf.Intersect(ray);
      if(t<hit.t)
        {
         hit.t=t;
         Point temp=ray.d;
         temp.Mul(t);
         temp.Sum(ray.o);

         hit.n=ExtNodesPool[index].leaf.GetNormal(temp);
        }
      //--- next
      index++;
     }
//---
  }
//+------------------------------------------------------------------+
//| Tracing itself                                                   |
//+------------------------------------------------------------------+
double TraceRay(const Node &scene,const Ray &ray)
  {
   Hit    hit;
   double diffuse=0.0;
//--- trace primary
   scene.Intersect(ray,hit);
   if(hit.t!=infinity)
      diffuse=-hit.n.Distance(ExtLight);
//---
   return(diffuse>0.0 ? diffuse : 0.0);
  }
//+------------------------------------------------------------------+
//| Scene trace                                                      |
//+------------------------------------------------------------------+
void Trace(const int width,const int height,uint& uint_array[])
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
//--- walk thru whole surface
   int pos=0;
   for(int i=height; i; --i)
     {
      for(int j=width; j; --j)
        {
         double g=0.0;
         for(int idx=0; idx<ss_sqr; ++idx)
           {
            ray.d=scan;
            ray.d.Sum(rgss[idx]);
            ray.d.Normal();
            //--- trace
            g+=TraceRay(ExtNodesPool[0],ray);
           }
         //--- result
         uint_array[pos++]=(uint)(scale*g);
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
//| Fill all the nodes                                               |
//+------------------------------------------------------------------+
uint CreateScene(uint index,int total,const int lvl,Point &c,Point &d,double radius)
  {
  if(index>=ExtNodesPoolTotal)
      return(index);
//--- fill current point
   ExtNodesPool[index].bound.Set(c,2.0*radius);
   ExtNodesPool[index].leaf.Set(c,radius);
   ExtNodesPool[index].diff=(lvl>1 ? total : 1);
   index++;
//--- is the end?
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
//--- return next position
   return(index);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
ulong TestRayTracer(long& result)
  {
   uint uint_array[];
   int  levels=ExtLevels;
   uint t0=GetTickCount();
//--- allocate node array for the all children
   ExtNodesPoolTotal=ExtChilds;
   while(--levels>1)
      ExtNodesPoolTotal=(ExtNodesPoolTotal*ExtChilds)+ExtChilds;
   ExtNodesPoolTotal++;

   ArrayResize(ExtNodesPool,ExtNodesPoolTotal);
//--- create scene
   Point c,d;
   c.Set(0.0,0.0,0.0);
   d.Set(+.25,+1,-.5);
   d.Normal();
   CreateScene(0,ExtNodesPoolTotal,ExtLevels,c,d,1.0);
//---
   ExtLight.Set(-0.5,-0.65,0.9);
   ExtLight.Normal();

   int image_size=ExtResolution*ExtResolution;
   if(ArrayResize(uint_array,image_size) && ArraySize(uint_array)==image_size)
      Trace(ExtResolution,ExtResolution,uint_array);
   uint elapsed=GetTickCount()-t0;
//---
   CFileBMP file_bmp;
   int file_handle=file_bmp.OpenWrite("pictures\\raytrace.bmp",true);
   if(file_handle!=INVALID_HANDLE)
     {
      int written=file_bmp.Write32BitsArray(uint_array,ExtResolution,ExtResolution);
      file_bmp.Close();
      result=written;
     }
   else
      result=0;
//---
   return(elapsed);
  }
//+------------------------------------------------------------------+
