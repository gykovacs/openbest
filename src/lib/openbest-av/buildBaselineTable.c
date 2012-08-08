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

    for ( i= 0; i < n_num_months; ++i )
        printf("%d ", num_months[i]);
    printf("\n");
    getchar();

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
    seti(cnts, len_R, 0);
    for ( k= 0; k < n_expand_map; ++k )
    {
        // Each data point counts as a duplication as the base values is
        // "repeated" according to the number of times it occurs.
        cnts[expand_map[k]]= cnts[expand_map[k]] + num_months[k];
    }



    for ( k= 0; k < n_correlation_table; ++k )
    {
        /*if (!num_months[k] )
            printf("%d ", num_months[k]);*/
        if ( num_months[k] != 0 )
            correlation_table[k*n_correlation_table + k]= (1.0 + (cnts[k]-1.0)*mix_term)/((double)cnts[k]);
        else
            correlation_table[k*n_correlation_table + k]= 0;
    }
    //getchar();

    // Determine baseline mixing values at each grid point
    double* correlation_table_T= dnalloc(n_correlation_table*n_correlation_table);
    double* target_map_T= dnalloc(n_target_map2 * n_target_map1);
    double* base_map= dnalloc(n_correlation_table * n_target_map1);
    if ( correlation_table_T == NULL || target_map_T == NULL || base_map == NULL )
        eprintf("malloc failed in buildBaselineTable\n");
    int n_base_map1= n_target_map1;
    int n_base_map2= n_correlation_table;

    //Check if correlation_table is symmetric
    /*for ( i= 0; i < n_correlation_table; ++i )
        for ( j= 0; j < n_correlation_table; ++j )
            if ( correlation_table[i*n_correlation_table + j] != correlation_table[j*n_correlation_table + i] )
            {
                printf(".");
                fflush(stdout);
            }
    getchar();*/

    tprintf("size of correlation table: %d\n", n_correlation_table);

    for ( i= 0; i < n_correlation_table; ++i )
        for ( j= 0; j < n_correlation_table; ++j )
            correlation_table_T[j*n_correlation_table + i]= correlation_table[i*n_correlation_table + j];

    /*for ( i= 0; i < n_correlation_table; ++i )
    {
        for ( j= 0; j < n_correlation_table; ++j )
        {
            printf("%f ", correlation_table[i*n_correlation_table + j]);
        }
        printf("\n");
    }
    getchar();*/
    tprintf("solving linear equation systems with A: (%dx%d), b: (%d)\n", n_correlation_table, n_correlation_table, n_target_map2);
    for ( j= 0; j < n_target_map1; ++j )
        for ( i= 0; i < n_target_map2; ++i )
            target_map_T[i*n_target_map1 + j]= target_map[j*n_target_map2 + i];
    /*for ( i= 0; i < n_target_map1; ++i )
    {*/


    FILE* ctable= fopen("correlation_table.txt", "w");
    for ( i= 0; i < n_correlation_table; ++i )
    {
        for ( j= 0; j < n_correlation_table; ++j )
            fprintf(ctable, "%g ", correlation_table_T[i*n_correlation_table + j]);
        fprintf(ctable, "\n");
    }
    fclose(ctable);
    FILE* targetmap= fopen("targetmap.txt", "w");
    for ( i= 0; i < n_target_map1; ++i )
    {
        for ( j= 0; j < n_target_map2; ++j )
        {
            fprintf(targetmap, "%g ", target_map_T[j*n_target_map1 + i]);
        }
        fprintf(targetmap, "\n");
    }
    fclose(targetmap);

        //printf("%d ", i); fflush(stdout);
        solveLinEqHD(correlation_table_T, n_correlation_table, n_correlation_table, target_map_T, n_target_map1, base_map);
        /*for ( j= 0; j < n_target_map2; ++j )
            printf("%f ", base_map[i*n_target_map2 + j]);
        printf("\n");
        getchar();*/
    //}
    tprintf("solutions finished\n");
    /*for ( j= 0; j < n_target_map2; ++j )
    {
        for ( i= 0; i < n_target_map1; ++i )
            printf("%f ", base_map[i*n_target_map2 + j]);
        printf("\n");
    }
    getchar();*/
    double* base_map2= dnalloc(n_base_map1*n_base_map2);
    for ( i= 0; i < n_base_map1; ++i )
        for ( j= 0; j < n_base_map2; ++j )
            base_map2[j*n_base_map1 + i]= base_map[i*n_base_map2 + j];
    base_map= base_map2;

    for ( i= 0; i < n_base_map1*n_base_map2; ++i )
        if ( base_map[i] != base_map[i] )
        {
            printf("%d ", i); fflush(stdout);
        }

    for ( i= 0; i < n_correlation_table; ++i )
        printf("%f ", base_map[i]);
    printf("\n");

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

    printf("cov_map: \n");
    for ( i= 0; i < n_cov_map; ++i )
        printf("%f ", cov_map[i]);
    printf("\n");

    // Some additional diagnostic parameters
    tprintf("base_map: %d,%d; mask: %d\n", n_base_map1, n_base_map2, n_mask);
    double completeness= 0;
    double a= 0, sum_mask= 0;
    for ( i= 0; i < n_base_map1; ++i )
        for ( j= 0; j < n_base_map2; ++j )
        {
            a+= mask[i]*base_map[i*n_base_map2 + j];
        }
    for ( i= 0; i < n_mask; ++i )
        sum_mask+= mask[i];
    tprintf("%f %f\n", a, sum_mask);
    completeness= a/sum_mask;

    tprintf("completeness: %f\n", completeness);

    // Limit empirical region for increased stability
    if ( options->limitEmpiricalBaselineRegion )
        for ( i= 0; i < n_cov_map; ++i )
            if ( cov_map[i] >= options->empiricalBaselineRegionCutoff )
                mask[i]= 0;

    // Perform numerical integral to generate final baseline mixing coefficients
    tprintf("Perform numerical integral to generate final baseline mixing coefficients\n");

    dea();
    real* baseline_weights= rnalloc(n_base_map2);
    int n_baseline_weights= n_base_map2;
    for ( i= 0; i < n_base_map2; ++i )
    {
        baseline_weights[i]= 0;
        for ( j= 0; j < n_base_map1; ++j )
        {
            baseline_weights[i]+= mask[j]*baseline_weights[j*n_base_map2 + i];
        }
    }

    dea();
    sum_mask= 0;
    for ( i= 0; i < n_mask; ++i )
        sum_mask+= mask[i];
    for ( i= 0; i < n_baseline_weights; ++i )
        baseline_weights[i]/= sum_mask;

    deb();
    for ( i= 0; i < n_baseline_weights; ++i )
        baseline_weights[i]/= cnts[i];

    dec();
    real* baseline_weights2= rnalloc(n_expand_map);
    for ( i= 0; i < n_expand_map; ++i )
        baseline_weights2[i]= base_map[expand_map[i]]*num_months[i];
    free(baseline_weights);
    baseline_weights= baseline_weights2;
    n_baseline_weights= n_expand_map;

    for ( i= 0; i < n_baseline_weights; ++i )
        printf("%f ", baseline_weights[i]);
    printf("\n");

    double* target2= dnalloc(n_target_map2);
    set(target2, n_target_map2, 0);
    for ( i= 0; i < n_target_map1; ++i )
        for ( j= 0; j < n_target_map2; ++j )
            target2[j]+= target_map[i*n_target_map2 + j];

    tprintf("computing global completeness: correlation_table: %d,%d; target2: %d\n", n_correlation_table, n_correlation_table, n_target_map2);

    for ( i= 0; i < n_correlation_table; ++i )
        for ( j= 0; j < n_correlation_table; ++j )
            correlation_table_T[j*n_correlation_table + i]= correlation_table[i*n_correlation_table + j];

    double* tmpM= dnalloc(n_correlation_table);


    solveLinEq(correlation_table_T, n_correlation_table, n_correlation_table, target2, tmpM);
    double global_completeness= 0;
    for ( i= 0; i < n_correlation_table; ++i )
    {
        printf("%f ", tmpM[i]);
        global_completeness+= tmpM[i];
    }
    printf("\n");
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

    tprintf("End of Build Baseline Table\n");
}
