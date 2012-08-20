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
    printf("initial_time_series: %d\n", results->initial_time_series);
    printf("post_scalpel_time_series: %d\n", results->post_scalpel_time_series);
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

    loadPreliminaryData();

    berkeleyAverageOptions* bao;

    if ( quick )
        bao= createBerkeleyAverageOptionsQuick();

    stationElement2p tmp;

    tprintf("reading station elements...\n");

    FILE* fse= fopen(argv[1], "r");
    int n;
    fscanf(fse, "%d", &n);
    se= (stationElement2**)malloc(sizeof(stationElement2*)*n);
    int i, j, k, m;
    float a, b, c;
    int d, e, f;
    char line[1000];
    for ( i= 0; i < n; ++i )
    {
        fscanf(fse, "%d\n", &m);
        se[i]= createSE2NM(m);

        for ( j= 0; j < m; ++j )
        {
            fgets(line, 1000, fse);
            f= sscanf(line, "%f %f %f %d %d", &a, &b, &c, &d, &e);
            se[i]->dates[j]= a;
            se[i]->data[j]= b;
            se[i]->uncertainty[j]= c;
            if ( f > 3 )
                se[i]->time_of_observation[j]= d;
            else
                se[i]->time_of_observation[j]= -99;
            if ( f > 4 )
                se[i]->num_measurements[j]= e;
            else
                se[i]->num_measurements[j]= -99;
        }
    }
    fclose(fse);

    tprintf("reading station sites...\n");
    int nn;
    int tmps;
    fse= fopen(argv[2], "r");
    fscanf(fse, "%d", &nn);
    ss= (stationSite2**)malloc(sizeof(stationSite2*)*nn);
    for ( i= 0; i < nn; ++i )
    {
        ss[i]= createSS2N();
        k= fscanf(fse, "%f %f %f", &a, &b, &c);
        if ( k == 3 && c == c )
            ss[i]->location= createGeoPoint23(a, b, c);
        else
        {
            ss[i]->location= createGeoPoint22(a, b);
        }
        fscanf(fse, "%d\n", &d);
        ss[i]->relocations= rnalloc(d);
        ss[i]->relocation_types= fnalloc(d);
        if ( d > 0 )
        {
            fgets(line, 1000, fse);
            tmps= 0;
            for ( j= 0; j < d; ++j )
            {
                sscanf(line + tmps, "%f%n", &a, &tmps);
                ss[i]->relocations[j]= a;
                ss[i]->relocation_types[j]= DECLARED_MOVE;
            }
        }
        fscanf(fse, "%d\n", &e);
        if ( e > 0 )
        {
            ss[i]->relocations= (real*)realloc(ss[i]->relocations, sizeof(real)*(d + e));
            ss[i]->relocation_types= (flag_t*)realloc(ss[i]->relocation_types, sizeof(flag_t)*(d + e));
            fgets(line, 1000, fse);
            tmps= 0;
            for ( j= 0; j < e; ++j )
            {
                sscanf(line + tmps, "%f%n", &a, &tmps);
                ss[i]->relocations[j]= a;
                ss[i]->relocation_types[j]= SUSPECTED_MOVE;
            }
        }
        ss[i]->n_relocations= d + e;
    }
    fclose(fse);

    n_stationElement2= n;
    n_stationSite2= nn;

    tprintf("reading finished\n");

    tprintf("Making dataset to be single valued\n");
    /*for ( i= 0; i < n_stationElement2; ++i )
    {
        tmp= makeSingleValued(se[i], NULL, 0);
        destroySE2(se[i]);
        se[i]= tmp;
    }*/
    tprintf("Converting to single valued finished\n");

    berkeleyAverageResults* results;

    bao->useScalpel=true;
    bao->scalpelGaps= false;
    bao->scalpelDeclaredMoves= false;
    bao->scalpelSuspectedMoves= false;
    bao->scalpelTOBChanges=false;
    bao->scalpelMetadata= false;

    results= berkeleyAverage(&se, &n_stationElement2, &ss, &n_stationSite2, bao);

    processResults(results);

    destroyBAR(results);

    destroySE2V(se, n_stationElement2);

    destroySS2V(ss, n_stationSite2);

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
    
    bool berkeleyav= false;
    bool berkeleyavtest= false;
    
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
