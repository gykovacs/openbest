#ifndef _BUILD_SPATIAL_MAP_H_
#define _BUILD_SPATIAL_MAP_H_

#include "openbest-ds/config-ds.h"
#include "openbest-av/berkeleyAverageOptions.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"

void buildSpatialMap(float* weight, int n_weight,
                     float* target_map, int n_target_map1, int n_target_map2,
                     bool* occurance_table, int n_occurance_table1, int n_occurance_table2,
                     int* expand_map, int n_expand_map,
                     double nugget, berkeleyAverageOptions* options,

                     double** spatial_tableIO, int* n_spatial_table1, int* n_spatial_table2,
                     double** coverage_mapIO, int* n_coverage_map1, int* n_coverage_map2);

void computeSpatialMapBlock(int* common, int n_common,
                            int* common_cnts, int n_common_cnts,
                            float* Bi, int n_Bi1, int n_Bi2,
                            bool* occ_fragment, int n_occ_fragment1, int n_occ_fragment2,
                            int* expand_map, int n_expand_map,
                            int* remap, int n_remap,
                            int* remap_s, int n_remap_s,
                            float* weight, int n_weight1, int n_weight2,
                            float* target_map, int n_target_map,
                            int len_M, int len_W,
                            int* index, int n_index,
                            double mix_term,
                            berkeleyAverageOptions* options,

                            float** exports1IO, int* n1_exports1IO, int* n2_exports1IO,
                            float** exports2IO, int* n1_exports2IO, int* n2_exports2IO,
                            float** exports3IO, int* n1_exports3IO, int* n2_exports3IO,
                            int* f, int n_f);

void mldivideWithParitalInverse2(float* M_C, int n_M_C1, int n_M_C2,
                                 float* M_D, int n_M_D1, int n_M_D2,
                                 float* T, int n_T,
                                 float* Ai, int n_Ai1, int n_Ai2,

                                 float** XIO, int* n_X1IO, int* n_X2IO);

#endif
