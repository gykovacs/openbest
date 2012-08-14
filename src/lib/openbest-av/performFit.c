#include <float.h>

#include "openbest-av/performFit.h"
#include "openbest-av/equationSolvers.h"

performFit(double* base_map, int n_base_map1, int n_base_map2,
           double* base_constants, int n_base_constants,
           double* temperature_map, int n_temperature_map1, int n_temperature_map2,
           double* temperature_constant, int n_temperature_constant,
           double* cross_map, int n_cross_map1, int n_cross_map2,
           real* all_station_mix, int n_all_station_mix,

           real** t_resIO, int* n_t_resIO, real** b_resIO, int* n_b_resIO)
{
    tprintf("Begin of Perform Fit\n");

    int pool_size= 1;

    int i, j, k;

    tprintf("base_map: %d,%d; all_station_mix: %d\n", n_base_map1, n_base_map2, n_all_station_mix);

    // Remove baseline ambiguity by setting global baseline average to zero
    double* base_mix= dnalloc(n_base_map2);
    int n_base_mix= n_base_map2;
    for ( i= 0; i < n_base_map2; ++i )
    {
        base_mix[i]= 0;
        for ( j= 0; j < n_base_map1; ++j )
            base_mix[i]+= all_station_mix[j]*base_map[j*n_base_map2 + i];
    }

    dea();

    double base_mix_constant= 0;
    for ( i= 0; i < n_base_constants; ++i )
        base_mix_constant+= base_constants[i] * all_station_mix[i];

    dea();
    double* cross_mapT= transposeMatrixND(cross_map, n_cross_map1, n_cross_map2);
    int n_cross_mapT1= n_cross_map2;
    int n_cross_mapT2= n_cross_map1;

    dek();
    tprintf("cross_mapT: %d,%d; base_map: %d,%d\n", n_cross_mapT1, n_cross_mapT2, n_base_map1, n_base_map2);
    double* mix_map;
    int n_mix_map1;
    int n_mix_map2;

    matrixMultiplicationND(cross_mapT, n_cross_mapT1, n_cross_mapT2,
                           base_map, n_base_map1, n_base_map2,
                           &mix_map, &n_mix_map1, &n_mix_map2);

    deb();
    double* tmpa;
    int n_tmpa1;
    int n_tmpa2;
    matrixSubtractND(temperature_map, n_temperature_map1, n_temperature_map2,
                     mix_map, n_mix_map1, n_mix_map2,
                     &tmpa, &n_tmpa1, &n_tmpa2);

    dec();
    double* temperature_map_mix;
    int n_temperature_map_mix1;
    int n_temperature_map_mix2;
    matrixSubtractVectorND(tmpa, n_tmpa1, n_tmpa2,
                           base_mix, 1, n_base_mix,
                           &temperature_map_mix, &n_temperature_map_mix1, &n_temperature_map_mix2);

    ded();
    free(tmpa);
    matrixMultiplicationByVector(cross_mapT, n_cross_mapT1, n_cross_mapT2,
                                 base_constants, n_base_constants,
                                 &tmpa, &n_tmpa1);

    dee();
    double* temperature_constant_mix= dnalloc(n_tmpa1);
    int n_temperature_constant_mix= n_tmpa1;

    for ( i= 0; i < n_tmpa1; ++i )
        temperature_constant_mix[i]= temperature_constant[i] - tmpa[i] - base_mix_constant;

    int len_t= n_temperature_constant;

    def();
    // Eliminate any entries that are unconstrained
    // gykovacs implemented it using f to contain the complementer
    int* f= inalloc(n_temperature_map_mix1);
    int n_f= 0;
    double sum;
    for ( i= 0; i < n_temperature_map_mix2; ++i )
    {
        sum= 0;
        for ( j= 0; j < n_temperature_map_mix1; ++j )
            sum+= temperature_map_mix[j*n_temperature_map_mix2 + i];
        if ( sum != 0 )
            f[n_f++]= i;
    }

    deg();
    double* temperature_map_mix2= dnalloc(n_f*n_f);

    for ( i= 0; i < n_f; ++i )
    {
        for ( j= 0; j < n_f; ++j )
        {
            temperature_map_mix2[i*n_f + j]= temperature_map_mix[f[i]*n_temperature_map_mix2 + f[j]];
        }
    }
    free(temperature_map_mix);
    temperature_map_mix= temperature_map_mix2;
    n_temperature_map_mix1= n_f;
    n_temperature_map_mix2= n_f;

    deh();
    for ( i= 0; i < n_f; ++i )
        temperature_constant_mix[i]= temperature_constant_mix[f[i]];
    n_temperature_constant_mix= n_f;

    int* new_map= inalloc(len_t);
    for ( i= 0; i < len_t; ++i )
        new_map[i]= i;
    for ( i= 0; i < n_f; ++i )
        new_map[i]= new_map[f[i]];
    int n_new_map= n_f;

    dei();
    while ( conditionNumberEstimateD(temperature_map_mix, n_temperature_map_mix1, n_temperature_map_mix2) > 1e6 )
    {
        //Remove first line from temperature_map_mix
        for ( i= n_temperature_map_mix2; i < n_temperature_map_mix1*n_temperature_map_mix2; ++i )
            temperature_map_mix[i - n_temperature_map_mix2]= temperature_map_mix[i];
        --n_temperature_map_mix1;
        //Remove first columns from temperature_map_mix
        j= 0;
        for ( i= 0; i < n_temperature_map_mix1*n_temperature_map_mix2; ++i )
            if ( i % n_temperature_map_mix2 != 0 )
                temperature_map_mix[j++]= temperature_map_mix[i];
        --n_temperature_map_mix2;

        for ( i= 1; i < n_temperature_constant_mix; ++i )
            temperature_constant_mix[i-1]= temperature_constant_mix[i];
        --n_temperature_constant_mix;

        for ( i= 1; i < n_new_map; ++i )
            new_map[i-1]= new_map[i];
        --n_new_map;
    }

    // Use parallel processing if available
    if ( 0 && pool_size > 1 )
    {
        // TODO
    }

    dej();
    // Solve for temperature series
    double* t_fit= dnalloc(n_temperature_map_mix2);
    int n_t_fit= n_temperature_map_mix2;
    solveLinEqHD(temperature_map_mix, n_temperature_map_mix1, n_temperature_map_mix2, temperature_constant_mix, 1, t_fit);

    real* t_res= rnalloc(len_t);
    int n_t_res= len_t;
    for ( i= 0; i < len_t; ++i )
        t_res[i]= -FLT_MAX;

    if ( 0 && pool_size > 1 )
    {
        // TODO
    }
    else
    {
        for ( i= 0; i < n_new_map; ++i )
            t_res[new_map[i]]= t_fit[i];
    }

    dek();
    // Compute baseline values
    double* base_map2= dnalloc(n_base_map1 * n_new_map);
    for ( i= 0; i < n_base_map1; ++i )
        for ( j= 0; j < n_new_map; ++j )
            base_map2[i*n_new_map + j]= base_map[i*n_base_map2 + new_map[j]];
    int n_base_map21= n_base_map1;
    int n_base_map22= n_new_map;
    double* t_res2= dnalloc(n_new_map);
    for ( i= 0; i < n_new_map; ++i )
        t_res2[i]= t_res[new_map[i]];
    int n_t_res2= n_new_map;

    dea();
    double* tmpb;
    int n_tmpb;
    matrixMultiplicationByVector(base_map2, n_base_map22, n_base_map21, t_res2, n_new_map, &tmpb, &n_tmpb);

    deb();
    real* b_res= rnalloc(n_base_constants);
    int n_b_res= n_base_constants;

    for ( i= 0; i < n_b_res; ++i )
        b_res[i]= base_constants[i] - tmpb[i];

    dec();
    // Residual zeroing, due to round off error, etc.
    double asmbr= 0;
    for ( i= 0; i < n_b_res; ++i )
        asmbr+= all_station_mix[i]*b_res[i];

    ded();
    for ( i= 0; i < n_t_res; ++i )
        t_res[i]+= asmbr;
    for ( i= 0; i < n_b_res; ++i )
        b_res[i]-= asmbr;

    *t_resIO= t_res;
    *b_resIO= b_res;
    *n_t_resIO= n_t_res;
    *n_b_resIO= n_b_res;

    tprintf("End of Perform Fit\n");
}
