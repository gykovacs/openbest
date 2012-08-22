#ifndef _PERFORM_FIT_H_
#define _PERFORM_FIT_H_

#include "openbest-ds/printOut.h"
#include "openbest-ds/config-ds.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-av/berkeleyAverageOptions.h"

/**
  * performFit function from the Matlab-BEST code
  * @param base_map base_map matrix
  * @param n_base_map1 rows of base_map
  * @param n_base_map2 columns of base_map
  * @param base_constants base_constants array
  * @param n_base_constants length of array base_constants
  * @param temperature_map temperature map matrix
  * @param n_temperature_map1 rows of temperature_map
  * @param n_temperature_map2 columns of temperature map
  * @param temperature_constant array
  * @param n_temperature_constant length of vector temperature_constant
  * @param cross_map cross_map matrix
  * @param n_cross_map1 rows of matrix cross_map
  * @param n_cross_map2 columns of matrix cross_map
  * @param all_station_mix array all_station_mix
  * @param n_all_station_mix length of array all_station_mix
  * @param t_res pointer to a real* variable for temperature output
  * @param n_t_res pointer to an int variable for the length of t_res
  * @param b_res pointer to a real* variable for baseline output
  * @param n_b_res pointer to an int variable for the length of b_res
  */
void performFit(double* base_map, int n_base_map1, int n_base_map2,
           double* base_constants, int n_base_constants,
           double* temperature_map, int n_temperature_map1, int n_temperature_map2,
           double* temperature_constant, int n_temperature_constant,
           double* cross_map, int n_cross_map1, int n_cross_map2,
           real* all_station_mix, int n_all_station_mix,

           real** t_res, int* n_t_res, real** b_res, int* n_b_res);

#endif
