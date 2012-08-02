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
    int n_locations= n_ss;
    geoPoint2** locations= (geoPoint2**)malloc(sizeof(geoPoint2*)*n_ss);
    for ( i= 0; i < n_ss; ++i )
        locations[i]= ss[i]->location;

    // Time range for station data
    real min_month, max_month;
    monthRange(se, n_se, &min_month, &max_month);
    int n_time_values= max_month - min_month + 1;
    real* time_values= rnalloc(n_time_values);
    for ( i= 0; i < n_time_values; ++i )
        time_values[i]= ((int)min_month + i)/12.0 - 1.0/24.0 + 1600;
    tprintf("Month range: %f - %f; first and last time value: %f %f\n", min_month, max_month, time_values[0], time_values[n_time_values-1]);

    // Get bad flag list
    int* bf= iBadFlags;
    int n_bf= n_badFlags;

    // Remove any data points that are flagged with indicators of the "bad flags list" These points are not considered at all doing the averaging process
    tprintf("Remove bad flagged data points\n");
    removeBadFlaggedDataV(se, n_se, bf, n_bf);
    tprintf("End of removing bad flagged data points\n");

    int orig_length= n_se;
    int n_orig_map= n_se;
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

    /*printf("ORIG_MAP: ");
    for ( i= 0; i < n_se; ++i )
        if ( orig_map[i] )
            printf("%d ", i);
    printf("\n");*/

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

    /*int tmp;
    for ( i= 0; i < n_se; ++i )
    {
        tmp= 0;
        for ( j= 0; j < n_time_values; ++j )
            tmp+= occurance_table[i*n_time_values + j];
        printf("%d ", tmp); fflush(stdout);
    }*/

    // Apply station length and minimum number of measurement requirements
    tprintf("Apply station length and minimum number of measurement requirements\n");
    int* not_fittable= inalloc(n_time_values);
    int n_not_fittable= 0;
    bool changed= true;
    int sum;
    bool* change= bnalloc(n_se);
    int* select= inalloc(10000);
    while ( changed )
    {
        changed= false;

        // Remove stations with too little data
        for ( i= 0; i < n_se; ++i )
        {
            sum= 0;
            for ( j= 0; j < n_time_values; ++j )
                sum+= occurance_table[i*n_time_values + j];
            if ( (sum < min_months) && orig_map[i] )
            {
                if ( changed == false )
                    changed= true;
                orig_map[i]= false;
                for ( j= 0; j < n_time_values; ++j )
                    occurance_table[i*n_time_values + j]= false;
            }
        }

        /*printf("\nORIG_MAP: ");
        for ( i= 0; i < n_se; ++i )
            if ( orig_map[i] )
                printf("%d ", i);
        printf("\n");

        printf("\n============================\n");
        int tmp;
        for ( i= 0; i < n_se; ++i )
        {
            tmp= 0;
            for ( j= 0; j < n_time_values; ++j )
                tmp+= occurance_table[i*n_time_values + j];
            printf("%d ", tmp); fflush(stdout);
        }
        printf("\n============================\n");*/

        // Remove time steps where fewer than min_stations reported data
        setb(change, n_se, false);

        n_not_fittable= 0;
        for ( i= 0; i < n_time_values; ++i )
        {
            sum= 0;
            for ( j= 0; j < n_se; ++j )
                sum+= occurance_table[j*n_time_values + i];

            //printf(".%d.", sum);

            if ( sum < min_stations )
                not_fittable[n_not_fittable++]= i;

        }

        /*for ( i= 0; i < n_not_fittable; ++i )
            printf("%d ", not_fittable[i]);
        printf("\n");*/

        real monthnum;
        int n_select= 0;
        for ( i= 0; i < n_se; ++i )
        {
            n_select= 0;
            for ( j= 0; j < se[i]->n_dates; ++j )
            {
                monthnum= monthNum(se[i]->dates[j]) - min_month;
                for ( k= 0; k < n_not_fittable; ++k )
                    if ( (int)monthnum == not_fittable[k] )
                        break;
                if ( k == n_not_fittable )
                {
                    select[n_select++]= j;
                }
            }
            if ( n_select != se[i]->n_dates )
            {
                stationElement2p seTmp= createSE2Select(se[i], select, n_select);
                destroySE2(se[i]);
                se[i]= seTmp;
                change[i]= true;
            }
        }
        int sum_changed= false;
        for ( i= 0; i < n_se; ++i )
            sum_changed+= change[i];
        //printf(" %d ", sum_changed); fflush(stdout);
        changed= changed || sum_changed;

        for ( i= 0; i < n_se; ++i )
            for ( j= 0; j < n_not_fittable; ++j )
                occurance_table[i*n_time_values + not_fittable[j]]= false;
    }

    free(change);
    free(select);

    tprintf("Finished: Apply station length and minimum number of measurement requirements\n");

    // Update the various time and occurance tables to deal with times removed in the previous step
    tprintf("Updating time and occurance talbes...\n");

    int* MM= inalloc(n_time_values);
    int n_MM= (int)(max_month - min_month);
    for ( i= (int)min_month; i <= (int)max_month; ++i )
        MM[i - (int)min_month]= i;
    for ( i= 0; i < n_not_fittable; ++i )
        MM[not_fittable[i]]= -1;
    min_month= 100000;
    max_month= -1;
    for ( i= 0; i < n_time_values; ++i )
    {
        if ( MM[i] != -1 )
        {
            if ( MM[i] < min_month )
                min_month= MM[i];
            if ( MM[i] > max_month )
                max_month= MM[i];
        }
    }

    free(not_fittable);
    free(MM);

    tprintf("Updated min and max months: %f %f; time_values: %d %d\n", min_month, max_month, n_time_values, (int)(max_month - min_month) + 1);

    int n_times2= (int)(max_month - min_month) + 1;
    real* times2= rnalloc(n_times2);
    for ( i= 0; i < n_times2; ++i )
        times2[i]= ((int)min_month + i)/12.0 - 1.0/24.0 + 1600;

    //dea();
    int* drop= inalloc(n_time_values);
    int n_drop= 0;

    /*printf("time_values:\n");
    for ( i= 0; i < n_time_values; ++i )
        printf("%f ", time_values[i]);
    printf("\n");
    for ( i= 0; i < n_times2; ++i )
        printf("%f ", times2[i]);
    printf("\n");*/

    //deb();
    for ( k= 0; k < n_time_values; ++k )
    {
        for ( i= 0; i < n_times2; ++i )
        {
            if ( times2[i] == time_values[k] )
                break;
        }
        if ( i == n_times2 )
            drop[n_drop++]= k;
    }

    /*printf("drop: \n");
    for ( i= 0; i < n_drop; ++i )
        printf("%d ", drop[i]);
    printf("\n");
    dec();*/
    free(time_values);
    time_values= times2;

    //ded();
    for ( i= 0; i < n_se; ++i )
        for ( j= 0; j < n_drop; ++j )
            occurance_table[i*n_time_values + drop[j]]= 255;

    //dee();
    bool* occurance_table2= bnalloc(n_times2*n_se);
    for ( i= 0; i < n_se; ++i )
    {
        k= 0;
        for ( j= 0; j < n_time_values; ++j )
        {
            //printf("%d ", occurance_table[i*n_time_values + j]);
            if ( occurance_table[i*n_time_values + j] != 255 )
                occurance_table2[i*n_times2+k++]= occurance_table[i*n_time_values + j];
        }
        //printf("%d,%d ", k, n_times2); fflush(stdout);
    }

    //def();
    free(occurance_table);
    occurance_table= occurance_table2;
    n_time_values= n_times2;

    //Collapse redundant locations
    tprintf("Collapse redundant locations\n");
    int n_locations2= 0;
    for ( i= 0; i < n_orig_map; ++i )
        if ( orig_map[i] == true )
            n_locations2++;

    geoPoint2** locations2= (geoPoint2**)malloc(sizeof(geoPoint2*)*n_locations2);
    j= 0;
    for ( i= 0; i < n_locations; ++i )
        if ( orig_map[i] == true )
            locations2[j++]= locations[i];

    locations= locations2;
    n_locations= n_locations2;

    int* locHashes= inalloc(n_locations);
    for ( i= 0; i < n_locations; ++i )
        locHashes[i]= (((int)(locations[i]->latitude*100000)+(int)(locations[i]->longitude*100000))+(int)(locations[i]->elevation*100000))%13333;

    int* collapsed;
    int* expand_map;
    int n_collapsed;
    int n_expand_map;
    uniqueIAN2(locHashes, n_locations, &collapsed, &n_collapsed, &expand_map, &n_expand_map);

    geoPoint2** locations_short= (geoPoint2**)malloc(sizeof(geoPoint2*)*n_collapsed);
    for ( i= 0; i < n_collapsed; ++i )
        locations_short[i]= locations[collapsed[i]];
    int num_sites= n_collapsed;

    free(locHashes);

    // Eliminate station locations with no usable data
    tprintf("Eliminate station locations with no usable data\n");

    int n_se2= 0;
    for ( i= 0; i < n_orig_map; ++i )
        if ( orig_map[i] == true )
            n_se2++;

    occurance_table2= bnalloc(n_time_values*n_se2);
    for ( i= 0; i < n_se2; ++i )
    {
        j= 0;
        if ( orig_map[i] == true )
        {
            for ( k= 0; k < n_time_values; ++k )
                occurance_table2[j*n_time_values + k]= occurance_table[i*n_time_values + k];
            ++j;
        }
    }

    stationSite2p* se2= (stationElement2p*)malloc(sizeof(stationElement2p*)*n_se2);
    j= 0;
    for ( i= 0; i < n_se; ++i )
        if ( orig_map[i] == true )
            se2[j++]= se[i];

    se= se2;
    n_se= n_se2;

    tprintf("Elmination of station locations finished\n");
    tprintf("occurance_table: %d x %d; se: %d; locations: %d; locations_short: %d\n", n_time_values, n_se, n_se, n_locations, num_sites);

    //Map of sites having multiple records
    tprintf("Map of sites having multiple records\n");
    int** comparison_map= (int**)malloc(sizeof(int*)*num_sites);
    int* n_comparison_map= (int*)malloc(sizeof(int)*num_sites);
    int* st_value= (int*)malloc(sizeof(int)*n_se);
    seti(st_value, n_se, 0);

    for ( k= 0; k < n_se; ++k )
    {
        int mn= 10000;
        for ( j= 0; j < se[k]->n_dates; ++j )
            if ( monthNum(se[k]->dates[j]) < mn )
                mn= monthNum(se[k]->dates[j]);
        st_value[k]= mn;
    }

    int* st_value_f= inalloc(10000);
    int n_st_value_f;

    for ( k= 0; k < num_sites; ++k )
    {
        int* f= inalloc(1000);
        int n_f= 0;
        for ( j= 0; j < n_expand_map; ++j )
            if ( expand_map[j] == k )
                f[n_f++]= j;

        outer_arrayI= st_value;
        qsortOIIA(f, n_f);
        comparison_map[k]= realloc(f, sizeof(int)*n_f);
        n_comparison_map[k]= n_f;
    }
    free(st_value_f);

    tprintf("Finished: Map of sites having multiple records\n");

    // Make access faster (requires more memory)
    int** dates_array= (int**)malloc(sizeof(int*)*n_se);
    int* n_dates_array= (int*)malloc(sizeof(int)*n_se);
    int n_n_dates_array= n_se;
    for ( i= 0; i < n_n_dates_array; ++i )
    {
        n_dates_array[i]= se[i]->n_dates;
        dates_array[i]= (int*)malloc(sizeof(int)*(se[i]->n_dates));
        for ( j= 0; j < n_dates_array[i]; ++j )
            dates_array[i][j]= monthNum(se[i]->dates[j]) - min_month;
    }

    temp_t** data_array= (temp_t**)malloc(sizeof(temp_t*)*n_se);
    int* n_data_array= (int*)malloc(sizeof(int)*n_se);
    int n_n_data_array= n_se;
    for ( i= 0; i < n_n_data_array; ++i )
    {
        n_data_array[i]= se[i]->n_data;
        data_array[i]= se[i]->data;
    }

    // Further collapse locations according to grid approximation rules


    tprintf("End of Berkeley Average Core Process\n");

    geoPoint2p* locations_collapsed;
    int n_locations_collapsed;
    int* collapse_indices;
    int n_collapse_indices;

    collapseLocations(locations_short, num_sites, options->gridApproximationDistance, &locations_collapsed, &n_locations_collapsed, &collapse_indices, &n_collapse_indices);
}
