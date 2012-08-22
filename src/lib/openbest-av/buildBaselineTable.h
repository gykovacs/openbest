#ifndef _BUILD_BASELINE_TABLE_H_
#define _BUILD_BASELINE_TABLE_H_

#include "openbest-ds/config-ds.h"
#include "openbest-av/berkeleyAverageOptions.h"

// This function uses a Kriging process to determine the optimal mix of
// baseline values to be used in adjusting the global temperatures to a true
// average temperature.

// This function is an auxillary helper function to BerkeleyAverageCore and
// is never expected to be called directly

/**
  * buildBaselineTable from the Matlab-BEST code
  * @param correlation_table pointer of correlation_table
  * @param n_correlation_table number of rows and columns of the correlation_table matrix
  * @param target_map pointer to the target_map matrix
  * @param n_target_map1 rows of the target_map
  * @param n_target_map2 columns of the target_map
  * @param occurance_table occurance_table matrix
  * @param n_occurance_table1 rows of the occurance_table
  * @param n_occurance_table1 columns of the occurance_table
  * @param expand_map expand_map vector
  * @param n_expand_map length of the expand_map vector
  * @param mask mask array
  * @param n_mask size of the mask array
  * @param options berkeleyAverageOptions instance
  * @param sites_elev sites elevation array
  * @param n_sites_elev length of sites_elev
  * @param sites_lat sites latitude array
  * @param n_sites_lat length of sites_lat
  * @param map_elev map elevation array
  * @param n_map_elev length of map_elev
  * @param map_lat map latitude array
  * @param n_map_lat length of map_lat
  * @param baseline_weightsIO pointer to a baseline weights matrix
  * @param n_baseline_weightsIO pointer to the length of baseline_weights
  * @param completeness pointer to a double variable for completeness
  * @param global_completeness pointer to a double variable for global completeness
  * @param cov_mapIO pointer to the covariance map matrix
  * @param n_cov_mapIO pointer to the length of the covariance map
  * @param base_mapIO pointer to the base map matrix
  * @param n_base_map1IO pointer to the number of rows of the base_map
  * @param n_base_map2IO pointer to the number of columns of base_map
  * @param base_mapping_sitesIO pointer to array base mapping sites
  * @param base_mapping_mapIO pointer to array base mapping map
  */
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
