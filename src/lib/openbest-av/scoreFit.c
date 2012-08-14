#include <float.h>

#include "openbest-av/scoreFit.h"

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
              int* data_points)
{
    int matlabPoolSize= 1;

    if ( matlabPoolSize > 1 )
    {

    }
    else
    {
        reallyScoreFit(data_array, n_data_array, n_n_data_array,
                       dates_array, n_dates_array, n_n_dates_array,
                       t_res, n_t_res,
                       b_res, n_b_res,
                       new_spatial_table, n_new_spatial_table1, n_new_spatial_table2,
                       map_dist, n_map_dist,
                       near_index, n_near_index,
                       first, sigma, sigma_full, options,

                       ssdIO,
                       base_adjustment_scaleIO, n_base_adjustment_scale1IO, n_base_adjustment_scale2IO,
                       data_points);
    }
}



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
              int* data_pointsIO)
{
    double local_outlier_limit;
    double global_outlier_limit;

    if ( options->useIterativeReweighting && options->useOutlierWeighting )
    {
        local_outlier_limit= options->outlierWeightingCutoffMultiplier;
        global_outlier_limit= options->outlierWeightingGlobalCutoffMultiplier;
    }
    else
    {
        local_outlier_limit= FLT_MAX;
        global_outlier_limit= FLT_MAX;
    }

    double ssd= 0;
    int data_points= 0;
    real* bs= copyRA(b_res, n_b_res);
    int n_bs= n_b_res;

    real* base_adjustment_scale= rnalloc(n_b_res*2);
    int n_base_adjustment_scale1= n_b_res;
    int n_base_adjustment_scale2= 2;

    int len_s= n_n_data_array;

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
                for ( k= 0; k < n_new_spatial_table2; ++k )
                {
                    ii= i - j;
                    sp_weight_temp[ii*n_new_spatial_table2 + k]= new_spatial_table[i*n_new_spatial_table2 + k];
                }
            n_sp_weight_temp1= max - j;
            n_sp_weight_temp2= n_new_spatial_table2;
        }
        if ( !first && bs[j] == -FLT_MAX )
            continue;

        // Load data from station
        int* monthnum= copyIA(dates_array[j], n_dates_array[j]);
        int n_monthnum= n_dates_array[j];

        temp_t* data= copyTA(data_array[j], n_data_array[j]);
        int n_data= n_data_array[j];

        bool* fs= bnalloc(n_monthnum);
        for ( i= 0; i < n_monthnum; ++i )
            if ( t_res[monthnum[i]] == -FLT_MAX )
                fs[i]= 1;
            else
                fs[i]= 0;

        k= 0;
        for ( i= 0; i < n_monthnum; ++i )
            if ( !fs[i] )
            {
                monthnum[k]= monthnum[i];
                data[k++]= data[i];
            }
        n_monthnum= k;
        n_data= k;

        if ( n_monthnum == 0 )
            continue;

        if ( !first && options->useOutlierWeighting )
        {
            // Perform outlier adjustments
            // TODO
        }

        temp_t* local_table= copyTA(data, n_data);
        if ( !first && options->localMode )
        {
            // Improve parameter fit by reducing data by the local anomaly field.
            // TODO
        }

        // Add entries corresponding to
        // (spatial correlation_table)*(data(t,x) - baseline(x) - mean_temp(t))

        s= dnalloc(n_monthnum);
        n_s= n_monthnum;
        int idx= j%temp_blocking_size;
        for ( i= 0; i < n_monthnum; ++i )
            s[i]= sp_weight_temp[idx*n_sp_weight_temp2 + (int)(monthnum[i])];

        double* d= dnalloc(n_monthnum);
        int n_d= n_monthnum;
        for ( i= 0; i < n_monthnum; ++i )
            d[i]= local_table[i] - b_res[j] - t_res[monthnum[i]];

        double sum= 0;
        for ( i= 0; i < n_d; ++i )
            sum+= s[i]*d[i]*d[i];
        ssd+= sum;

        sum= 0;
        for ( i= 0; i < n_d; ++i )
            sum+= s[i]*d[i];

        base_adjustment_scale[j*2 + 0]= -2*sum;

        sum= 0;
        for ( i= 0; i < n_d; ++i )
            sum+= s[i];

        base_adjustment_scale[j*2 + 1]= sum;

        data_points= data_points + n_data;
    }

    *ssdIO= ssd;
    *base_adjustment_scaleIO= base_adjustment_scale;
    *n_base_adjustment_scale1IO= n_base_adjustment_scale1;
    *n_base_adjustment_scale2IO= n_base_adjustment_scale2;
    *data_pointsIO= data_points;
}
