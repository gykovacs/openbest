#ifndef _GEO_POINT2_VECTOR_H_
#define _GEO_POINT2_VECTOR_H_

#include <stdlib.h>

#include "openbest-ds/geoPoint2.h"
#include "openbest-ds/matrix3.h"
#include "openbest-ds/tile.h"

/**
 * struct to represent geoPoint2Vector
 */ 
typedef struct
{
  geoPoint2* t;
  int n;
} geoPoint2Vector;

/**
 * creates a new geoPoint2Vector
 * @returns new geoPoint2Vector instance
 */ 
geoPoint2Vector* createGeoPoint2VectorN();

/**
 * creates a new geoPoint2Vector with n elements
 * @param n number of elements
 * @returns new geoPoint2Vector instance
 */ 
geoPoint2Vector* createGeoPoint2Vector1(int n);

void destroyGeoPoint2Vector(geoPoint2Vector* gpv);

/**
 * prints the properties of the parameter to the stdout
 * @param p parameter geoPoint2Vector
 */ 
void displayGP2V(geoPoint2Vector* p);

/**
 * returns an array of distances
 * @param pv to compute the distances from
 * @param p to compute the distances to
 * @returns distance array
 */ 
real* distanceGP2V(geoPoint2Vector* pv, geoPoint2* p);

/**
 * computes the center of a geoPoint2Vector
 * @param pv pointer to a geoPoint2Vector instance
 * @returns the center
 */ 
geoPoint* centerGP2V(geoPoint2Vector* pv);

/**
 * returns the index of the closest element in pv to p
 * @param p geoPoint to find the closest element to
 * @param pv geoPoint2Vector to find the closest element from
 * @returns index of the closest element
 */ 
int nearest1GP2V(geoPoint2* p, geoPoint2Vector* pv);

/**
 * returns the indeces of the closest n elements in pv to p
 * @param p geoPoint to find the closest elements to
 * @param pv geoPoint2Vector to find the closest elements from
 * @returns array of indeces
 */ 
int* nearestNGP2V(geoPoint2* p, geoPoint2Vector* pv, int n);

/**
 * rotates the points in the parameter geoPoint2Vector
 * @param g geoPoint2Vector to rotate
 * @param rot rotation matrix3
 */ 
void rotateGP2V(geoPoint2Vector* g, matrix3 rot);

int isnanGP2V(geoPoint2Vector* g);

geoPoint2Vector* setUncertaintyGP2V1(geoPoint2Vector* gpv, real lat_error, real long_error, real elev_error);

geoPoint2Vector* setUncertaintyGP2VA(geoPoint2Vector* gpv, real* lat_error, real* long_error, real* elev_error);

typedef struct
{
  real lat;
  real longitude;
} ucScale;

ucScale uncertaintyScaleGP2(geoPoint2* gp2);

ucScale* uncertaintyScaleGP2V(geoPoint2Vector* gp2);

geoPoint2Vector* overlapTiles(geoPoint2Vector* p);

geoPoint2Vector* merge(geoPoint2Vector* p);

void getLatitudes(geoPoint2Vector* p, real* t);
void getLongitudes(geoPoint2Vector* p, real* t);
void getElevations(geoPoint2Vector* p, real* t);
void getLatUncertainties(geoPoint2Vector* p, real* t);
void getLongUncertainties(geoPoint2Vector* p, real* t);
void getElevUncertainties(geoPoint2Vector* p, real* t);

void breakOverlap(real* t1, real* t2, real** region1, real** region2, real** intersect, int* c1p, int* c2p, int* c3p);

/**
 * TODO: implement subsref and subsasgn on demand
 */

#endif