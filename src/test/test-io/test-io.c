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

int stationSourceTest(int argc, char** argv)
{
  initDS();
  
  loadStationSourceTypesPKT();
  
  displayPKTbyValue(stationSourceTypesPKT);

  finalizeDS();
  
  return 0;
}

int countryNamesTest(int argc, char** argv)
{
    initDS();

    loadCountryNamesPKT();

    displayPKTbyValue(countryNamesPKT);

    finalizeDS();

    return 0;
}

int countryCodesTest(int argc, char** argv)
{
    initDS();

    loadCountryCodesPKT();

    displayPKTbyValue(countryCodesPKT);

    finalizeDS();

    return 0;
}

int dataFlagsTest(int argc, char** argv)
{
    initDS();

    loadDataFlagsPKT();

    displayPKTbyValue(dataFlagsPKT);

    finalizeDS();

    return 0;
}

int siteFlagsTest(int argc, char** argv)
{
    initDS();

    loadSiteFlagsPKT();

    displayPKTbyValue(siteFlagsPKT);

    finalizeDS();

    return 0;
}

int stationRecordTypesTest(int argc, char** argv)
{
    initDS();

    loadStationRecordTypesPKT();

    displayPKTbyValue(stationRecordTypesPKT);

    finalizeDS();

    return 0;
}

int otherFlagsTest(int argc, char** argv)
{
    initDS();

    loadDataFlagsPKT();
    loadOtherFlags();

    int i;
    printf("n: %d\n", n_badFlags);
    for ( i= 0; i < n_badFlags; ++i )
        printf("%s\n", badFlags[i]);
    printf("\n");
    for ( i= 0; i < n_sourceRecordTypes; ++i )
        printf("%s\n", sourceRecordTypes[i]);
    printf("\n");
    for ( i= 0; i < n_sourceFileCodes; ++i )
        printf("%s %d\n", sourceFileCodes[i], monthly[i]);

    finalizeDS();

    return 0;
}

int matchFlagsTest(int argc, char** argv)
{
    initDS();

    loadMatchFlagsPKT();

    displayPKTbyValue(matchFlagsPKT);

    finalizeDS();

    return 0;
}

int loadSSTest(int argc, char** argv)
{
    initDS();
    stationSite2p** ss;
    int n_stationSite2;
    printf("start loading...\n"); fflush(stdout);
    loadPreliminaryData();
    loadStationSite2(&ss, &n_stationSite2);
    printf("finish loading...\n"); fflush(stdout);
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
    return 0;
}

int loadSETest(int argc, char** argv)
{
    initDS();
    stationElement2p** se;
    int n_stationElement2;
    printf("start loading...\n"); fflush(stdout);
    loadPreliminaryData();
    loadStationElement2(&se, &n_stationElement2);
    printf("finish loading...\n"); fflush(stdout);
    int id;
    do
    {
        printf("give the number of record to display (-1 to exit):\n");
        scanf("%d", &id);
        if ( id == -1 )
            break;
        displaySE2(se[id]);
    }while ( 1 );
    finalizeDS();
    return 0;
}

int loadAllTest(int argc, char** argv)
{
    initDS();
    stationElement2p** se;
    stationSite2p** ss;
    int n_stationElement2;
    int n_stationSite2;

    tprintf("start loading...\n"); fflush(stdout);
    loadData(ss, &n_stationSite2, se, &n_stationElement2);
    tprintf("finish loading...\n"); fflush(stdout);

    finalizeDS();
    return 0;
}

int loadPrelimTest(int argc, char** argv)
{
    initDS();
    loadPreliminaryData();
    displayPKTbyValue(stationSourceTypesPKT);
    finalizeDS();

    return 0;
}

/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();
    
    setPrefix(ot, "openbest i/o tests");
    addUsage(ot, "test-ds <option-name> arg1 arg2 ...");
    
    bool stationSource= false;
    bool countryNames= false;
    bool countryCodes= false;
    bool dataFlags= false;
    bool siteFlags= false;
    bool stationRecordTypes= false;
    bool otherFlags= false;
    bool matchFlags= false;
    bool loadPrelim= false;
    bool loadss= false;
    bool loadse= false;
    bool loadall= false;
    int err;
    
    addOption(ot, "--stationSourceTypes", OPTION_BOOL, (char*)&stationSource, 0, "station source types");
    addOption(ot, "--countryNames", OPTION_BOOL, (char*)&countryNames, 0, "country names");
    addOption(ot, "--countryCodes", OPTION_BOOL, (char*)&countryCodes, 0, "country codes");
    addOption(ot, "--dataFlags", OPTION_BOOL, (char*)&dataFlags, 0, "data flags");
    addOption(ot, "--siteFlags", OPTION_BOOL, (char*)&siteFlags, 0, "site flags");
    addOption(ot, "--stationRecordTypes", OPTION_BOOL, (char*)&stationRecordTypes, 0, "station record types");
    addOption(ot, "--otherFlags", OPTION_BOOL, (char*)&otherFlags, 0, "other flags");
    addOption(ot, "--matchFlags", OPTION_BOOL, (char*)&matchFlags, 0, "match flags");
    addOption(ot, "--loadPrelim", OPTION_BOOL, (char*)&loadPrelim, 0, "load preliminary data");
    addOption(ot, "--loadSS", OPTION_BOOL, (char*)&loadss, 0, "load station sites");
    addOption(ot, "--loadSE", OPTION_BOOL, (char*)&loadse, 0, "load station elements");
    addOption(ot, "--loadAll", OPTION_BOOL, (char*)&loadall, 0, "load all data");
    
    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }
    
    if ( stationSource )
        err= stationSourceTest(argc, argv);
    else if ( countryNames )
        err= countryNamesTest(argc, argv);
    else if ( countryCodes )
        err= countryCodesTest(argc, argv);
    else if ( dataFlags )
        err= dataFlagsTest(argc, argv);
    else if ( siteFlags )
        err= siteFlagsTest(argc, argv);
    else if ( stationRecordTypes )
        err= stationRecordTypesTest(argc, argv);
    else if ( otherFlags )
        err= otherFlagsTest(argc, argv);
    else if ( matchFlags )
        err= matchFlagsTest(argc, argv);
    else if ( loadPrelim )
        err= loadPrelimTest(argc, argv);
    else if ( loadss )
        err= loadSSTest(argc, argv);
    else if ( loadse )
        err= loadSETest(argc, argv);
    else if ( loadall )
        err= loadAllTest(argc, argv);
    
    destroyOptionTable(ot);

    return err;
}
