#include <openbest-ds/geoPointVector.h>
#include <openbest-ds/temperatureGlobals.h>

//const float earth_radius;

geoPointVector* createGeoPointVectorN()
{
  geoPointVector* p= (geoPointVector*)malloc(sizeof(geoPointVector));
  p->t= NULL;
  p->n= 0;
}

geoPointVector* createGeoPointVector1(int n)
{
  geoPointVector* p= createGeoPointVectorN();
  p->t= (geoPoint*)(malloc(sizeof(geoPoint)*n));
  p->n= n;
  return p;
}

void displayGPV(geoPointVector* p)
{
  printf("%d geoPoints", p->n);
  fflush(stdout);
}

float* distanceGPV(geoPointVector* pv, geoPoint* p)
{
  float* res= (float*)malloc(sizeof(float)*(pv->n));
  int i;
  for ( i= 0; i < pv->n; ++i )
    res[i]= distance(pv->t + i, p);
}

geoPoint* center(geoPointVector* pv)
{
  float x, y, z;
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
  
  float d= sqrt(x*x + y*y + z*z);
  x/= d;
  y/= d;
  z/= d;
  
  float lat= 90 - acos(z)*180/M_PI;
  float longitude= atan2(y, x)*180/M_PI;
  
  return createGeoPoint2(lat, longitude);
}