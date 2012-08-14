#ifndef _PERFORM_FIT_H_
#define _PERFORM_FIT_H_

#include "openbest-ds/printOut.h"
#include "openbest-ds/config-ds.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-av/berkeleyAverageOptions.h"

performFit(double* base_map, int n_base_map1, int n_base_map2,
           double* base_constants, int n_base_constants,
           double* temperature_map, int n_temperature_map1, int n_temperature_map2,
           double* temperature_constant, int n_temperature_constant,
           double* cross_map, int n_cross_map1, int n_cross_map2,
           real* all_station_mix, int n_all_station_mix,

           real** t_res, int* n_t_res, real** b_res, int* n_b_res);

#endif
