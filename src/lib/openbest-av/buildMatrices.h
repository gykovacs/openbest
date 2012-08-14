#ifndef _BUILD_MATRICES_H_
#define _BUILD_MATRICES_H_

#include "openbest-ds/printOut.h"
#include "openbest-ds/config-ds.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-av/berkeleyAverageOptions.h"

void buildMatrices(temp_t** data_array, int* n_data_array, int n_n_data_array,
                   int** dates_array, int* n_dates_array, int n_n_dates_array,
                   double* new_spatial_table, int n_new_spatial_table1, int n_new_spatial_table2,
                   float* map, int n_map,
                   int* near_index, int n_near_index,
                   real* t_res, int n_t_res,
                   real* b_res, int n_b_res,
                   real sigma, real sigma_full, bool first, berkeleyAverageOptions* options,

                   double** base_weightsIO, int* n_base_weightsIO,
                   double** base_constantsIO, int* n_base_constantsIO,
                   double** temperature_mapIO, int* n_temperature_mapIO,
                   double** temperature_constantsIO, int* n_temperature_constantsIO,
                   double** record_weightIO, int* n_record_weightIO);

void reallyBuildMatrices(temp_t** data_array, int* n_data_array, int n_n_data_array,
                         int** dates_array, int* n_dates_array, int n_n_dates_array,
                         double* new_spatial_table, int n_new_spatial_table1, int n_new_spatial_table2,
                         float* map, int n_map,
                         int* near_index, int n_near_index,
                         real* t_res, int n_t_res,
                         real* b_res, int n_b_res,
                         real sigma, real sigma_full, bool first, berkeleyAverageOptions* options,

                         double** base_weightsIO, int* n_base_weightsIO,
                         double** base_constantsIO, int* n_base_constantsIO,
                         double** temperature_mapIO, int* n_temperature_mapIO,
                         double** temperature_constantsIO, int* n_temperature_constantsIO,
                         double** record_weightIO, int* n_record_weightIO);

#endif
