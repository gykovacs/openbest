#include <unistd.h>

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

#include "openbest-ds/optionTable.h"

int shrinkTest(int argc, char** argv)
{
    initDS();
    stationSite2p* ss;
    int n_stationSite2;
    loadPreliminaryData();
    loadStationSite2(&ss, &n_stationSite2);

    printf("%d records loaded\n", n_stationSite2);

    shrinkSS2V(&ss, &n_stationSite2);

    int id;
    do
    {
        printf("give the number of record to display (-1 to exit):\n");
        scanf("%d", &id);
        if ( id == -1 )
            break;
        displaySS2(ss[id]);
    }while ( 1 );

    finalizeDS();
    destroySS2V(ss, n_stationSite2);

    return 0;
}

int singleValuedTest(int argc, char** argv)
{
    initDS();
    stationElement2p* se;
    int n_stationElement2;
    loadPreliminaryData();
    loadStationElement2(&se, &n_stationElement2);

    int i;
    for ( i= 0; i < n_stationElement2; ++i )
        printf("%d", isSingleValued(se[i]));

    finalizeDS();
    destroySE2V(se, n_stationElement2);

    return 0;
}

int makeSingleValuedTest(int argc, char** argv)
{
    initDS();
    stationElement2p* se;
    int n_stationElement2;
    loadPreliminaryData();
    loadStationElement2(&se, &n_stationElement2);

    int i;
    for ( i= 0; i < n_stationElement2; ++i )
        se[i]= makeSingleValued(se[i], NULL, 0);

    finalizeDS();
    destroySE2V(se, n_stationElement2);

    return 0;
}


/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();
    
    setPrefix(ot, "openbest scalpel and filtering tests");
    addUsage(ot, "test-sc <option-name> arg1 arg2 ...");
    
    bool shrink= false;
    bool issingle= false;
    bool mksingle= false;
    int err;
    
    addOption(ot, "--shrink", OPTION_BOOL, (char*)&shrink, 0, "shrink station site vector");
    addOption(ot, "--issingle", OPTION_BOOL, (char*)&issingle, 0, "isSingleValued test");
    addOption(ot, "--mksingle", OPTION_BOOL, (char*)&mksingle, 0, "makeSingleValued test");
    
    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }
    
    if ( shrink )
        err= shrinkTest(argc, argv);
    else if ( issingle )
        err= singleValuedTest(argc, argv);
    else if ( mksingle )
        err= makeSingleValuedTest(argc, argv);
    
    destroyOptionTable(ot);

    return err;
}
