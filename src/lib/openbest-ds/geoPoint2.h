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
 * creates geoPoint2 structure from latitude, longitude and elevation
 * @param latitude latitude parameter
 * @param longitude longitude parameter
 * @returns the pointer of the new object
 */ 
geoPoint2* createGeoPoint23(real latitude, real longitude, real elevation);

/**
  * creates geoPoint2 structure with latitude, longitude, elevation and corresponding uncertainties
  * @param latitude latitude parameter
  * @param longitude longitude parameter
  * @param elevation elevation parameter
  * @param lat_uncertainty uncertainty of the latitude
  * @param long_uncertainty uncertainty of the longitude
  * @param elev_uncertainty uncertainty of the elevation
  * @return pointer of the new structure
  */
geoPoint2* createGeoPoint26(real latitude, real longitude, real elevation, real lat_uncertainty, real long_uncertainty, real elev_unceratinty);

/**
  * sets the parameters of the geoPoint2 structure
  * @param gp geoPoint2 structure to set
  * @param latitude latitude parameter
  * @param longitude longitude parameter
  * @param elevation elevation parameter
  * @param lat_uncertainty uncertainty of the latitude
  * @param long_uncertainty uncertainty of the longitude
  * @param elev_uncertainty uncertainty of the elevation
  */
void setGeoPoint2(geoPoint2* gp, real latitude, real longitude, real elevation, real lat_uncertainty, real long_uncertainty, real elev_uncertainty);

/**
  * computes the x, y, z values of the geoPoint2 object
  * @param gp geoPoint2 object to compute the x, y, z values of
  */
void computeXYZ_GP2(geoPoint2* gp);

/**
  * displayes the parameter geoPoint2 object
  * @param gp geoPoint2 object to display
  */
void displayGP2(geoPoint2* gp);

/**
  * returns the distance of the two geoPoint2 objects
  * @param a geoPoint2 object 1
  * @param b geoPoint2 object 2
  * @returns Euclidean-distance
  */
real distanceGP2(geoPoint2* a, geoPoint2* b);

/**
  * rotates the parameter geoPoint2 object by rotation matrix rot
  * @param pt geoPoint2 object to rotate
  * @param rot rotation matrix
  */
void rotateGP2(geoPoint2* pt, matrix3 rot);

/**
  * returns logical true if the parameter geoPoint2 object contains NaN coordinates
  * @param gp geoPoint2 object
  * @returns logical true if the parameter has NaN coordinates, false otherwise
  */
int isnanGP2(geoPoint2* gp);

/**
  * sets the uncertainty of the parameters geoPoint2 object and returns its pointer
  * @param gp geoPoint2 object to set the uncertainty parameters of
  * @param lat_error uncertainty of latitude
  * @param long_error uncertainty of longitude
  * @param elev_error uncertainty of elevation
  * @returns the pointer of parameter gp
  */
geoPoint2* setUncertaintyGP2(geoPoint2* gp, real lat_error, real long_error, real elev_error);

/**
  * returns true if the parameter geoPoint2 object has normal coordinates
  * @param gp geoPoint2 object to check the coordinates of
  * @returns true if the parameter gp has normal coordinates, false otherwise
  */
bool isValidGP2(geoPoint2* gp);

#endif
