#ifndef _GEO_POINT2_H_
#define _GEO_POINT2_H_

#include <float.h>

#include "openbest-ds/geoPoint.h"

/**
 * geoPoint2 structure
 */ 
typedef struct 
{
  /** latitude coordinate */
  real latitude;
  /** longitude coordinate */
  real longitude;
  /** elevation coordinate */
  real elevation;

  /** latitude uncertainty */
  real lat_uncertainty;
  /** longitude uncertainty */
  real long_uncertainty;
  /** elevation uncertainty */
  real elev_uncertainty;
  
  /** x coordinate */
  real x;
  /** y coordiante */
  real y;
  /** z coordinate */
  real z;
} geoPoint2;

typedef geoPoint2* geoPoint2p;

/**
 * creates a geoPoint2 instance and returns its pointer
 * @returns pointer of the new instance
 */ 
geoPoint2* createGeoPoint2N();

/**
 * copys a geoPoint2 instance and assigns the values of the fields of the parameter to it
 * @param gp instance to copy
 * @returns the copied instance
 */ 
geoPoint2* createGeoPoint2C(geoPoint2* gp);

/**
 * creates geoPoint2 from geoPoint and returns its pointer
 * @param gp geoPoint object
 * @returns the pointer of the new object
 */ 
geoPoint2* createGeoPoint21(geoPoint* gp);

/**
 * creates geoPoint2 structure from latitude and longitude
 * @param latitude latitude paramter
 * @param longitude longitude parameter
 * @returns the pointer of the new object
 */ 
geoPoint2* createGeoPoint22(real latitude, real longitude);

/**
 * creates geoPoint2 structur from latitude, longitude and elevation
 * @param latitude latitude parameter
 * @param longitude longitude parameter
 * @returns the pointer of the new object
 */ 
geoPoint2* createGeoPoint23(real latitude, real longitude, real elevation);

geoPoint2* createGeoPoint26(real latitude, real longitude, real elevation, real lat_uncertainty, real long_uncertainty, real elev_unceratinty);

void setGeoPoint2(geoPoint2* gp, real latitude, real longitude, real elevation, real lat_uncertainty, real long_uncertainty, real elev_uncertainty);

void computeXYZ_GP2(geoPoint2* gp);

void displayGP2(geoPoint2* gp);

real distanceGP2(geoPoint2* a, geoPoint2* b);

void rotateGP2(geoPoint2* pt, matrix3 rot);

int isnanGP2(geoPoint2* gp);

geoPoint2* setUncertaintyGP2(geoPoint2* gp, real lat_error, real long_error, real elev_error);

bool isValidGP2(geoPoint2* gp);

#endif
