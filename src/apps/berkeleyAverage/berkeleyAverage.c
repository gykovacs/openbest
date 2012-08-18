#include <unistd.h>
#include <stdio.h>

#include "openbest-ds/hello-openbest.h"
#include "openbest-ds/geoPoint.h"
#include "openbest-ds/geoPoint2.h"
#include "openbest-ds/geoPoint2Vector.h"
#include "openbest-ds/matrix3.h"
#include "openbest-ds/timeInstant.h"
#include "openbest-ds/timeRange.h"
#include "openbest-ds/primaryKeyTables.h"
#include "openbest-ds/init-ds.h"
#include "openbest-io/loadCountryNames.h"
#include "openbest-io/loadStationSourceTypes.h"
#include "openbest-io/loadCountryCodes.h"
#include "openbest-io/loadCountryNames.h"
#include "openbest-io/loadDataFlags.h"
#include "openbest-io/loadStationRecordTypes.h"
#include "openbest-io/loadOtherFlags.h"
#include "openbest-io/loadData.h"
#include "openbest-ds/makeSingleValued.h"
#include "openbest-av/scalpelMethods.h"
#include "openbest-ds/printOut.h"
#include "openbest-ds/memFunctions.h"


#include "openbest-ds/optionTable.h"
#include "openbest-av/berkeleyAverage.h"

bool printMonthlyTimeSeries= false;
bool quick= false;

void processResults(berkeleyAverageResults* results)
{
    int i;

    if ( printMonthlyTimeSeries )
    {
        for ( i= 0; i < results->n_times_monthly; ++i )
            fprintf(stderr, "%f %f\n", results->times_monthly[i], results->values_monthly[i]);
    }
    printf("network_completeness: %f\n", results->network_completeness);
    printf("quality_of_fit: %f\n", results->quality_of_fit);
    printf("data_points: %f\n", results->data_points);
    printf("time_parameters: %f\n", results->time_parameters);
    printf("baseline_parameters: %f\n", results->baseline_parameters);
    printf("initial_time_series: %f\n", results->initial_time_series);
    printf("post_scalpel_time_series: %f\n", results->post_scalpel_time_series);
}

int berkeleyAverageFunction(int argc, char** argv)
{
    initDS();

    stationSite2p* ss;
    int n_stationSite2;
    stationElement2p* se;
    int n_stationElement2;

    loadData(&ss, &n_stationSite2, &se, &n_stationElement2);

    berkeleyAverageOptions* bao;

    if ( quick )
        bao= createBerkeleyAverageOptionsQuick();

    int i;
    stationElement2p tmp;


    for ( i= 0; i < n_stationElement2; ++i )
        if ( isMultiValued(se[i]) )
        {
            destroySE2(se[i]);
            destroySS2(ss[i]);
            se[i]= NULL;
            ss[i]= NULL;
        }

    int nse2= 0;
    for ( i= 0; i < n_stationElement2; ++i )
        if ( se[i] != NULL )
        {
            se[nse2]= se[i];
            ss[nse2++]= ss[i];
        }
    n_stationElement2= nse2;
    n_stationSite2= nse2;

    tprintf("Making dataset to be single valued\n");
    /*for ( i= 0; i < n_stationElement2; ++i )
    {
        tmp= makeSingleValued(se[i], NULL, 0);
        destroySE2(se[i]);
        se[i]= tmp;
    }*/
    tprintf("Converting to single valued finished\n");

    berkeleyAverageResults* results;

    results= berkeleyAverage(&se, &n_stationElement2, &ss, &n_stationSite2, bao);

    processResults(results);

    return 0;
}

