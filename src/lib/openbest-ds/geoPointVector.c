#include "openbest-ds/geoPointVector.h"
#include "openbest-ds/temperatureGlobals.h"

geoPointVector* createGeoPointVectorN()
{
  geoPointVector* p= (geoPointVector*)malloc(sizeof(geoPointVector));
  
  if ( !p )
    eprintf("malloc failed in createGeoPointVectorN");
  
  p->t= NULL;
  p->n= 0;
}

geoPointVector* createGeoPointVector1(int n)
{
  geoPointVector* p= createGeoPointVectorN();
  p->t= (geoPoint*)(malloc(sizeof(geoPoint)*n));
  
  if ( ! (p->t) )
    eprintf("malloc failed in createGeoPointVector1");
  p->n= n;
  return p;
}

void destroyGeoPointVector(geoPointVector* gpv)
{
  int i;
  for ( i= 0; i < gpv->n; ++i )
    free(gpv->t + i);
  free(gpv);
}

void displayGPV(geoPointVector* p)
{
  printf("%d geoPoints", p->n);
  fflush(stdout);
}

real* distanceGPV(geoPointVector* pv, geoPoint* p)
{
  real* res= (real*)malloc(sizeof(real)*(pv->n));
  
  if ( !res )
    eprintf("malloc failed in distanceGPV");
  
  int i;
  for ( i= 0; i < pv->n; ++i )
    res[i]= distanceGP(pv->t + i, p);
}

geoPoint* centerGPV(geoPointVector* pv)
{
  real x, y, z;
  x= y= z= 0;
  
  int i;
  for ( i= 0; i < pv->n; ++i )
  {
    x+= pv->t[i].x;
    y+= pv->t[i].y;
    z+= pv->t[i].z;
  }
  x/= pv->n;
  y/= pv->n;
  z/= pv->n;
  
  real d= sqrt(x*x + y*y + z*z);
  x/= d;
  y/= d;
  z/= d;
  
  real lat= 90 - acos(z)*180/M_PI;
  real longitude= atan2(y, x)*180/M_PI;
  
  return createGeoPoint2(lat, longitude);
}

int nearest1GPV(geoPoint* p, geoPointVector* pv)
{
  real minDist= FLT_MAX;
  int minIdx= 0;
  real tmp;
  int i;
  for ( i= 0; i < pv->n; ++i )
  {
    tmp= distanceGP(pv->t + i, p);
    if ( tmp < minDist )
    {
      minDist= tmp;
      minIdx= i;
    }
  }
  
  return minIdx;
}

int* nearestNGPV(geoPoint* p, geoPointVector* pv, int n)
{
  real* dist= distanceGPV(pv, p);
  int* res= (int*)(malloc(sizeof(int)*n));
  
  if ( !res )
    eprintf("malloc failed in nearestN");
  
  real minDist= FLT_MAX;
  int minIdx= 0;
  real tmp;
  while ( n > 0 )
  {
      --n;
      
      int i;
      for ( i= 0; i < pv->n; ++i )
      {
	tmp= dist[i];
	if ( tmp < minDist )
	{
	  minDist= tmp;
	  minIdx= i;
	}
      }
      
      res[n]= minIdx;
      dist[minIdx]= FLT_MAX;
  }
  
  free(dist);
  
  return res;
}

void rotateGPV(geoPointVector* g, matrix3 rot)
{
  int i;
  for ( i= 0; i < g->n; ++i )
    rotateGP(g->t+i, rot);
}
