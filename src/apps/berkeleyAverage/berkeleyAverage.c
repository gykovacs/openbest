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
            printf("%f %f\n", results->times_monthly[i], results->values_monthly[i]);
    }
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

/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();
    
    setPrefix(ot, "berkeleyAverage application");
    addUsage(ot, "berkeleyAverage <option-name> arg1 arg2 ...");
    
    bool berkeleyav;
    
    addOption(ot, "--berkeleyav", OPTION_BOOL, (char*)&berkeleyav, 0, "run berkeley average algorithm");
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
    
    destroyOptionTable(ot);

    return err;
}
