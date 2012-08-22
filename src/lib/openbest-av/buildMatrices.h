#ifndef _BUILD_MATRICES_H_
#define _BUILD_MATRICES_H_

#include "openbest-ds/printOut.h"
#include "openbest-ds/config-ds.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-av/berkeleyAverageOptions.h"


/**
  * buildMatrices function from the Matlab-BEST code
  * @param data_array array of arrays of data series
  * @param n_data_array length of the arrays in data_array
  * @param n_n_data_array length of data_array
  * @param dates_array array of arrays of time series
  * @param n_dates_array length of the arrays in dates_array
  * @param n_n_dates_array length of dates_array
  * @param new_spatial_table spatial table matrix
  * @param n_new_spatial_table1 rows of new_spatial_table
  * @param n_new_spatial_table2 columns of new_spatial_table
  * @param map map array
  * @param n_map length of map
  * @param near_index near index array
  * @param n_near_index length of near_index
  * @param t_res temperature results
  * @param n_t_res length of temperature results
  * @param b_res baseline results
  * @param n_b_res length of baseline results
  * @param sigma sigma parameter
  * @param sigma_full sigma full parameter
  * @param first logical value about the first iteration
  * @param options berkeleyAverageOptions instance

  * @param base_weightsIO pointer to base weights array
  * @param n_base_weightsIO pointer to the length of base weights array
  * @param base_constantsIO pointer to base constants array
  * @param n_base_constantsIO pointer to the length of base constants array
  * @param temperature_mapIO pointer to temperature_map array
  * @param n_temperature_mapIO pointer to the length of the temperature_map array
  * @param record_weightIO pointer to the record_weight array
  * @param n_record_weightIO pointer to the length of the record_weights array
  */
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

/**
  * reallyBuildMatrices function from the Matlab-BEST code
  * @param data_array array of arrays of data series
  * @param n_data_array length of the arrays in data_array
  * @param n_n_data_array length of data_array
  * @param dates_array array of arrays of time series
  * @param n_dates_array length of the arrays in dates_array
  * @param n_n_dates_array length of dates_array
  * @param new_spatial_table spatial table matrix
  * @param n_new_spatial_table1 rows of new_spatial_table
  * @param n_new_spatial_table2 columns of new_spatial_table
  * @param map map array
  * @param n_map length of map
  * @param near_index near index array
  * @param n_near_index length of near_index
  * @param t_res temperature results
  * @param n_t_res length of temperature results
  * @param b_res baseline results
  * @param n_b_res length of baseline results
  * @param sigma sigma parameter
  * @param sigma_full sigma full parameter
  * @param first logical value about the first iteration
  * @param options berkeleyAverageOptions instance

  * @param base_weightsIO pointer to base weights array
  * @param n_base_weightsIO pointer to the length of base weights array
  * @param base_constantsIO pointer to base constants array
  * @param n_base_constantsIO pointer to the length of base constants array
  * @param temperature_mapIO pointer to temperature_map array
  * @param n_temperature_mapIO pointer to the length of the temperature_map array
  * @param record_weightIO pointer to the record_weight array
  * @param n_record_weightIO pointer to the length of the record_weights array
  */
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
