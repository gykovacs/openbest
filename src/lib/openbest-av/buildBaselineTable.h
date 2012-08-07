#ifndef _BUILD_BASELINE_TABLE_H_
#define _BUILD_BASELINE_TABLE_H_

#include "openbest-ds/config-ds.h"
#include "openbest-av/berkeleyAverageOptions.h"

// This function uses a Kriging process to determine the optimal mix of
// baseline values to be used in adjusting the global temperatures to a true
// average temperature.

// This function is an auxillary helper function to BerkeleyAverageCore and
// is never expected to be called directly

void buildBaselineTable(float* correlation_table, int n_correlation_table,
                        float* target_map, int n_target_map1, int n_target_map2,
                        bool* occurance_table, int n_occurance_table1, int n_occurance_table2,
                        int* expand_map, int n_expand_map, float nugget,
                        real* mask, int n_mask, berkeleyAverageOptions* options,
                        real* sites_elev, int n_sites_elev, real* sites_lat, int n_sites_lat,
                        real* map_elev, int n_map_elev, real* map_lat, int n_map_lat,

                        real** baseline_weightsIO, int* n_baseline_weightsIO,
                        double* completeness, double* global_completeness,
                        real** cov_mapIO, int* n_cov_mapIO,
                        double** base_mapIO, int* n_base_map1IO, int* n_base_map2IO,
                        real** base_mapping_sitesIO, real** base_mapping_mapIO);

#endif