int berkeleyAverageTestFunction(int argc, char** argv)
{
    initDS();

    stationSite2p* ss;
    int n_stationSite2;
    stationElement2p* se;
    int n_stationElement2;

    loadData(&ss, &n_stationSite2, &se, &n_stationElement2);

    berkeleyAverageOptions* bao;

    if ( quick )
        bao= createBerkeleyAverageOptionsQuick();

    stationElement2p tmp;


    /*for ( i= 0; i < n_stationElement2; ++i )
        if ( isMultiValued(se[i]) )
        {
            destroySE2(se[i]);
            destroySS2(ss[i]);
            se[i]= NULL;
            ss[i]= NULL;
        }

    int nse2= 0;
    for ( i= 0; i < n_stationElement2; ++i )
        if ( se[i] != NULL )
        {
            se[nse2]= se[i];
            ss[nse2++]= ss[i];
        }
    n_stationElement2= nse2;
    n_stationSite2= nse2;*/

    FILE* fse= fopen(argv[1], "r");
    int n;
    fscanf(fse, "%d", &n);
    se= (stationElement2**)malloc(sizeof(stationElement2*)*n);
    int i, j, k, m;
    float a, b, c;
    for ( i= 0; i < n; ++i )
    {
        se[i]= createSE2N();
        fscanf(fse, "%d", &m);
        se[i]->n_data= m;
        se[i]->n_dates= m;
        se[i]->n_time_of_observation= m;
        se[i]->n_uncertainty= m;
        se[i]->n_num_measurements= m;
        se[i]->n_n_flags= m;
        se[i]->n_flags= cnalloc(m);
        setc(se[i]->n_flags, m, 0);
        se[i]->flags= (flag_t**)malloc(sizeof(flag_t*)*m);
        for ( j= 0; j < m; ++j )
            se[i]->flags[j]= NULL;
        se[i]->n_n_sources= m;
        se[i]->n_sources= cnalloc(m);
        se[i]->sources= (flag_t**)malloc(sizeof(flag_t*)*m);
        for ( j= 0; j < m; ++j )
            se[i]->sources[j]= NULL;
        setc(se[i]->n_sources, m, 0);
        se[i]->num_measurements= snalloc(m);
        se[i]->uncertainty= rnalloc(m);
        se[i]->time_of_observation= cnalloc(m);
        se[i]->dates= rnalloc(m);
        se[i]->data= tnalloc(m);

        for ( j= 0; j < m; ++j )
        {
            fscanf(fse, "%f %f", &a, &b);
            se[i]->dates[j]= a;
            se[i]->data[j]= b;
        }
    }
    fclose(fse);

    int nn;
    fse= fopen(argv[2], "r");
    fscanf(fse, "%d", &nn);
    char line[1000];
    ss= (stationSite2**)malloc(sizeof(stationSite2*)*nn);
    for ( i= 0; i < nn; ++i )
    {
        ss[i]= createSS2N();
            k= fscanf(fse, "%f %f %f", &a, &b, &c);
            if ( k == 3 && c == c )
                ss[i]->location= createGeoPoint23(a, b, c);
            else
            {
                printf(".");
                ss[i]->location= createGeoPoint22(a, b);
            }
            printf("%f %f %f %f %f %f\n", ss[i]->location->latitude,
                   ss[i]->location->longitude,
                   ss[i]->location->elevation,
                   ss[i]->location->x,
                   ss[i]->location->y,
                   ss[i]->location->z);
    }
    getchar();
    fclose(fse);

    n_stationElement2= n;
    n_stationSite2= nn;

    tprintf("Making dataset to be single valued\n");
    /*for ( i= 0; i < n_stationElement2; ++i )
    {
        tmp= makeSingleValued(se[i], NULL, 0);
        destroySE2(se[i]);
        se[i]= tmp;
    }*/
    tprintf("Converting to single valued finished\n");

    berkeleyAverageResults* results;

    results= berkeleyAverage(&se, &n_stationElement2, &ss, &n_stationSite2, bao);

    processResults(results);

    return 0;
}

/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();
    
    setPrefix(ot, "berkeleyAverage application");
    addUsage(ot, "berkeleyAverage <option-name> arg1 arg2 ...");
    
    bool berkeleyav;
    bool berkeleyavtest;
    
    addOption(ot, "--berkeleyav", OPTION_BOOL, (char*)&berkeleyav, 0, "run berkeley average algorithm");
    addOption(ot, "--berkeleyavtest", OPTION_BOOL, (char*)&berkeleyavtest, 0, "run berkeley average algorithm");
    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "OPTION PRESETS");
    addOption(ot, "--quick", OPTION_BOOL, (char*)&quick, 0, "quick mode");
    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "OUTPUT PROCESSING");
    addOption(ot, "--pmts", OPTION_BOOL, (char*)&printMonthlyTimeSeries, 0, "print monthly time series to stdout");
    
    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }
    
    int err;
    if ( berkeleyav )
        err= berkeleyAverageFunction(argc, argv);
    else if ( berkeleyavtest )
        err= berkeleyAverageTestFunction(argc, argv);
    
    destroyOptionTable(ot);

    return err;
}
