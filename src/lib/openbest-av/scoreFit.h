#ifndef _SCORE_FIT_H_
#define _SCORE_FIT_H_

#include "openbest-ds/printOut.h"
#include "openbest-ds/config-ds.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-av/berkeleyAverageOptions.h"

/**
  * scoreFit function from the Matlab-BEST code
  * @param data_array array of arrays of data series
  * @param n_data_array lengths of arrays in data_array
  * @param n_n_data_array length of data_array
  * @param dates_array array of arrays of dates series
  * @param n_dates_array lengths of arrays in dates_array
  * @param n_n_dates_array length of dates_array
  * @param t_res temperature results array
  * @param n_t_res length of temperature_results
  * @param b_res baseline results array
  * @param n_b_res length of baseline results array
  * @param new_spatial_table spatial table matrix
  * @param n_new_spatial_table1 rows of new_spatial_table
  * @param n_new_spatial_table2 columns of new_spatial_table
  * @param map_dist map dist vector
  * @param n_map_dist length of map_dist
  * @param near_index vector of nearest indices
  * @param n_near_index length of near_index
  * @param first logical indicator of first iteration
  * @param sigma sigma parameter
  * @param sigma_full sigma_full parameter
  * @param options berkeleyAverageOptions instance
  * @param ssdIO pointer to a double variable to return the ssd value
  * @param base_adjustment_scaleIO pointer to a real* variable to return the base adjustment scale matrix
  * @param n_base_adjustment_scale1IO pointer to an int variable to return the rows of base adjustment scale matrix
  * @param n_base_adjustment_scale2IO pointer to an int variable to return the columns of base adjustment scale matrix
  * @param data_pointer pointer to an int variable to return data_points
  */
void scoreFit(temp_t** data_array, int* n_data_array, int n_n_data_array,
              int** dates_array, int* n_dates_array, int n_n_dates_array,
              real* t_res, int n_t_res,
              real* b_res, int n_b_res,
              double* new_spatial_table, int n_new_spatial_table1, int n_new_spatial_table2,
              float* map_dist, int n_map_dist,
              int* near_index, int n_near_index,
              bool first, double sigma, double sigma_full, berkeleyAverageOptions* options,

              double* ssdIO,
              real** base_adjustment_scaleIO, int* n_base_adjustment_scale1IO, int* n_base_adjustment_scale2IO,
              int* data_points);

/**
  * reallyScoreFit function from the Matlab-BEST code
  * @param data_array array of arrays of data series
  * @param n_data_array lengths of arrays in data_array
  * @param n_n_data_array length of data_array
  * @param dates_array array of arrays of dates series
  * @param n_dates_array lengths of arrays in dates_array
  * @param n_n_dates_array length of dates_array
  * @param t_res temperature results array
  * @param n_t_res length of temperature_results
  * @param b_res baseline results array
  * @param n_b_res length of baseline results array
  * @param new_spatial_table spatial table matrix
  * @param n_new_spatial_table1 rows of new_spatial_table
  * @param n_new_spatial_table2 columns of new_spatial_table
  * @param map_dist map dist vector
  * @param n_map_dist length of map_dist
  * @param near_index vector of nearest indices
  * @param n_near_index length of near_index
  * @param first logical indicator of first iteration
  * @param sigma sigma parameter
  * @param sigma_full sigma_full parameter
  * @param options berkeleyAverageOptions instance
  * @param ssdIO pointer to a double variable to return the ssd value
  * @param base_adjustment_scaleIO pointer to a real* variable to return the base adjustment scale matrix
  * @param n_base_adjustment_scale1IO pointer to an int variable to return the rows of base adjustment scale matrix
  * @param n_base_adjustment_scale2IO pointer to an int variable to return the columns of base adjustment scale matrix
  * @param data_pointer pointer to an int variable to return data_points
  */
void reallyScoreFit(temp_t** data_array, int* n_data_array, int n_n_data_array,
              int** dates_array, int* n_dates_array, int n_n_dates_array,
              real* t_res, int n_t_res,
              real* b_res, int n_b_res,
              double* new_spatial_table, int n_new_spatial_table1, int n_new_spatial_table2,
              float* map_dist, int n_map_dist,
              int* near_index, int n_near_index,
              bool first, double sigma, double sigma_full, berkeleyAverageOptions* options,

              double* ssdIO,
              real** base_adjustment_scaleIO, int* n_base_adjustment_scale1IO, int* n_base_adjustment_scale2IO,
              int* data_points);

#endif
