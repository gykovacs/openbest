#ifndef _BUILD_COVARIANCE_TABLE_H_
#define _BUILD_COVARIANCE_TABLE_H_

#include "openbest-ds/geoPoint2.h"
#include "openbest-av/berkeleyAverageOptions.h"

void buildCovarianceTable(geoPoint2** locations_short, int n_locations_short, berkeleyAverageOptions* options, float** weights, int* n_weights, float* nugget);

#endif
