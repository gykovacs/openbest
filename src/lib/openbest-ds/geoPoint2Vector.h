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

/**
  * destroys the parameter geoPoint2Vector object
  * @param gpv geoPoint2Vector to destroy
  */
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

/**
  * returns true if any of the geoPoint2 objects in g has NaN coordinates
  * @param g geoPoint2Vector object to check
  * @return true if at least one of the elements of geoPoint2 has NaN coordinates, false otherwise
  */
int isnanGP2V(geoPoint2Vector* g);

/**
  * sets the uncertainties of the elements in geoPoint2Vector to the specified values
  * @param gpv geoPoint2Vector object to set the uncertainties of
  * @param lat_error specified uncertainty of latitude
  * @param long_error specified uncertainty of longitude
  * @param elev_error specified uncertainty of elevation
  * @returns the pointer of the parameter vector gpv
  */
geoPoint2Vector* setUncertaintyGP2V1(geoPoint2Vector* gpv, real lat_error, real long_error, real elev_error);

/**
  * sets the uncertainties of the elements in geoPoint2Vector to the specified values
  * @param gpv geoPoint2Vector object to set the uncertainties of
  * @param lat_error specified array of uncertainty of latitude
  * @param long_error specified array of uncertainty of longitude
  * @param elev_error specified array of uncertainty of elevation
  * @returns the pointer of the parameter vector gpv
  */
geoPoint2Vector* setUncertaintyGP2VA(geoPoint2Vector* gpv, real* lat_error, real* long_error, real* elev_error);

/** ucScale structure*/
typedef struct
{
  real lat;
  real longitude;
} ucScale;

/**
  * returns the ucScale of the parameter geoPoint2 object
  * @param gp2 parameter geoPoint2 object
  * @returns the uncertainty scale of the parameter
  */
ucScale uncertaintyScaleGP2(geoPoint2* gp2);

/**
  * returns the ucScale vector of the parameter geoPoint2Vector object
  * @param gp2 parameter geoPoint2Vector array object
  * @returns the uncertainty scales of the elements of gp2
  */
ucScale* uncertaintyScaleGP2V(geoPoint2Vector* gp2);

/**
  * represents the "overlapTiles" function from the Matlab BEST code
  * @param p parameter geoPoint2Vector object
  * @returns new geoPoint2Vector object
  */
geoPoint2Vector* overlapTiles(geoPoint2Vector* p);

/**
  * represents the "merge" function from the Matlab BEST code
  * @param p parameter geoPoint2Vector object
  * @returns new geoPoint2Vector object
  */
geoPoint2Vector* merge(geoPoint2Vector* p);

/**
  * extracts the latitude values from the parameter geoPoint2 vector into the parameter t
  * @param p geoPoint2 vector
  * @param t preallocated area for the latitude values
  */
void getLatitudes(geoPoint2Vector* p, real* t);

/**
  * extracts the longitude values from the parameter geoPoint2 vector into the parameter t
  * @param p geoPoint2 vector
  * @param t preallocated area for the longitude values
  */
void getLongitudes(geoPoint2Vector* p, real* t);

/**
  * extracts the elevations values from the parameter geoPoint2 vector into the parameter t
  * @param p geoPoint2 vector
  * @param t preallocated area for the elevations values
  */
void getElevations(geoPoint2Vector* p, real* t);

/**
  * extracts the latitude uncertainty values from the parameter geoPoint2 vector into the parameter t
  * @param p geoPoint2 vector
  * @param t preallocated area for the latitude uncertainty
  */
void getLatUncertainties(geoPoint2Vector* p, real* t);

/**
  * extracts the longitude uncertainty values from the parameter geoPoint2 vector into the parameter t
  * @param p geoPoint2 vector
  * @param t preallocated area for the longitude uncertainty values
  */
void getLongUncertainties(geoPoint2Vector* p, real* t);

/**
  * extracts the elevation uncertainty values from the parameter geoPoint2 vector into the parameter t
  * @param p geoPoint2 vector
  * @param t preallocated area for the elevation uncertainty values
  */
void getElevUncertainties(geoPoint2Vector* p, real* t);

/**
  * represents the "breakOverlap" method from the Matlab-BEST code
  * @param t1 tile 1
  * @param t2 tile 2
  * @param region1
  * @param region2
  * @param intersect
  * @param c1p
  * @param c2p
  * @param c3p
  */
void breakOverlap(real* t1, real* t2, real** region1, real** region2, real** intersect, int* c1p, int* c2p, int* c3p);

/**
 * TODO: implement subsref and subsasgn on demand
 */

#endif
