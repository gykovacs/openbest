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
#include "openbest-ds/printOut.h"
#include "openbest-io/loadCountryNames.h"
#include "openbest-io/loadStationSourceTypes.h"
#include "openbest-io/loadCountryCodes.h"
#include "openbest-io/loadCountryNames.h"
#include "openbest-io/loadDataFlags.h"
#include "openbest-io/loadStationRecordTypes.h"
#include "openbest-io/loadOtherFlags.h"
#include "openbest-io/loadData.h"
#include "openbest-av/berkeleyAverage.h"
#include "openbest-av/berkeleyAverageOptions.h"
#include "openbest-av/scalpelMethods.h"

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

    loadStationElement2(&se, &n_stationElement2);

    tprintf("start checking multivaluedness...\n");
    int i;
    for ( i= 0; i < n_stationElement2; ++i )
        if ( isMultiValued(se[i]) )
            tprintf("%d %d\n", i, se[i]->site);

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

    //int i;
    //for ( i= 0; i < n_stationElement2; ++i )
    int n= 24;
    printf("%d\n", isMultiValued(se[n]));
    stationElement2p sv;
    sv= makeSingleValued(se[n], NULL, 0);

    displaySE2(sv);

    destroySE2(sv);

    finalizeDS();
    destroySE2V(se, n_stationElement2);

    return 0;
}

int makeSingleValuedAllTest(int argc, char** argv)
{
    initDS();
    stationElement2p* se;
    stationElement2p* ses;
    int n_stationElement2;
    loadPreliminaryData();
    loadStationElement2(&se, &n_stationElement2);
    ses= (stationElement2p*)malloc(sizeof(stationElement2p)*n_stationElement2);

    printf("%d\n", n_stationElement2);
    //getchar();

    int i;
    stationElement2p sv;
    for ( i= 0; i < n_stationElement2; ++i )
    {
        if ( isMultiValued(se[i]) )
            tprintf("================= WORKING ON %d =================\n", i);
        sv= makeSingleValued(se[i], NULL, 0);
        //displaySE2(se[i]);
        ses[i]= sv;
    }

    tprintf("================= FINISHED ================\n");

    for ( i= 0; i < n_stationElement2; ++i )
            tprintf("%d: %d %d - %d %d\n", i, isMultiValued(se[i]), isMultiValued(ses[i]), se[i]->n_dates, ses[i]->n_dates);

    finalizeDS();
    destroySE2V(se, n_stationElement2);

    return 0;
}

int splitStationBreaksTest(int argc, char** argv)
{
    initDS();
    stationElement2p* se;
    int n_stationElement2;
    stationSite2p* ss;
    int n_stationSite2;

    loadData(&ss, &n_stationSite2, &se, &n_stationElement2);

    printf("%d %d\n", n_stationSite2, n_stationElement2);

    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();

    int* a;
    int* b;

    splitStationBreaks(&se, &n_stationElement2, &ss, &n_stationSite2, bao->scalpelGapLength, iBadFlags, n_badFlags, &a, &b);

    printf("aaa\n"); fflush(stdout);
    int i;
    /*for ( i= 0; i < n_stationElement2; ++i )
    {
        tprintf("=========%d\n", i);
        displaySE2(se[i]);
    }*/
    printf("%d %d\n", n_stationElement2, n_stationSite2);
    /*displaySE2(se[3360]);
    destroySE2(se[3360]);
    getchar();*/
    printf("ccc\n"); fflush(stdout);
    destroySE2V(se, n_stationElement2);
    printf("bbb\n"); fflush(stdout);
    destroySS2V(ss, n_stationSite2);
    printf("ddd\n"); fflush(stdout);
    free(a);
    free(b);
    finalizeDS();
    destroyBAO(bao);

    return 0;
}

int splitStationMovesTest(int argc, char** argv)
{
    initDS();
    stationElement2p* se;
    int n_stationElement2;
    stationSite2p* ss;
    int n_stationSite2;

    loadData(&ss, &n_stationSite2, &se, &n_stationElement2);

    printf("%d %d\n", n_stationSite2, n_stationElement2);
    getchar();

    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();

    int* a;
    int* b;

    splitStationMoves(&se, &n_stationElement2, &ss, &n_stationSite2, true, true, &a, &b);

    printf("aaa\n"); fflush(stdout);
    int i;
    /*for ( i= 0; i < n_stationElement2; ++i )
    {
        tprintf("=========%d\n", i);
        displaySE2(se[i]);
    }*/
    printf("%d %d\n", n_stationElement2, n_stationSite2);
    getchar();
    /*displaySE2(se[3360]);
    destroySE2(se[3360]);
    getchar();*/
    printf("ccc\n"); fflush(stdout);
    destroySE2V(se, n_stationElement2);
    printf("bbb\n"); fflush(stdout);
    destroySS2V(ss, n_stationSite2);
    printf("ddd\n"); fflush(stdout);
    free(a);
    free(b);
    finalizeDS();
    destroyBAO(bao);

    return 0;
}

