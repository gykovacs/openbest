#include "openbest-av/berkeleyAverageCore.h"
#include "openbest-ds/primaryKeyTables.h"
#include "openbest-ds/init-ds.h"
#include "openbest-ds/config-ds.h"
#include "openbest-ds/temperatureGlobals.h"
#include "openbest-ds/printOut.h"
#include "openbest-ds/stationElement2.h"
#include "openbest-ds/stationSite2.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"

void berkeleyAverageCore(stationElement2p** seIO, int* n_seIO, stationSite2p** ssIO, int* n_ssIO, berkeleyAverageOptions* options)
{
    int i, j, k;

    stationElement2p* se= *seIO;
    stationSite2p* ss= *ssIO;
    int n_se= *n_seIO;
    int n_ss= *n_ssIO;

    tprintf("Berkeley Average Core called with %d time series\n", n_se);

    for ( i= 0; i < n_se; ++i )
    {
        if ( !isMonthly(se[i]) )
        {
            eprintf("Only monthly time series are supported\n");
            return;
        }
        if ( isMultiValued(se[i]) )
        {
            eprintf("Only single-valued time series are supported\n");
            return;
        }
    }

    int min_stations= options->minStations;
    int min_months= options->minMonths;

    float precision_target= 0;
    int max_loop;

    if ( options.useIterativeReweighting )
    {
        precision_target= options->precisionTarget;
        max_loop= options->maxIterations;
    }
    else
        max_loop= 1;

    // TODO: options->useSeed

    tprintf("Beginning Berkeley Average Core Process\n");

    // Convert locations to geoPoint format
    geoPoint2** locations= (geoPoint2**)malloc(sizeof(geoPoint2*)*n_ss);
    for ( i= 0; i < n_ss; ++i )
        locations[i]= ss[i]->location;

    // Time range for station data
    real min_month, max_month;

    bool* truly_bad= bnalloc(n);
    setb(truly_bad, n, false);

    real* limsMin= rnalloc(n);
    real* limsMax= rnalloc(n);

    int i, j, k;
    for ( k= 0; k < n; ++k )
    {
        if ( se[k]->n_dates < 12 )
            truly_bad[k]= true;
        limsMin[k]= minR(se[k]->dates, se[k]->n_dates);
        limsMax[k]= maxR(se[k]->dates, se[k]->n_dates);
    }

    int block_size;

    if ( !options->clusterMode )
        block_size= 1000;
    else
        block_size= 50000;

    real* x= rnalloc(n);
    real* y= rnalloc(n);
    real* z= rnalloc(n);

    for ( k= 0; k < n; ++k )
    {
        x[k]= ss[k]->location->x;
        y[k]= ss[k]->location->y;
        z[k]= ss[k]->location->z;
    }

    tprintf("Distance Mapping\n");

    int** neighbor_list= (int**)malloc(sizeof(int*)*n);
    for ( k= 0; k < n; ++k )
        neighbor_list[k]= inalloc(consider + 1);
    real* distances= rnalloc(n);
    int* neighborhood= inalloc(n);
    int n_neighborhood;

    int block;
    for ( block= 0; block < n; block+= block_size )
    {
        int max_block= (block + block_size - 1) < (n - 1) ? (block + block_size - 1) : (n - 1);

        for ( k= block; k < max_block; ++k )
        {
            for ( i= 0; i < n; ++i )
                distances[i]= sqrt((x[i] - x[k]) * (x[i] - x[k]) + (y[i] - y[k])*(y[i] - y[k]) + (z[i] - z[k])*(z[i] - z[k]));
            distances[k]= FLT_MAX;

            n_neighborhood= 0;

            for ( i= 0; i < n; ++i )
                if ( distances[i] < max_dist && ! truly_bad[i] )
                    neighborhood[n_neighborhood++]= i;

            if ( n_neighborhood < consider )
            {
                for ( i= 0; i < n_neighborhood; ++i )
                    neighbor_list[k][i]= neighborhood[i];
                neighbor_list[k][i]= -1;
            }
            else
            {
                outer_array= distances;
                qsortORIA(neighborhood, n_neighborhood);

                for ( i= 0; i < consider; ++i )
                    neighbor_list[k][i]= neighborhood[i];
                neighbor_list[k][i]= -1;
            }
        }
    }

    real* data_high= rnalloc(size2*c);
    real* data_low= rnalloc(size2*c);

    int* consistent;
    printf("command: \n"); fflush(stdout);
    switch (command)
    {
        case 1: ;
        case 5: ;
            for ( i= 0; i < size2; ++i )
                for ( j= 0; j < c; ++j )
                    if ( mask[i*c + j] )
                    {
                        data2[i*c + j]= 0;
                        unc2[i*c + j]= 0;
                        counts2[i*c + j]= 0;
                    }

            for ( i= 0; i < size2; ++i )
                for ( j= 0; j < c; ++j )
                {
                    data_high[i*c + j]= data2[i*c + j] + unc2[i*c + j];
                    if ( ! counts2[i*c + j] )
                        data_high[i*c + j]= FLT_MAX;
                    data_low[i*c + j]= data2[i*c + j] - unc2[i*c + j];
                    if ( ! counts2[i*c + j] )
                        data_low[i*c + j]= -FLT_MAX;
                }

            for ( i= 0; i < size2; ++i )
            {
                for ( j= 0; j < c; ++j )
                {
                    printf("%f,%f,%f ", data2[i*c + j], data_high[i*c + j], data_low[i*c + j]);
                }
                printf("\n");
            }

            rangeResolver(data, unc, &consistent, f, size2, data_high, data_low, c);

            for ( i= 0; i < size1; ++i )
            {
                for ( j= 0; j < c; ++j )
                {
                    printf("%f,%f ", data[i*c + j], unc[i*c + j]);
                }
                printf("\n");
            }

            real* unc3= rnalloc(size2*c);
            for ( i= 0; i < size2; ++i )
                for ( j= 0; j < c; ++j )
                    unc3[i*c + j]= unc2[i*c + j];
            for ( i= 0; i < size2; ++i )
                for ( j= 0; j < c; ++j )
                    if ( !counts[i*c + j] )
                        unc3[i*c + j]= FLT_MAX;

            real* min_unc= rnalloc(size2);
            real minTmp;
            for ( i= 0; i < size2; ++i )
            {
                minTmp= unc3[i*c];
                for ( j= 0; j < c; ++j )
                    if ( minTmp > unc3[i*c + j] )
                        minTmp= unc3[i*c + j];
                min_unc[i]= minTmp;
            }

            for ( i= 0; i < size1; ++i )
                if ( unc[i*c] < min_unc[i] )
                    unc[i*c]= min_unc[i];

            if ( command == 5 )
                for ( i= 0; i < size1; ++i )
                    result_st->dates[i]= dates[i];

            for ( i= 0; i < size1; ++i )
            {
                result_st->data[i]= data[i*c];
                result_st->uncertainty[i]= unc[i*c];
            }

            printf("%d %d %d\n", result_st->n_dates, result_st->n_data, result_st->n_uncertainty);
            for ( i= 0; i < result_st->n_dates; ++i )
                printf("%f %f %f\n", result_st->dates[i], result_st->data[i], result_st->uncertainty[i]);

    }



}
