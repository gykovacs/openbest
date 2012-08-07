#ifndef _BUILD_TARGET_TABLE_H_
#define _BUILD_TARGET_TABLE_H_

#include "openbest-ds/geoPoint2.h"
#include "openbest-av/berkeleyAverageOptions.h"

void buildTargetTable(geoPoint2** locations, int n_locations, geoPoint2** map, int n_map, berkeleyAverageOptions* options, float** target_map, int* n_target_map1, int* n_target_map2, int** nearest, int* n_nearest);

#endif
