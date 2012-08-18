#include "openbest-av/berkeleyAverageCore.h"
#include "openbest-av/berkeleyAverageResults.h"
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
#include "openbest-av/idealGrid.h"
#include "openbest-av/buildCovarianceTable.h"
#include "openbest-av/buildTargetTable.h"
#include "openbest-ds/geoPoint2.h"
#include "openbest-av/buildBaselineTable.h"
#include "openbest-av/buildSpatialMap.h"
#include "openbest-av/buildMatrices.h"
#include "openbest-av/performFit.h"
#include "openbest-av/movingAverages.h"
#include "openbest-ds/timeInstant.h"

berkeleyAverageResults* berkeleyAverageCore(stationElement2p** seIO, int* n_seIO, stationSite2p** ssIO, int* n_ssIO, berkeleyAverageOptions* options)
{
    int i, j, k, l;

    berkeleyAverageResults* results= (berkeleyAverageResults*)malloc(sizeof(berkeleyAverageResults));

    stationElement2p* se= *seIO;
    stationSite2p* ss= *ssIO;
    int n_se= *n_seIO;
    int n_ss= *n_ssIO;

    tprintf("Berkeley Average Core called with %d time series\n", n_se);

    /*for ( i= 0; i < n_se; ++i )
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
    }*/

    /*displayTempDataSE2(se, n_se);
    getchar();*/

    int min_stations= options->minStations;
    int min_months= options->minMonths;
    int matlabPoolSize= 1;

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
    printArrayRFile("time_values", time_values, n_time_values);
    tprintf("Month range: %f - %f; first and last time value: %f %f\n", min_month, max_month, time_values[0], time_values[n_time_values-1]);

    // Get bad flag list
    int* bf= iBadFlags;
    int n_bf= n_badFlags;

    // Remove any data points that are flagged with indicators of the "bad flags list" These points are not considered at all doing the averaging process
    tprintf("Remove bad flagged data points\n");
    //removeBadFlaggedDataV(se, n_se, bf, n_bf);
    tprintf("End of removing bad flagged data points\n");

    int orig_length= n_se;
    int n_orig_map= n_se;
    bool* orig_map= bnalloc(orig_length);
    setb(orig_map, orig_length, true);

    // Remove stations with no location
    for ( i= 0; i < n_se; ++i )
        if ( !isValidGP2(locations[i]))
            orig_map[i]= false;

    int sum_orig_map= 0;
    for ( i= 0; i < n_orig_map; ++i )
        sum_orig_map+= orig_map[i];
    tprintf("sum orig_map isvalidgp2: %d\n", sum_orig_map);

    // Temporary Fix: removing ultra-high variability created by bad sasonality procedure on sparse data. This will be removed in the future versions when the seasonality procedure is fixed.
    for ( i= 0; i < n_se; ++i )
        if ( stdT(se[i]->data, se[i]->n_data) > 7 )
            orig_map[i]= false;

    sum_orig_map= 0;
    for ( i= 0; i < n_orig_map; ++i )
        sum_orig_map+= orig_map[i];
    tprintf("sum orig_map > 7: %d\n", sum_orig_map);

    // Create a table of data occurances, used to generate spatial weights
    bool* occurance_table= bnalloc(n_se*n_time_values); // dimensions: n_se x n_time_values; stride: n_time_values
    int n_occurance_table1= n_se;
    int n_occurance_table2= n_time_values;
    setb(occurance_table, n_se*n_time_values, false);
    for ( i= 0; i < n_se; ++i )
        if ( orig_map[i] )
        {
            for ( j= 0; j < se[i]->n_dates; ++j )
            {
                int monthnum= monthNum(se[i]->dates[j]) - min_month;
                occurance_table[i*n_time_values + monthnum]= true;
            }
        }

    int sum_occ= 0;
    for ( i= 0; i < n_occurance_table1*n_occurance_table2; ++i )
        sum_occ+= occurance_table[i];
    tprintf("sum_occ: %d\n", sum_occ);
    getchar();

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

        setb(change, n_se, false);

        n_not_fittable= 0;
        for ( i= 0; i < n_time_values; ++i )
        {
            sum= 0;
            for ( j= 0; j < n_se; ++j )
                sum+= occurance_table[j*n_time_values + i];

            if ( sum < min_stations )
                not_fittable[n_not_fittable++]= i;

        }

        int monthnum;
        int n_select= 0;
        for ( i= 0; i < n_se; ++i )
        {
            n_select= 0;
            for ( j= 0; j < se[i]->n_dates; ++j )
            {
                monthnum= monthNum(se[i]->dates[j]) - min_month;
                for ( k= 0; k < n_not_fittable; ++k )
                    if ( monthnum == not_fittable[k] )
                        break;
                if ( k == n_not_fittable )
                    select[n_select++]= j;
            }
            if ( n_select != se[i]->n_dates )
            {
                printf("."); fflush(stdout);
                stationElement2p seTmp= createSE2Select(se[i], select, n_select);
                printf(","); fflush(stdout);
                destroySE2(se[i]);
                se[i]= seTmp;
                change[i]= true;
            }
        }
        int sum_changed= false;
        for ( i= 0; i < n_se; ++i )
            sum_changed+= change[i];

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
    int n_MM= (int)(max_month - min_month + 1);
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

    tprintf("times2 0, end: %f, %f\n", times2[0], times2[n_times2 - 1]);
    getchar();

    int* drop= inalloc(n_time_values);
    int n_drop= 0;

    for ( k= 0; k < n_time_values; ++k )
    {
        for ( i= 0; i < n_times2; ++i )
            if ( times2[i] == time_values[k] )
                break;
        if ( i == n_times2 )
            drop[n_drop++]= k;
    }

    free(time_values);
    time_values= times2;

    for ( i= 0; i < n_se; ++i )
        for ( j= 0; j < n_drop; ++j )
            occurance_table[i*n_time_values + drop[j]]= 255;

    bool* occurance_table2= bnalloc(n_times2*n_se);
    for ( i= 0; i < n_se; ++i )
    {
        k= 0;
        for ( j= 0; j < n_time_values; ++j )
        {
            if ( occurance_table[i*n_time_values + j] != 255 )
                occurance_table2[i*n_times2+k++]= occurance_table[i*n_time_values + j];
        }
    }

    free(occurance_table);
    occurance_table= occurance_table2;
    n_time_values= n_times2;
    n_occurance_table1= n_se;
    n_occurance_table2= n_times2;

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
        locHashes[i]= (((int)(locations[i]->latitude*100000)+(int)(locations[i]->longitude*100000))+(int)(locations[i]->elevation*100000))%13453;

    int* collapsed;
    int* expand_map;
    int n_collapsed;
    int n_expand_map;
    uniqueIAN2(locHashes, n_locations, &collapsed, &n_collapsed, &expand_map, &n_expand_map);
    /*collapsed= inalloc(n_locations);
    expand_map= inalloc(n_locations);
    for ( i= 0; i < n_locations; ++i )
        collapsed[i]= expand_map[i]= i;
    n_collapsed= n_locations;
    n_expand_map= n_locations;*/

    printArrayIFile("collapsed", collapsed, n_collapsed);
    printArrayIFile("expand_map", expand_map, n_expand_map);

    geoPoint2** locations_short= (geoPoint2**)malloc(sizeof(geoPoint2*)*n_collapsed);
    for ( i= 0; i < n_collapsed; ++i )
        locations_short[i]= locations[collapsed[i]];
    int num_sites= n_collapsed;
    int n_locations_short= n_collapsed;

    free(locHashes);

    // Eliminate station locations with no usable data
    tprintf("Eliminate station locations with no usable data\n");

    int n_se2= 0;
    for ( i= 0; i < n_orig_map; ++i )
        if ( orig_map[i] == true )
            n_se2++;

    occurance_table2= bnalloc(n_time_values*n_se2);
    j= 0;
    for ( i= 0; i < n_orig_map; ++i )
    {

        if ( orig_map[i] == true )
        {
            for ( k= 0; k < n_time_values; ++k )
                occurance_table2[j*n_time_values + k]= occurance_table[i*n_time_values + k];
            ++j;
        }
    }

    sum_occ= 0;
    for ( i= 0; i < n_occurance_table1*n_occurance_table2; ++i )
        sum_occ+= occurance_table[i];
    tprintf("sum_occ: %d\n", sum_occ);
    sum_orig_map= 0;
    for ( i= 0; i < n_orig_map; ++i )
        sum_orig_map+= orig_map[i];
    tprintf("orig_map: %d\n", sum_orig_map);
    getchar();

    occurance_table= occurance_table2;
    n_occurance_table1= n_se2;
    n_occurance_table2= n_time_values;
    stationElement2p* se2= (stationElement2p*)malloc(sizeof(stationElement2p*)*n_se2);
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

/*    printf("n_se: %d; n_locations: %d\n", n_se, n_locations);
    for ( i= 0; i < n_se; ++i )
        printf("%f %d %f %f %f %f\n", yearNum(dates_array[i][0]+min_month), dates_array[i][0], data_array[i][0], locations[i]->latitude, locations[i]->longitude, locations[i]->elevation);
    getchar();*/

    // Further collapse locations according to grid approximation rules

    /*geoPoint2p* locations_collapsed;
    int n_locations_collapsed;
    int* collapse_indices;
    int n_collapse_indices;

    collapseLocations(locations_short, n_locations_short, options->gridApproximationDistance, &locations_collapsed, &n_locations_collapsed, &collapse_indices, &n_collapse_indices);

    locations_short= locations_collapsed;
    n_locations_short= n_locations_collapsed;
    for ( i= 0; i < n_expand_map; ++i )
        expand_map[i]= collapse_indices[expand_map[i]];*/

    real* lat2;
    int n_lat2;
    real* long2;
    int n_long2;
    idealGrid(options->gridSize, &lat2, &n_lat2, &long2, &n_long2);
    geoPoint2** map_pts= (geoPoint2**)malloc(sizeof(geoPoint2*)*n_lat2);
    int n_map_pts= n_lat2;
    for ( i= 0; i < n_lat2; ++i )
        map_pts[i]= createGeoPoint22(lat2[i], long2[i]);

    free(lat2);
    free(long2);

    real* areal_weight= NULL;
    real* map_elev= NULL;
    int n_areal_weight;
    if ( options->useLandMask || options->fullBaselineMapping )
    {
        if ( options->gridSize == 16000 )
        {
            //TODO: load mask16000.mat
        }
        else
        {
            //TODO: makeLandMask function
        }
        //transpose and convert areal_weight and map_elev
        if ( !options->useLandMask )
        {
            //TODO: initialize areal_weight with ones
        }
    }
    else
    {
        areal_weight= rnalloc(n_map_pts);
        set(areal_weight, n_map_pts, 1);
        n_areal_weight= n_map_pts;
    }

    // If using the full climatology model, we need to assign an elevation to
    // each station

    real* site_elev= NULL;
    real* site_lat= NULL;
    real* map_lat= NULL;
    if ( options->fullBaselineMapping )
    {
        // TODO
    }

    tprintf("Averaging: %d records used from %d sites\n", n_n_data_array, num_sites);
    tprintf("%d sites used for network approximation\n", n_locations_short);


    /////////////////////////////////////////////////////////////////////////////////////
    ////////////////////BEGINNING OF VALIDATED   ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////

    float* correlation_table;
    int n_correlation_table;
    float nugget;
    {
        FILE* lsfile= fopen("locations_short", "wt");
        for ( i= 0; i < n_locations_short; ++i )
            fprintf(lsfile, "%f %f %f\n", locations_short[i]->latitude, locations_short[i]->longitude, locations_short[i]->elevation);
        fclose(lsfile);
    }

    buildCovarianceTable(locations_short, n_locations_short, options, &correlation_table, &n_correlation_table, &nugget);
    printArray2FloatFile("correlation_table", correlation_table, n_correlation_table, n_correlation_table);
    tprintf("nugget: %f\n", nugget);
    getchar();

    // Build spatial target function
    float* target_map;
    int* near_index;
    int n_target_map1, n_target_map2;
    int n_near_index;

    buildTargetTable(locations_short, n_locations_short, map_pts, n_map_pts, options, &target_map, &n_target_map1, &n_target_map2, &near_index, &n_near_index);
    printArray2FloatFile("target_map", target_map, n_target_map1, n_target_map2);
    printArrayIFile("near_index", near_index, n_near_index);
    printArrayIFile("expand_map", expand_map, n_expand_map);
    tprintf("buildTargetTable finished: n_target_map1,2 %d %d, n_near_index: %d\n", n_target_map1, n_target_map2, n_near_index);

    int* near_index_new= inalloc(n_expand_map);
    for ( i= 0; i < n_expand_map; ++i )
        near_index_new[i]= near_index[expand_map[i]];
    free(near_index);
    near_index= near_index_new;
    n_near_index= n_expand_map;

    free(locations_short);
    //free(locations_collapsed);

    // Variable for storing spatial weights
    // spatial_map - I did not find any use of this structure in the rest of the code, so I ignore it's reimplementation

    int* num_months= inalloc(n_se2);
    seti(num_months, n_se2, 0);
    for ( i= 0; i < n_se2; ++i )
        for ( j= 0; j < n_time_values; ++j )
            num_months[i]+= occurance_table[i*n_time_values + j];
    int n_num_months= n_se2;

    int* total_cnts= inalloc(n_correlation_table);
    seti(total_cnts, n_correlation_table, 0);
    int n_total_cnts= n_correlation_table;
    for ( k= 0; k < n_expand_map; ++k )
        total_cnts[expand_map[k]]= total_cnts[expand_map[k]] + num_months[k];

    tprintf("Determine baseline mixing weights\n");
    // Determine baseline mixing weights

    real* all_station_mix;
    int n_all_station_mix;
    double completeness;
    double global_completeness;
    real* coverage_map;
    int n_coverage_map;
    double* base_weights_map;
    int n_base_weights_map1;
    int n_base_weights_map2;
    if ( options->fullBaselineMapping )
    {
        // TODO
    }
    else
    {
        printArray2FloatFile("correlation_table", correlation_table, n_correlation_table, n_correlation_table);
        printArray2FloatFile("target_map", target_map, n_target_map1, n_target_map2);
        printArray2BFile("occurance_table", occurance_table, n_occurance_table1, n_occurance_table2);
        printArrayIFile("expand_map", expand_map, n_expand_map);
        printArrayRFile("areal_weight", areal_weight, n_areal_weight);

        tprintf("correlation_table: %d %d\n", n_correlation_table, n_correlation_table);
        tprintf("target_map: %d %d\n", n_target_map1, n_target_map2);
        tprintf("occurance_table: %d %d\n", n_occurance_table1, n_occurance_table2);
        tprintf("expand_map: %d\n", n_expand_map);
        tprintf("areal_weight: %d\n", n_areal_weight);
        buildBaselineTable(correlation_table, n_correlation_table,
                           target_map, n_target_map1, n_target_map2,
                           occurance_table, n_se2, n_time_values,
                           expand_map, n_expand_map, nugget,
                           areal_weight, n_areal_weight, options,
                           NULL, 0, NULL, 0, NULL, 0, NULL, 0,

                           &all_station_mix, &n_all_station_mix,
                           &completeness, &global_completeness,
                           &coverage_map, &n_coverage_map,
                           &base_weights_map, &n_base_weights_map1, &n_base_weights_map2,
                           NULL, NULL);

        tprintf("all_station_mix: %d\n", n_all_station_mix);
        tprintf("coverage_map: %d\n", n_coverage_map);
        tprintf("base_weights_map: %d %d\n", n_base_weights_map1, n_base_weights_map2);
        printArrayRFile("all_station_mix", all_station_mix, n_all_station_mix);
        printArrayRFile("coverage_map", coverage_map, n_coverage_map);
        printArray2DFile("base_weights_map", base_weights_map, n_base_weights_map1, n_base_weights_map2);
        getchar();
    }

    n_occurance_table1= n_se2;
    n_occurance_table2= n_time_values;
    // Crop fit region if requested
    if ( options->limitEmpiricalFitRegion )
        for ( i= 0; i < n_coverage_map; ++i )
            if ( coverage_map[i] < options->empiricalFitRegionCutoff )
                areal_weight[i]= 0;

    // Perform numerical integral.
    double sum_areal_weight= 0;
    for ( i= 0; i < n_areal_weight; ++i )
        sum_areal_weight+= areal_weight[i];

    tprintf("areal_weight: %d; target_map: %d, %d\n", n_areal_weight, n_target_map1, n_target_map2);

    float* target= (float*)malloc(sizeof(float)*n_target_map2);
    int n_target= n_target_map2;
    for ( i= 0; i < n_target_map2; ++i )
    {
        target[i]= 0;
        for ( j= 0; j < n_target_map1; ++j )
            target[i]+= areal_weight[j]*target_map[j*n_target_map2 + i];
        target[i]/= sum_areal_weight;
    }

    // Spatial weights for the global average
    tprintf("Spatial weights for the global average\n");

    double* spatial_table;
    int n_spatial_table1;
    int n_spatial_table2;

    buildSpatialMap(correlation_table, n_correlation_table,
                    target, 1, n_target,
                    occurance_table, n_occurance_table1, n_occurance_table2,
                    expand_map, n_expand_map, nugget, options,
                    &spatial_table, &n_spatial_table1, &n_spatial_table2,
                    NULL, NULL, NULL);

    tprintf("spatial_table: %d %d\n", n_spatial_table1, n_spatial_table2);
    printArray2DFile("spatial_table", spatial_table, n_spatial_table1, n_spatial_table2);

    real* coverage_summary= rnalloc(n_spatial_table2);
    int n_coverage_summary= n_spatial_table2;
    set(coverage_summary, n_spatial_table2, 0);
    for ( i= 0; i < n_spatial_table2; ++i )
        for ( j= 0; j < n_spatial_table1; ++j )
            coverage_summary[i]+= spatial_table[j*n_spatial_table2 + i];
    for ( i= 0; i < n_spatial_table2; ++i )
        coverage_summary[i]*= completeness;

    tprintf("coverage_summary\n");
    printArrayRFile("coverage_summary", coverage_summary, n_coverage_summary);
    getchar();

    //////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////END OF VALIDATED///////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////

    double* spatial_maps;
    int n_spatial_maps1;
    int n_spatial_maps2;

    // If local we need to generate the more complicated spatial mapping tables
    if ( options->localMode )
    {
        //TODO
    }
    else
    {
        spatial_maps= spatial_table;
        n_spatial_maps1= n_spatial_table1;
        n_spatial_maps2= n_spatial_table2;
    }

    // If working under a parallel processing environment, distribute the data
    // to the different nodes.

    if ( matlabPoolSize > 1 )
    {
        // TODO
    }

    tprintf("Total Network Land Completeness: %f\n", completeness);
    tprintf("Total Network All Surface Completeness: %f\n", global_completeness);

    results->network_completeness= completeness;

    int len_t= n_time_values;
    int len_s= n_n_data_array;

    tprintf("len_t, len_s: %d, %d\n", len_t, len_s);

    bool first= true;
    bool done= false;

    int* window= inalloc(n_occurance_table2);
    int n_window= 0;
    if ( options->useIterativeReweighting )
    {
        // TODO
    }

    tprintf("Compute Temperature Fit\n");

    int loop= 0;

    tprintf("len_t, len_s, max_loop: %d, %d, %d\n", len_t, len_s, max_loop);
    // Various data storage variables
    real* results_list= rnalloc((len_t + len_s)*max_loop);
    int n_results_list1= len_t + len_s;
    int n_results_list2= max_loop;
    set(results_list, n_results_list1 * n_results_list2, 0);
    real* input_list= copyRA(results_list, n_results_list1*n_results_list2);
    int n_input_list1= n_results_list1;
    int n_input_list2= n_results_list2;

    float* map;
    int n_map= 0;
    float* map_dist;
    int n_map_dist= 0;

    if ( options->fullBaselineMapping )
    {
        // TODO
    }

    // Used for the low-memory Jacobian approximation in Broyden's method
    if ( options->useIterativeReweighting && options->useBroydenMethod )
    {
        // TODO (useIterativeReweighting is disabled in "quick" mode)
    }

    real* local_misfit= rnalloc(max_loop);
    int n_local_misfit= max_loop;
    set(local_misfit, n_local_misfit, 0);

    real* global_misfit= copyRA(local_misfit, n_local_misfit);
    int n_global_misfit= n_local_misfit;

    real* max_change= copyRA(local_misfit, n_local_misfit);
    int n_max_change= n_local_misfit;

    real* t_res= rnalloc(len_t);
    int n_t_res= len_t;
    set(t_res, n_t_res, -FLT_MAX);
    real* b_res= rnalloc(len_s);
    int n_b_res= len_s;
    set(b_res, n_b_res, -FLT_MAX);

    real sigma;
    real sigma_full;

    real* site_weight;
    int n_site_weight;
    double* new_spatial_table= NULL;
    int n_new_spatial_table1;
    int n_new_spatial_table2;
    double* new_spatial_dist= NULL;
    int n_new_spatial_dist1;
    int n_new_spatial_dist2;

    double* base_weights;
    double* base_constants;
    double* temperature_map;
    double* temperature_constant;
    double* record_weight;
    int n_base_weights;
    int n_base_constants;
    int n_temperature_map;
    int n_temperature_constant;
    int n_record_weight;

    real* res2;
    int n_res2;

    // This loop is the main workhorse for the iterative reweighting
    while ( !done )
    {
        ++loop;
        tprintf("loop: %d\n", loop);

        if ( loop > 1 && ! options->useIterativeReweighting )
        {
            // Break out if iterative process not requested
            break;
        }

        if ( loop > max_loop )
        {
            // Break out if too many lopps already executed
            tprintf("Max loops reached... Exiting.\n");
            break;
        }

        if ( options->useIterativeReweighting )
        {
            // Display loop number
            tprintf("Averaging Loop: %d\n", loop);
        }

        printf("aaa\n"); fflush(stdout);
        for ( i= 0; i < n_input_list1; ++i )
            if ( i < n_t_res )
                input_list[i*n_input_list2 + loop - 1]= t_res[i];
            else
                input_list[i*n_input_list2 + loop - 1]= b_res[i - n_t_res];

        printf("bbb\n"); fflush(stdout);
        if ( ! first )
        {
            // Determine the quality of the fit during the previous iteration
            // TODO
        }
        else
        {
            sigma= FLT_MAX;
            sigma_full= FLT_MAX;
            local_misfit[loop - 1]= -FLT_MAX;
            global_misfit[loop - 1]= -FLT_MAX;
        }

        printf("ccc\n"); fflush(stdout);
        // Compute the quality of fit for each record
        if ( !first && options->useSiteWeighting )
        {
            // TODO (disabled in "quick" mode)
        }
        else
        {
            site_weight= rnalloc(len_s);
            n_site_weight= len_s;
            set(site_weight, n_site_weight, 1);
        }

        if ( !first && options->useSiteWeighting && options->localMode )
        {
            // In local mode we perform a complicated reweighting of the spatial maps table.
            // This produces a more accurate accounting of the impact of site weighting,
            // but is slow.
            // TODO (diabled in "quick" mode)
        }
        else
        {
            // Without local mode, we simply scale station spatial weights by the
            // associated site weight. This is an approximate solution that
            // avoids the difficult numerical integral issues. It works well
            // enough in many cases, but the results are not as accurate as
            // those provided by the full relocalization process.
            tprintf("spatial_table: %d,%d, site_weight: %d\n", n_spatial_table1, n_spatial_table2, n_site_weight);
            new_spatial_table= dnalloc(n_spatial_table1*n_spatial_table2);
            n_new_spatial_table1= n_spatial_table1;
            n_new_spatial_table2= n_spatial_table2;

            printf("ddd\n"); fflush(stdout);
            for ( i= 0; i < n_spatial_table1; ++i )
                for ( j= 0; j < n_spatial_table2; ++j )
                    new_spatial_table[i*n_spatial_table2 + j]= spatial_table[i*n_spatial_table2 + j] * site_weight[i];

            tprintf("new_spatial_table %d %d\n", n_new_spatial_table1, n_new_spatial_table2);
            printArray2DFile("new_spatial_table", new_spatial_table, n_new_spatial_table1, n_new_spatial_table2);

            if ( options->localMode )
            {
                // TODO
            }
            if ( matlabPoolSize > 1 )
            {
                // TODO
            }
            else
            {
                new_spatial_dist= copyDA(new_spatial_table, n_new_spatial_table1*n_new_spatial_table2);
                n_new_spatial_dist1= n_new_spatial_table1;
                n_new_spatial_dist2= n_new_spatial_table2;
            }
        }

        {
            FILE* daf= fopen("data_array", "wt");
            fprintf(daf, "%d\n", n_n_data_array);
            for ( i= 0; i < n_n_data_array; ++i )
            {
                fprintf(daf, "%d ", n_data_array[i]);
                for ( j= 0; j < n_data_array[i]; ++j )
                    fprintf(daf, "%f ", data_array[i][j]);
                fprintf(daf, "\n");
            }
            fclose(daf);
            FILE* def= fopen("dates_array", "wt");
            fprintf(def, "%d\n", n_n_dates_array);
            for ( i= 0; i < n_n_dates_array; ++i )
            {
                fprintf(def, "%d ", n_dates_array[i]);
                for ( j= 0; j < n_dates_array[i]; ++j )
                    fprintf(def, "%d ", dates_array[i][j]);
                fprintf(def, "\n");
            }
            fclose(def);
            printArray2DFile("new_spatial_dist", new_spatial_dist, n_new_spatial_dist1, n_new_spatial_dist2);
            printArrayFloatFile("map_dist", map_dist, n_map_dist);
            printArrayIFile("near_index", near_index, n_near_index);
            printArrayRFile("t_res", t_res, n_t_res);
            printArrayRFile("b_res", b_res, n_b_res);
            tprintf("sigma: %f, sigma_full: %f\n", sigma, sigma_full);
        }
        // Build the matrices used to solve the contraint problem
        buildMatrices(data_array, n_data_array, n_n_data_array,
                      dates_array, n_dates_array, n_n_dates_array,
                      new_spatial_dist, n_new_spatial_dist1, n_new_spatial_dist2,
                      map_dist, n_map_dist,
                      near_index, n_near_index,
                      t_res, n_t_res,
                      b_res, n_b_res,
                      sigma, sigma_full, first, options,

                      &base_weights, &n_base_weights,
                      &base_constants, &n_base_constants,
                      &temperature_map, &n_temperature_map,
                      &temperature_constant, &n_temperature_constant,
                      &record_weight, &n_record_weight);

        int n_temperature_map1, n_temperature_map2;
        n_temperature_map1= n_temperature_map2= n_temperature_map;

        tprintf("base_weights: %d\n", n_base_weights);
        printArrayDFile("base_weights", base_weights, n_base_weights);
        tprintf("base_constants: %d\n", n_base_constants);
        printArrayDFile("base_constants", base_constants, n_base_constants);
        tprintf("temperature_map: %d %d\n", n_temperature_map, n_temperature_map);
        printArray2DFile("temperature_map", temperature_map, n_temperature_map, n_temperature_map);
        tprintf("temperature_constant: %d\n", n_temperature_constant);
        printArrayDFile("temperature_constant", temperature_constant, n_temperature_constant);
        tprintf("record_weight: %d\n", n_record_weight);
        printArrayDFile("record_weight", record_weight, n_record_weight);

        getchar();
        tprintf("occurance_table: %d,%d\n", n_occurance_table1, n_occurance_table2);
        tprintf("all_station_mix: %d\n", n_all_station_mix);

        for ( i= 0; i < n_base_weights; ++i )
            if ( base_weights[i] == 0 )
                base_weights[i]= 1;

        double* base_map= dnalloc(n_occurance_table1*n_occurance_table2);
        int n_base_map1= n_occurance_table1;
        int n_base_map2= n_occurance_table2;
        for ( i= 0; i < n_occurance_table1; ++i )
            for ( j= 0; j < n_occurance_table2; ++j )
                base_map[i*n_occurance_table2 + j]= occurance_table[i*n_occurance_table2 + j]/base_weights[i];

        if ( !first )
        {
            // Times at which the original fit failed can't be considered
            // because there is no baseline comparison data.
            // TODO
        }

        for ( i= 0; i < n_base_constants; ++i )
            base_constants[i]/= base_weights[i];

        printArray2DFile("base_map", base_map, n_base_map1, n_base_map2);
        printArrayDFile("base_constants", base_constants, n_base_constants);
        tprintf("base_map: %d, %d; base_constants: %d\n", n_base_map1, n_base_map2, n_base_constants);
        getchar();

        free(t_res);
        free(b_res);
        // Determine the temperature and baseline values
        performFit(base_map, n_base_map1, n_base_map2,
                   base_constants, n_base_constants,
                   temperature_map, n_temperature_map1, n_temperature_map2,
                   temperature_constant, n_temperature_constant,
                   new_spatial_table, n_new_spatial_table1, n_new_spatial_table2,
                   all_station_mix, n_all_station_mix,

                   &t_res, &n_t_res, &b_res, &n_b_res);

        for ( i= 0; i < n_t_res; ++i )
            printf("%f ", t_res[i]);
        printf("\n");
        for ( i= 0; i < n_b_res; ++i )
            printf("%f ", b_res[i]);
        printf("\n");
        getchar();

        if ( options->fullBaselineMapping )
        {
            // TODO
        }

        tprintf("Begin of Score Resulting Fit\n");

        // Replace solution with seeded values if requested. We do this for
        // multiple interations in order to allow the quality of fit parameters
        // to become consistent with this seed.

        if ( options->useSeed && loop <= 3 )
        {
            // TODO
        }

        res2= rnalloc(n_t_res + n_b_res);
        n_res2= n_t_res + n_b_res;
        for ( i= 0; i < n_t_res; ++i )
            res2[i]= t_res[i];
        for ( ; i < n_t_res + n_b_res; ++i )
            res2[i]= b_res[i - n_t_res];

        printArrayR("res2", res2, n_res2);

        // Measure the global quality of fit
        real* base_adjustment_scale;
        int n_base_adjustment_scale1;
        int n_base_adjustment_scale2;
        double ssd;
        int data_points;

        printArrayRFile("t_res", t_res, n_t_res);
        printArrayRFile("b_res", b_res, n_b_res);
        printArray2DFile("new_spatial_dist", new_spatial_dist, n_new_spatial_dist1, n_new_spatial_dist2);
        printArrayDFile("map_dist", map_dist, n_map_dist);
        printArrayIFile("near_index", near_index, n_near_index);

        scoreFit(data_array, n_data_array, n_n_data_array,
                 dates_array, n_dates_array, n_n_dates_array,
                 t_res, n_t_res,
                 b_res, n_b_res,
                 new_spatial_dist, n_new_spatial_dist1, n_new_spatial_dist2,
                 map_dist, n_map_dist,
                 near_index, n_near_index,
                 first, sigma, sigma_full, options,

                 &ssd,
                 &base_adjustment_scale, &n_base_adjustment_scale1, &n_base_adjustment_scale2,
                 &data_points);

        tprintf("ssd: %f, data_points: %d\n", ssd, data_points);
        printArray2R("base_adjustment_scale", base_adjustment_scale, n_base_adjustment_scale1, n_base_adjustment_scale2);
        getchar();

        double n_isnan_res2= 0;
        for ( i= 0; i < n_res2; ++i )
            if ( res2[i] != -FLT_MAX )
                n_isnan_res2+= 1.0;
        results->quality_of_fit= ssd/(data_points - n_isnan_res2);
        results->data_points= data_points;

        double n_isnan_t_res= 0;
        for ( i= 0; i < n_t_res; ++i )
            if ( t_res[i] != -FLT_MAX )
                n_isnan_t_res+= 1.0;
        double n_isnan_b_res= 0;
        for ( i= 0; i < n_b_res; ++i )
            if ( b_res[i] != -FLT_MAX )
                n_isnan_b_res+= 1.0;

        results->time_parameters= n_isnan_t_res;
        results->baseline_parameters= n_isnan_b_res;

        tprintf("End of Score Resulting Fit\n");

        // This optional section backs out scalpel breaks if the associated
        // change in baseline is determined to be statistically insignificant.
        // While potentially desirable, this operation is computationally
        // expensive and generally has no impact on large scale averages, since
        // the inclusion/exclusion of a statistically insignificant break can
        // be anticipated to have no statistically significant effect on the
        // outcome.

        if ( options->removeInsignificantBreaks )
        {
            // TODO
        }

        free(temperature_map);
        free(base_map);

        if ( options->localMode )
        {
            // Create map of local anomaly field
            // TODO
        }

        // Tests for result convergence
        if ( (loop >= 2 && !options->useSeed) || loop >= 4 )
        {
            // Check for convergence
            // TODO
        }
        else
        {
            first= false;
            max_change[loop-1]= -FLT_MAX;
        }

        for ( i= 0; i < n_res2; ++i )
            results_list[i*n_results_list2 + loop - 1]= res2[i];

        if ( options->fullBaselineMapping )
        {
            // TODO
        }

        // Apply a modification of Broyden's method to determine the values for
        // temperature and baseline to use for the next iteration
        //
        // We use two different approximations of the Jacobian and average the
        // predictions they make. This eliminates a bifurcation instability,
        // though it may converge somewhat slower.

        if ( loop >= 3 && options->useBroydenMethod )
        {
            // TODO
        }

        if ( options->useIterativeReweighting )
            tprintf("End of Averaging Loop %d\n", loop);
    }

    free(base_weights);
    free(base_constants);
    free(temperature_constant);
    if ( new_spatial_table )
        free(new_spatial_table);
    if ( new_spatial_dist )
        free(new_spatial_dist);
    free(spatial_table);
    //free(spatial_reweight);

    tprintf("End of Compute Temperature Fit\n");

    // Store baseline and climatology anomaly information.
    real* baseline= rnalloc(orig_length);
    int n_baseline= orig_length;
    for ( i= 0; i < n_baseline; ++i )
        baseline[i]= -FLT_MAX;

    k= 0;
    for ( i= 0; i < n_orig_map; ++i )
        if ( orig_map[i] )
            baseline[i]= res2[len_t + k++];

    if ( options->fullBaselineMapping )
    {
        // TODO
    }

    // Store information on the total weight each record recieves

    real* record_weights= rnalloc(orig_length);
    int n_record_weights= orig_length;
    double sum_record_weight= 0;
    for ( i= 0; i < n_record_weight; ++i )
        if ( record_weight[i] != -FLT_MAX )
            sum_record_weight+= record_weight[i];
    for ( i= 0; i < n_record_weights; ++i )
        record_weights[i]= -FLT_MAX;
    l= 0;
    for ( i= 0; i < n_record_weights; ++i )
        if ( orig_map[i] )
            record_weights[i]= record_weight[l++]/sum_record_weight;

    results->record_weights= record_weights;
    results->n_record_weights= n_record_weights;

    real* site_weights= rnalloc(orig_length);
    int n_site_weights= orig_length;
    l= 0;
    for ( i= 0; i < n_orig_map; ++i )
        if ( orig_map[i] )
            site_weights[i]= site_weight[l++];

    results->site_weights= site_weights;
    results->n_site_weights= n_site_weights;

    // Store results in result structure

    for ( i= 0; i < n_t_res; ++i )
        printf("%f ", t_res[i]);
    printf("\n");

    results->times_monthly= time_values;
    results->n_times_monthly= n_time_values;
    results->values_monthly= t_res;
    results->n_values_monthly= n_t_res;

    simpleAnnualMovingAverageRR(time_values, n_time_values, t_res, n_t_res,
                                &(results->times_annual), &(results->n_times_annual),
                                &(results->values_annual), &(results->n_values_annual));

    simpleMovingAverageRR(time_values, n_time_values, t_res, n_t_res, 60,
                          &(results->times_five_year), &(results->n_times_five_year),
                          &(results->values_five_year), &(results->n_values_five_year));

    simpleMovingAverageRR(time_values, n_time_values, t_res, n_t_res, 120,
                          &(results->times_ten_year), &(results->n_times_ten_year),
                          &(results->values_ten_year), &(results->n_values_ten_year));

    simpleMovingAverageRR(time_values, n_time_values, t_res, n_t_res, 240,
                          &(results->times_twenty_year), &(results->n_times_twenty_year),
                          &(results->values_twenty_year), &(results->n_values_twenty_year));

    results->baselines= baseline;
    results->n_baselines= n_baseline;

    if ( options->fullBaselineMapping )
    {
        // TODO
    }

    results->coverage_summary= coverage_summary;
    results->n_coverage_summary= n_coverage_summary;
    results->location_pts= locations;
    results->n_location_pts= n_locations;
    results->occurance_table= occurance_table;
    results->n_occurance_table1= n_occurance_table1;
    results->n_occurance_table2= n_occurance_table2;

    // Store map data, if appropriate
    if ( options->localMode )
    {
        // TODO
    }
    if ( options->fullBaselineMapping )
    {
        // TODO
    }

    // Information on the convergence process.
    if ( options->useIterativeReweighting )
    {
        // TODO
    }

    tprintf("End of Berkeley Average Core\n");

    return results;
}
