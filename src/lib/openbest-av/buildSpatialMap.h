#ifndef _BUILD_SPATIAL_MAP_H_
#define _BUILD_SPATIAL_MAP_H_

#include "openbest-ds/config-ds.h"
#include "openbest-av/berkeleyAverageOptions.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"

/**
  * buildSpatialMap from the Matlab-BEST code
  * @param weight pointer to the weight array
  * @param n_weight length of the weight array
  * @param target_map pointer to the target_map
  * @param n_target_map1 rows of the target_map
  * @param n_target_map2 columns of the target_map
  * @param occurance_table occurance table
  * @param n_occurance_table1 rows of the occurance table
  * @param n_occurance_table2 columns of the occurance_table
  * @param expand_map expand map array
  * @param n_expand_map length of expand map
  * @param nugget nugget value
  * @param options berkeleyAverageOptions instance
  * @param spatial_tableIO pointer to array spatial table
  * @param n_spatial_table1 pointer to the number of rows in spatial_table
  * @param n_spatial_table2 pointer to the number of columns in spatial_table
  * @param coverage_mapIO pointer to array coverage_map
  * @param n_coverage_map1 pointer to the number of rows in coverage map
  * @param n_coverage_map2 pointer to the number of columns in coverage map
  */
void buildSpatialMap(float* weight, int n_weight,
                     float* target_map, int n_target_map1, int n_target_map2,
                     bool* occurance_table, int n_occurance_table1, int n_occurance_table2,
                     int* expand_map, int n_expand_map,
                     double nugget, berkeleyAverageOptions* options,

                     double** spatial_tableIO, int* n_spatial_table1, int* n_spatial_table2,
                     double** coverage_mapIO, int* n_coverage_map1, int* n_coverage_map2);

/**
  * computeSpatialMapBlock function from the Matlab-BEST code (do not call directly!)
  * @param common common array
  * @param n_common length of array common
  * @param common_cnts common counts array
  * @param n_common_cnts length of array common_cnts
  * @param occ_fragment occurance table fragment
  * @param n_occ_fragment1 number of rows in occ_fragment
  * @param n_occ_fragment2 number of columns in occ_fragment
  * @param expand_map expand map array
  * @param n_expand_map length of expand_map
  * @param remap remap array
  * @param n_remap length of remap
  * @param remap_s remap_s array
  * @param n_remap_s length of remap_s array
  * @param weight weight matrix
  * @param n_weight1 number of rows in weight
  * @param n_weight2 number of columns in weight
  * @param target_map target_map matrix
  * @param n_target_map1 number of rows in target_map
  * @param n_target_map2 number of columns in target_map
  * @param len_M see Matlab code
  * @param len_W see Matlab code
  * @param mix_term mix_term value
  * @param options berkeleyAverageOptions instance
  * @param exports1IO see Matlab code
  * @param n1_exports1IO number of rows in exports1
  * @param n2_exports2IO number of columns in exports1
  * @param exports2IO see Matlab code
  * @param n1_exports2IO number of rows in exports2
  * @param n2_exports2IO number of columns in exports2
  * @param exports3IO see Matlab code
  * @param n1_exports3IO number of rows in exports3
  * @param n2_exports3IO number of columns in exports3
  */
void computeSpatialMapBlock(int* common, int n_common,
                            int* common_cnts, int n_common_cnts,
                            float* Bi, int n_Bi1, int n_Bi2,
                            bool* occ_fragment, int n_occ_fragment1, int n_occ_fragment2,
                            int* expand_map, int n_expand_map,
                            int* remap, int n_remap,
                            int* remap_s, int n_remap_s,
                            float* weight, int n_weight1, int n_weight2,
                            real* target_map, int n_target_map1, int n_target_map2,
                            int len_M, int len_W,
                            int* index, int n_index,
                            double mix_term,
                            berkeleyAverageOptions* options,
                            float** exports1IO, int* n1_exports1IO, int* n2_exports1IO,
                            float** exports2IO, int* n1_exports2IO, int* n2_exports2IO,
                            float** exports3IO, int* n1_exports3IO, int* n2_exports3IO,
                            int* f, int n_f);

/**
  * mldivideWithPartialInverse2 - see Matlab-BEST code
  * @param M_C see Matlab code
  * @param n_M_C1 rows of M_C
  * @param n_M_C2 columns of M_C
  * @param M_D see Matlab code
  * @param n_M_D1 rows of M_D
  * @param n_M_D2 columns of M_D
  * @param T see Matlab code
  * @param n_T length of T
  * @param Ai see Matlab code
  * @param n_Ai1 rows of Ai
  * @param n_Ai2 columns of Ai
  * @param XIO result matrix
  * @param n_X1IO pointer to the rows of XIO
  * @param n_X2IO pointer to the columns of XIO
  */
void mldivideWithParitalInverse2(float* M_C, int n_M_C1, int n_M_C2,
                                 float* M_D, int n_M_D1, int n_M_D2,
                                 float* T, int n_T,
                                 float* Ai, int n_Ai1, int n_Ai2,

                                 float** XIO, int* n_X1IO, int* n_X2IO);

/**
  * invWithPartial2 - see Matlab-Best code
  * @param M_B see Matlab code
  * @param n_M_B1 rows of M_B
  * @param n_M_B2 columns of M_B
  * @param M_D see Matlab code
  * @param n_M_D1 rows of M_D
  * @param n_M_D2 columns of M_D
  * @param Ai see Matlab code
  * @param n_Ai1 rows of Ai
  * @param n_Ai2 columns of Ai
  * @param res result matrix
  * @param n_res1 pointer to the rows of res
  * @param n_res2 pointer to the columns of res
  */
void invWithPartial2(float* M_B, int n_M_B1, int n_M_B2,
                     float* M_D, int n_M_D1, int n_M_D2,
                     float* Ai, int n_Ai1, int n_Ai2,
                     float** res, int* n_res1, int* n_res2);

#endif
