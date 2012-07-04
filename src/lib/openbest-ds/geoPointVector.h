#ifndef _GEO_POINT_VECTOR_H_
#define _GEO_POINT_VECTOR_H_

#include <stdlib.h>

#include "openbest-ds/geoPoint.h"
#include "openbest-ds/matrix3.h"

/**
 * struct to represent geoPointVector
 */ 
typedef struct
{
  geoPoint* t;
  int n;
} geoPointVector;

/**
 * creates a new geoPointVector
 * @returns new geoPointVector instance
 */ 
geoPointVector* createGeoPointVectorN();

/**
 * creates a new geoPointVector with n elements
 * @param n number of elements
 * @returns new geoPointVector instance
 */ 
geoPointVector* createGeoPointVector1(int n);

void destroyGeoPointVector(geoPointVector* gpv);

/**
 * prints the properties of the parameter to the stdout
 * @param p parameter geoPointVector
 */ 
void displayGPV(geoPointVector* p);

/**
 * returns an array of distances
 * @param pv to compute the distances from
 * @param p to compute the distances to
 * @returns distance array
 */ 
real* distanceGPV(geoPointVector* pv, geoPoint* p);

/**
 * computes the center of a geoPointVector
 * @param pv pointer to a geoPointVector instance
 * @returns the center
 */ 
geoPoint* centerGPV(geoPointVector* pv);

/**
 * returns the index of the closest element in pv to p
 * @param p geoPoint to find the closest element to
 * @param pv geoPointVector to find the closest element from
 * @returns index of the closest element
 */ 
int nearest1GPV(geoPoint* p, geoPointVector* pv);

/**
 * returns the indeces of the closest n elements in pv to p
 * @param p geoPoint to find the closest elements to
 * @param pv geoPointVector to find the closest elements from
 * @returns array of indeces
 */ 
int* nearestNGPV(geoPoint* p, geoPointVector* pv, int n);

/**
 * rotates the points in the parameter geoPointVector
 * @param g geoPointVector to rotate
 * @param rot rotation matrix3
 */ 
void rotateGPV(geoPointVector* g, matrix3 rot);

/**
 * TODO: implement subsref and subsasgn on demand
 */

#endif