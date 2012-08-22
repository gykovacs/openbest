#ifndef _BUILD_TARGET_TABLE_H_
#define _BUILD_TARGET_TABLE_H_

#include "openbest-ds/geoPoint2.h"
#include "openbest-av/berkeleyAverageOptions.h"

/**
  * buildTargetTable function from the Matlab-BEST code
  * @param locations array of locations
  * @param n_locations length of array locations
  * @param map array of map points
  * @param n_map length of map
  * @param options berkeleyAverageOptions instance
  * @param target_map result matrix
  * @param n_target_map1 pointer to the rows of target_map
  * @param n_target_map2 pointer to the columns of target_map
  * @param nearest nearest index array
  * @param n_nearest map length of nearest array
  */
void buildTargetTable(geoPoint2** locations, int n_locations,
            geoPoint2** map, int n_map,
            berkeleyAverageOptions* options,
            float** target_map, int* n_target_map1, int* n_target_map2,
            int** nearest, int* n_nearest);

#endif
