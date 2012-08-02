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

geoPoint2* centerGP2NV(geoPoint2** v, int n_v, int* indeces, int n_indeces);

void collapseLocations(geoPoint2** locations, int n_locations, double max_move, geoPoint2p** locationsOut, int* n_locationsOut, int** indecesOut, int* n_indecesOut);

#endif
