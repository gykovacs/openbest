#ifndef _COLLAPSE_LOCATIONS_H_
#define _COLLAPSE_LOCATIONS_H_

#include "openbest-ds/geoPoint2.h"
#include "openbest-ds/printOut.h"

#include <math.h>

/**
  * In order to improve computational performance, we treat station sites
  * that ar closely located in space as if they occur at the same location
  * for the purpose of computing the initial Kriging coefficients. This
  * function performs the "collapse" that replaces closely colocated
  * stations location information with an effective value in-between. The
  * station time series remain separate and independent, and only the
  * positions used for Kriging coefficient purposes are adjusted
  */

/**
  * computes the center of the elements indexed by array indeces in the geoPoint2 vector v
  * @param v array of geoPoint2 instances
  * @param n_v length of array v
  * @param indeces array of indeces, indexing those elements of v, that are used to compute the center
  * @param n_indeces length of array indeces
  * @returns the geoPoint2 vector of the center
  */
geoPoint2* centerGP2NV(geoPoint2** v, int n_v, int* indeces, int n_indeces);

/**
  * collapses locations that are closer to each other than max_move
  * @param locations array of geoPoint2 instances
  * @param n_locations length of array locations
  * @param max_move maximum distance in kilometers (according to the Viterbi-formula)
  * @param locationsOut output parameter - pointer to an empty array of geoPoint2 instances
  * @param n_locationsOut output parameter - pointer to the length of array locationsOut
  * @param indecesOut output parameter - pointer to an empty array of indeces; these indeces can be used to find out which locations are mapped to which collapsed ones
  * @param n_indecesOut output parameter - pointer to the length of array indecesOut
  */
void collapseLocations(geoPoint2** locations, int n_locations, double max_move, geoPoint2p** locationsOut, int* n_locationsOut, int** indecesOut, int* n_indecesOut);

#endif