int splitStationTOBChangesTest(int argc, char** argv)
{
    initDS();
    stationElement2p* se;
    int n_stationElement2;
    stationSite2p* ss;
    int n_stationSite2;

    loadData(&ss, &n_stationSite2, &se, &n_stationElement2);

    printf("%d %d\n", n_stationSite2, n_stationElement2);
    getchar();

    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();

    int* a;
    int* b;

    splitStationTOBChanges(&se, &n_stationElement2, &ss, &n_stationSite2, 0.5, iBadFlags, n_badFlags, 4, &a, &b);

    printf("aaa\n"); fflush(stdout);
    int i;
    /*for ( i= 0; i < n_stationElement2; ++i )
    {
        tprintf("=========%d\n", i);
        displaySE2(se[i]);
    }*/
    printf("%d %d\n", n_stationElement2, n_stationSite2);
    getchar();
    /*displaySE2(se[3360]);
    destroySE2(se[3360]);
    getchar();*/
    printf("ccc\n"); fflush(stdout);
    destroySE2V(se, n_stationElement2);
    printf("bbb\n"); fflush(stdout);
    destroySS2V(ss, n_stationSite2);
    printf("ddd\n"); fflush(stdout);
    free(a);
    free(b);
    finalizeDS();
    destroyBAO(bao);

    return 0;
}

int splitStationAllTest(int argc, char** argv)
{
    initDS();
    stationElement2p* se;
    int n_stationElement2;
    stationSite2p* ss;
    int n_stationSite2;

    loadData(&ss, &n_stationSite2, &se, &n_stationElement2);

    printf("%d %d\n", n_stationSite2, n_stationElement2);
    getchar();

    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();

    int* a;
    int* b;

    splitStationBreaks(&se, &n_stationElement2, &ss, &n_stationSite2, bao->scalpelGapLength, iBadFlags, n_badFlags, &a, &b);
    splitStationMoves(&se, &n_stationElement2, &ss, &n_stationSite2, true, true, &a, &b);
    splitStationTOBChanges(&se, &n_stationElement2, &ss, &n_stationSite2, 0.5, iBadFlags, n_badFlags, 4, &a, &b);

    printf("aaa\n"); fflush(stdout);
    int i;
    /*for ( i= 0; i < n_stationElement2; ++i )
    {
        tprintf("=========%d\n", i);
        displaySE2(se[i]);
    }*/
    printf("%d %d\n", n_stationElement2, n_stationSite2);
    getchar();
    /*displaySE2(se[3360]);
    destroySE2(se[3360]);
    getchar();*/
    printf("ccc\n"); fflush(stdout);
    destroySE2V(se, n_stationElement2);
    printf("bbb\n"); fflush(stdout);
    destroySS2V(ss, n_stationSite2);
    printf("ddd\n"); fflush(stdout);
    free(a);
    free(b);
    finalizeDS();
    destroyBAO(bao);

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
    bool ssbtest= false;
    bool ssmtest= false;
    bool ssttest= false;
    bool ssatest= false;
    bool mksingleall= false;
    int err;
    
    addOption(ot, "--shrink", OPTION_BOOL, (char*)&shrink, 0, "shrink station site vector");
    addOption(ot, "--issingle", OPTION_BOOL, (char*)&issingle, 0, "isSingleValued test");
    addOption(ot, "--mksingle", OPTION_BOOL, (char*)&mksingle, 0, "makeSingleValued test");
    addOption(ot, "--mksingleall", OPTION_BOOL, (char*)&mksingleall, 0, "makeSingleValued all loaded data test");
    addOption(ot, "--ssb", OPTION_BOOL, (char*)&ssbtest, 0, "split station breaks test");
    addOption(ot, "--ssm", OPTION_BOOL, (char*)&ssmtest, 0, "split station moves test");
    addOption(ot, "--sst", OPTION_BOOL, (char*)&ssttest, 0, "split station tob test");
    addOption(ot, "--ssa", OPTION_BOOL, (char*)&ssatest, 0, "split station all test");
    
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
    else if ( ssbtest )
        err= splitStationBreaksTest(argc, argv);
    else if ( ssmtest )
        err= splitStationMovesTest(argc, argv);
    else if ( ssttest )
        err= splitStationTOBChangesTest(argc, argv);
    else if ( ssatest )
        err= splitStationAllTest(argc, argv);
    else if ( mksingleall )
        err= makeSingleValuedAllTest(argc, argv);
    
    destroyOptionTable(ot);

    return err;
}
