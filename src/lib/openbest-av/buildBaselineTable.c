#include "openbest-av/buildBaselineTable.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-ds/printOut.h"
#include "openbest-ds/config-ds.h"
#include "openbest-av/equationSolvers.h"

void buildBaselineTable(float* correlation_table, int n_correlation_table,
                        float* target_map, int n_target_map1, int n_target_map2,
                        bool* occurance_table, int n_occurance_table1, int n_occurance_table2,
                        int* expand_map, int n_expand_map, float nugget,
                        real* mask, int n_mask, berkeleyAverageOptions* options,
                        real* sites_elev, int n_sites_elev, real* sites_lat, int n_sites_lat,
                        real* map_elev, int n_map_elev, real* map_lat, int n_map_lat,

                        real** baseline_weightsIO, int* n_baseline_weightsIO,
                        double* completenessIO, double* global_completenessIO,
                        real** cov_mapIO, int* n_cov_mapIO,
                        double** base_mapIO, int* n_base_map1IO, int* n_base_map2IO,
                        real** base_mapping_sitesIO, real** base_mapping_mapIO)
{
    tprintf("Begin of Build Baseline Table\n");
    tprintf("correlation_table: %d, target_map: %d,%d, occurance_table: %d,%d, expand_map: %d\n", n_correlation_table, n_target_map1, n_target_map2, n_occurance_table1, n_occurance_table2, n_expand_map);

    int i, j, k;

    int* num_months= inalloc(n_occurance_table1);
    seti(num_months, n_occurance_table1, 0);
    for ( i= 0; i < n_occurance_table1; ++i )
        for ( j= 0; j < n_occurance_table2; ++j )
            num_months[i]+= occurance_table[i*n_occurance_table2 + j];
    int n_num_months= n_occurance_table1;

    int len_R= n_correlation_table;

    if ( options->clusterMode )
    {
        //TODO
    }
    else
    {
        //converions of correlation_table and target_map to double representation
    }

    // Total mix
    double mix_term= 1.0 - nugget;
    int* cnts= inalloc(len_R);
    int n_cnts= len_R;
    seti(cnts, len_R, 0);

    // Each data point counts as a duplication as the base values is
    // "repeated" according to the number of times it occurs.
    for ( k= 0; k < n_expand_map; ++k )
        cnts[expand_map[k]]+= num_months[k];

    for ( k= 0; k < n_correlation_table; ++k )
        correlation_table[k*n_correlation_table + k]= (1.0 + (cnts[k]-1.0)*mix_term)/((double)cnts[k]);

    // Determine baseline mixing values at each grid point
    double* correlation_table_T= transposeMatrixNFloatD(correlation_table, n_correlation_table, n_correlation_table);
    double* target_map_T= transposeMatrixNFloatD(target_map, n_target_map1, n_target_map2);
    double* base_map= NULL;
    double* base_mapT= dnalloc(n_correlation_table * n_target_map1);

    int n_base_map1= n_target_map1;
    int n_base_map2= n_correlation_table;
    int n_base_mapT1= n_correlation_table;
    int n_base_mapT2= n_target_map1;

    tprintf("size of correlation table: %d\n", n_correlation_table);

    tprintf("solving linear equation systems with A: (%dx%d), b: (%d)\n", n_correlation_table, n_correlation_table, n_target_map2);

    solveLinEqHD(correlation_table_T, n_correlation_table, n_correlation_table, target_map_T, n_target_map1, base_mapT);

    base_map= transposeMatrixND(base_mapT, n_base_mapT1, n_base_mapT2);

    if ( options->clusterMode )
    {
        // TODO
    }

    real* cov_map= rnalloc(n_base_map1);
    int n_cov_map= n_base_map1;
    for ( i= 0; i < n_base_map1; ++i )
    {
        cov_map[i]= 0;
        for ( j= 0; j < n_base_map2; ++j )
            cov_map[i]+= base_map[i*n_base_map2 + j];
    }

    // Some additional diagnostic parameters
    double completeness= 0;
    double a= 0, sum_mask= 0;
    for ( i= 0; i < n_base_map1; ++i )
        for ( j= 0; j < n_base_map2; ++j )
            a+= mask[i]*base_map[i*n_base_map2 + j];
    for ( i= 0; i < n_mask; ++i )
        sum_mask+= mask[i];

    completeness= a/sum_mask;

    tprintf("completeness: %f\n", completeness);

    // Limit empirical region for increased stability
    if ( options->limitEmpiricalBaselineRegion )
        for ( i= 0; i < n_cov_map; ++i )
            if ( !(cov_map[i] >= options->empiricalBaselineRegionCutoff) )
                mask[i]= 0;

    // Perform numerical integral to generate final baseline mixing coefficients
    tprintf("Perform numerical integral to generate final baseline mixing coefficients\n");

    real* baseline_weights= rnalloc(n_base_map2);
    int n_baseline_weights= n_base_map2;
    for ( i= 0; i < n_base_map2; ++i )
    {
        baseline_weights[i]= 0;
        for ( j= 0; j < n_base_map1; ++j )
            baseline_weights[i]+= mask[j]*base_map[j*n_base_map2 + i];
    }



    sum_mask= 0;
    for ( i= 0; i < n_mask; ++i )
        sum_mask+= mask[i];
    for ( i= 0; i < n_baseline_weights; ++i )
        baseline_weights[i]/= sum_mask;

    for ( i= 0; i < n_baseline_weights; ++i )
        baseline_weights[i]/= cnts[i];



    real* baseline_weights2= rnalloc(n_expand_map);
    for ( i= 0; i < n_expand_map; ++i )
        baseline_weights2[i]= baseline_weights[expand_map[i]]*num_months[i];
    free(baseline_weights);
    baseline_weights= baseline_weights2;
    n_baseline_weights= n_expand_map;

    double* target2= dnalloc(n_target_map2);
    setd(target2, n_target_map2, 0);
    for ( i= 0; i < n_target_map1; ++i )
        for ( j= 0; j < n_target_map2; ++j )
            target2[j]+= target_map[i*n_target_map2 + j];

    for ( i= 0; i < n_target_map2; ++i )
        target2[i]= target2[i]/(double)(n_target_map1);

    tprintf("computing global completeness: correlation_table: %d,%d; target2: %d\n", n_correlation_table, n_correlation_table, n_target_map2);

    free(correlation_table_T);
    correlation_table_T= transposeMatrixNFloatD(correlation_table, n_correlation_table, n_correlation_table);

    double* tmpM= dnalloc(n_correlation_table);

    solveLinEqHD(correlation_table_T, n_correlation_table, n_correlation_table, target2, 1, tmpM);
    double global_completeness= 0;
    for ( i= 0; i < n_correlation_table; ++i )
        global_completeness+= tmpM[i];

    tprintf("global completeness: %f\n", global_completeness);

    if ( options->clusterMode )
    {
        // TODO
    }

    real* base_mapping_sites= NULL;
    real* base_mapping_map= NULL;
    if ( options->fullBaselineMapping )
    {
        // TODO
    }

    *baseline_weightsIO= baseline_weights;
    *n_baseline_weightsIO= n_baseline_weights;
    *completenessIO= completeness;
    *global_completenessIO= global_completeness;
    *cov_mapIO= cov_map;
    *n_cov_mapIO= n_cov_map;
    *base_mapIO= base_map;
    *n_base_map1IO= n_base_map1;
    *n_base_map2IO= n_base_map2;
    if ( base_mapping_sitesIO != NULL )
    *base_mapping_sitesIO= base_mapping_sites;
    if ( base_mapping_mapIO != NULL )
    *base_mapping_mapIO= base_mapping_map;

    if ( options->fullBaselineMapping )
    {
        // TODO
    }

    tprintf("End of Build Baseline Table\n");
}
