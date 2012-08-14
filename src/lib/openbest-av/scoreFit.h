#ifndef _SCORE_FIT_H_
#define _SCORE_FIT_H_

#include "openbest-ds/printOut.h"
#include "openbest-ds/config-ds.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-av/berkeleyAverageOptions.h"

void scoreFit(temp_t** data_array, int* n_data_array, int n_n_data_array,
              real** dates_array, int* n_dates_array, int n_n_dates_array,
              real* t_res, int n_t_res,
              real* b_res, int n_b_res,
              double* new_spatial_table, int n_new_spatial_table1, int n_new_spatial_table2,
              float* map_dist, int n_map_dist,
              int* near_index, int n_near_index,
              bool first, double sigma, double sigma_full, berkeleyAverageOptions* options,

              double* ssdIO,
              real** base_adjustment_scaleIO, int* n_base_adjustment_scale1IO, int* n_base_adjustment_scale2IO,
              int* data_points);

void reallyScoreFit(temp_t** data_array, int* n_data_array, int n_n_data_array,
              real** dates_array, int* n_dates_array, int n_n_dates_array,
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
