#ifndef _GEO_POINT_VECTOR_H_
#define _GEO_POINT_VECTOR_H_

#include <stdlib.h>

#include <openbest-ds/geoPoint.h>

typedef struct
{
  geoPoint* t;
  int n;
} geoPointVector;

geoPointVector* createGeoPointVectorN();

geoPointVector* createGeoPointVector1(int n);

void displayGPV(geoPointVector* p);

float* distanceGPV(geoPointVector* pv, geoPoint* p);

geoPoint* center(geoPointVector* pv);

int nearest1(geoPoint* p, geoPointVector* pv)
{
  float minDist= FLT_MAX;
  int minIdx= 0;
  float tmp;
  int i;
  for ( i= 0; i < pv->n; ++i )
  {
    tmp= distance(pv->t + i, p);
    if ( tmp < minDist )
    {
      minDist= tmp;
      minIdx= i;
    }
  }
  
  return minIdx;
}

int* nearestN(geoPoint* p, geoPointVector* pv, int n)
{
  float* dist= distanceGPV(pv, p);
  int* res= (int*)(malloc(sizeof(int)*n));
  
  float minDist= FLT_MAX;
  int minIdx= 0;
  float tmp;
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

#endif