#ifndef _GEO_POINT_H_
#define _GEO_POINT_H_

#include <float.h>
#include <stdio.h>
#include <math.h>

#include "openbest-ds/config.h"
#include "openbest-ds/temperatureGlobals.h"
#include "openbest-ds/matrix3.h"

/**
 * geoPoint structure
 */ 
typedef struct 
{
  /** latitude coordinate */
  real latitude;
  /** longitude coordinate */
  real longitude;
  /** elevation coordinate */
  real elevation;
  
  /** x coordinate */
  real x;
  /** y coordinate */
  real y;
  /** z coordinate */
  real z;
} geoPoint;

/**
 * create new geoPoint object (allocation)
 * @returns new geoPoint object with FLT_MAX field values
 */ 
geoPoint* createGeoPointN();

/**
 * create new geoPoint object (copy)
 * @param g object to copy
 * @returns new geoPoint object with copied fields
 */ 
geoPoint* createGeoPointC(geoPoint* g);

/**
 * create new geoPoint object with latitude and longitude given
 * @param latitude latitude of the point
 * @param longitude longitude of the point
 * @returns new geoPoint object with the given fields
 */ 
geoPoint* createGeoPoint2(real latitude, real longitude);

/**
 * create new geoPoint object with latitude, longitude and elevation given
 * @param latitude latitude of the point
 * @param longitude longitude of the point
 * @param elevation elevation of the point
 * @returns new geoPoint object with given fields
 */
geoPoint* createGeoPoint3(real latitude, real longitude, real elevation);

void destroyGeoPoint(geoPoint* g);

/**
 * computes spherical polar coordinates
 * @param geoPoint geoPoint object to fill its x, y and z fields
 */ 
void computeXYZ_GP(geoPoint* pt);

/**
 * displays the properties of a geoPoint object
 * @param pt parameter to display it's properties
 */ 
void displayGP(geoPoint* pt);

/**
 * Computes the distance between the points. Distance is computed as a great circle on a spherical Earth. Elevation changes are not considered.
 * @param p1 point #1
 * @param p2 point #2
 * @returns the distance of the parameters
 */ 
real distanceGP(geoPoint* p1, geoPoint* p2);

/**
 * Rotates the geoPoint with matrix rot
 * @param pt geoPoint to rotate
 * @param rot rotation matrix
 */ 
void rotateGP(geoPoint* pt, matrix3 rot);

/**
 * Rotates the geoPoint and returns a new, rotated instance
 * @param pt geoPoint to rotate
 * @param rot rotation matrix
 * @returns rotated geoPoint
 */ 
geoPoint rotateGPN(geoPoint* pt, matrix3 rot);

#endif