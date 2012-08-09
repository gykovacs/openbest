#ifndef _BUILD_SPATIAL_MAP_H_
#define _BUILD_SPATIAL_MAP_H_

void buildSpatialMap(float* correlation_table, int n_correlation_table,
                     real* target, int n_target,
                     bool* occurance_table, int n_occurance_table1, int n_occurance_table2,
                     int* expand_map, int n_expand_map,
                     double nugget, berkeleyAverageOptions* options

                     );

#endif
