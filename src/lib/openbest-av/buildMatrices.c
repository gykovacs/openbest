#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "openbest-av/buildMatrices.h"

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
                   double** record_weightIO, int* n_record_weightIO)
{
    tprintf("Begin of Build Matrices\n");

    int matlabPoolSize= 1;

    if ( matlabPoolSize > 1 )
    {
        // TODO
    }
    else
    {
        reallyBuildMatrices(data_array, n_data_array, n_n_data_array,
                            dates_array, n_dates_array, n_n_dates_array,
                            new_spatial_table, n_new_spatial_table1, n_new_spatial_table2,
                            map, n_map,
                            near_index, n_near_index,
                            t_res, n_t_res,
                            b_res, n_b_res,
                            sigma, sigma_full, first, options,

                            base_weightsIO, n_base_weightsIO,
                            base_constantsIO, n_base_constantsIO,
                            temperature_mapIO, n_temperature_mapIO,
                            temperature_constantsIO, n_temperature_constantsIO,
                            record_weightIO, n_record_weightIO);
    }

    tprintf("End of Build Matrices\n");
}

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
                         double** record_weightIO, int* n_record_weightIO)
{
    // Helper function used to efficiently divide the solution into parallel work blocks
    // for parallel processing.

    double local_outlier_limit;
    double global_outlier_limit;
    if ( options->useIterativeReweighting )
        if ( options->useOutlierWeighting )
        {
            local_outlier_limit= options->outlierWeightingCutoffMultiplier;
            global_outlier_limit= options->outlierWeightingCutoffMultiplier;
        }

    int len_t= n_new_spatial_table2;
    int len_s= n_new_spatial_table1;

    // Prepare matrix equation data holders
    double* base_weights= dnalloc(len_s);
    int n_base_weights= len_s;
    setd(base_weights, n_base_weights, 0);

    double* base_constants= dnalloc(len_s);
    int n_base_constants= len_s;
    setd(base_constants, n_base_constants, 0);

    double* temperature_map= dnalloc(len_t*len_t);
    int n_temperature_map, n_temperature_map1, n_temperature_map2;
    n_temperature_map= n_temperature_map1= n_temperature_map2= len_t;
    setd(temperature_map, n_temperature_map*n_temperature_map, 0);

    double* temperature_constant= dnalloc(len_t);
    int n_temperature_constant= len_t;
    setd(temperature_constant, n_temperature_constant, 0);

    // Loop over stations

    double* record_weight= dnalloc(len_s);
    int n_record_weight= len_s;
    setd(record_weight, n_record_weight, 0);

    int temp_blocking_size= 2500;

    int i, j, k;

    double* sp_weight_temp;
    int n_sp_weight_temp1;
    int n_sp_weight_temp2;

    double* s;
    int n_s1;
    int n_s2;
    int n_s;

    for ( j= 0; j < len_s; ++j )
    {
        if ( j % temp_blocking_size == 0 )
        {
            int max= (j + temp_blocking_size) < n_new_spatial_table1 ? (j + temp_blocking_size) : n_new_spatial_table1;
            sp_weight_temp= dnalloc(n_new_spatial_table2 * (max - j));

            int ii;
            for ( i= j; i < max; ++i )
            {
                ii= i - j;
                for ( k= 0; k < n_new_spatial_table2; ++k )
                {
                    sp_weight_temp[ii*n_new_spatial_table2 + k]= new_spatial_table[i*n_new_spatial_table2 + k];
                }
            }
            n_sp_weight_temp1= max - j;
            n_sp_weight_temp2= n_new_spatial_table2;
        }

        //dea();
        if ( !first && b_res[j] == -FLT_MAX )
            continue;

        // Load data from station
        int* monthnum= dates_array[j];
        int n_monthnum= n_dates_array[j];

        temp_t* data= data_array[j];
        int n_data= n_data_array[j];

        if ( !first )
        {
            // TODO
        }

        real* outlier_weight= rnalloc(n_data);
        int n_outlier_weight= n_data;
        set(outlier_weight, n_outlier_weight, 1);

        if ( !first && options->useOutlierWeighting )
        {
            // Perform outlier adjustments
            // TODO
        }

        temp_t* local_table;
        int n_local_table;
        if ( !first && options->localMode )
        {
            //Improve paramter fit by reducing data by the local anomaly field.
            // TODO
        }
        else
        {
            local_table= data;
            n_local_table= n_data;
        }

        // Add entries corresponding to
        // (spatial correlation_table)*(data(t,x) - baseline(x) - mean_temp(t))

        s= dnalloc(n_monthnum);
        n_s= n_monthnum;

        int idx= j%temp_blocking_size;
        for ( i= 0; i < n_monthnum; ++i )
            s[i]= sp_weight_temp[idx*n_sp_weight_temp2 + monthnum[i]];

        //def();
        double sum= 0;
        for ( i= 0; i < n_s; ++i )
            sum+= fabs(s[i]) * outlier_weight[i];
        record_weight[j]= sum;

        //deg();
        base_weights[j]= base_weights[j] + n_s;

        sum= 0;
        for ( i= 0; i < n_local_table; ++i )
            sum+= local_table[i];

        //ded();
        base_constants[j]= base_constants[j] + sum;

        for ( i= 0; i < n_monthnum; ++i )
            temperature_map[(int)(monthnum[i])*n_temperature_map + (int)(monthnum[i])]+= s[i];

        for ( i= 0; i < n_monthnum; ++i )
            temperature_constant[(int)(monthnum[i])]+= s[i]*data[i];
    }

    *record_weightIO= record_weight;
    *base_weightsIO= base_weights;
    *base_constantsIO= base_constants;
    *temperature_mapIO= temperature_map;
    *temperature_constantsIO= temperature_constant;
    *n_record_weightIO= n_record_weight;
    *n_base_weightsIO= n_base_weights;
    *n_base_constantsIO= n_base_constants;
    *n_temperature_mapIO= n_temperature_map;
    *n_temperature_constantsIO= n_temperature_constant;
}
