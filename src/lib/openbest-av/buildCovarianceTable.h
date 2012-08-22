#ifndef _BUILD_COVARIANCE_TABLE_H_
#define _BUILD_COVARIANCE_TABLE_H_

#include "openbest-ds/geoPoint2.h"
#include "openbest-av/berkeleyAverageOptions.h"

/**
  * builds the covariance table - see the Matlab-BEST code
  * @param locations_short array of site locations
  * @param n_locations_short length of locations_short
  * @param options berkeleyAverageOptions instance
  * @param weights pointer to weights array
  * @param n_weights pointer to the size of weights
  * @param nugget pointer to a float variable for the nugget value
  */
void buildCovarianceTable(geoPoint2** locations_short, int n_locations_short, berkeleyAverageOptions* options, float** weights, int* n_weights, float* nugget);

#endif
