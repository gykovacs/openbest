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

    if ( options->useIterativeReweighting )
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
    monthRange(se, n_se, &min_month, &max_month);
    int n_time_values= max_month - min_month + 1;
    real* time_values= rnalloc(n_time_values);
    for ( i= 0; i < n_time_values; ++i )
        time_values[i]= (min_month + i)/12.0 - 1.0/24.0 + 1600;
    tprintf("Month range: %f - %f; first and last time value: %f %f\n", min_month, max_month, time_values[0], time_values[n_time_values-1]);

    // Get bad flag list
    int* bf= iBadFlags;
    int n_bf= n_badFlags;

    // Remove any data points that are flagged with indicators of the "bad flags list" These points are not considered at all doing the averaging process
    tprintf("Remove bad flagged data points\n");
    removeBadFlaggedDataV(se, n_se, bf, n_bf);
    tprintf("End of removing bad flagged data points\n");

    int orig_length= n_se;
    bool* orig_map= bnalloc(orig_length);
    setb(orig_map, orig_length, true);

    // Remove stations with no location
    for ( i= 0; i < n_se; ++i )
        if ( !isValidGP2(locations[i]))
            orig_map[i]= false;

    // Temporary Fix: removing ultra-high variability created by bad sasonality procedure on sparse data. This will be removed in the future versions when the seasonality procedure is fixed.
    for ( i= 0; i < n_se; ++i )
        if ( stdT(se[i]->data, se[i]->n_data) > 7 )
            orig_map[i]= false;

    // Create a table of data occurances, used to generate spatial weights
    bool* occurance_table= bnalloc(n_se*n_time_values); // dimensions: n_se x n_time_values; stride: n_time_values
    setb(occurance_table, n_se*n_time_values, false);
    for ( i= 0; i < n_se; ++i )
        if ( orig_map[i] )
        {
            for ( j= 0; j < se[i]->n_dates; ++j )
            {
                real monthnum= monthNum(se[i]->dates[j]) - min_month;
                occurance_table[i*n_time_values + (int)monthnum]= true;
            }
        }
    ded();
    // Apply station length and minimum number of measurement requirements
    bool changed= true;
    int sum;
    while ( changed )
    {
        changed= false;

        // Remove stations with too little data
        for ( i= 0; i < n_se; ++i )
        {
            sum= 0;
            for ( j= 0; j < n_time_values; ++j )
                sum+= occurance_table[i*n_time_values + j];
            if ( sum < min_months && orig_map[i] )
            {
                if ( changed == false )
                    changed= true;
                orig_map[i]= false;
            }
            for ( j= 0; j < n_time_values; ++j )
                occurance_table[j]= false;
        }

        // Remove time steps where fewer than min_stations reported data
        for ( i= 0; i < n_time_values; ++i )
        {
            sum= 0;
            for ( j= 0; j < n_se; ++j )
                sum+= occurance_table[j*n_time_values + i];
            if ( sum < min_stations )
                ;
        }
}
