#include <stdio.h>

#include "openbest-ds/init-ds.h"
#include "openbest-ds/optionTable.h"
#include "openbest-io/loadData.h"
#include "openbest-ds/makeSingleValued.h"
#include "openbest-av/scalpelMethods.h"
#include "openbest-av/berkeleyAverage.h"
#include "openbest-av/berkeleyAverageOptions.h"
#include "openbest-ds/printOut.h"

int boptionsTest(int argc, char** argv)
{
    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();
    berkeleyAverageOptionsConsistency(bao);
    return 0;
}

int displayBAOTest(int argc, char** argv)
{
    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();
    displayBAO(bao);
    return 0;
}

int berkeleyAverageTest(int argc, char** argv)
{
    initDS();

    stationSite2p* ss;
    int n_stationSite2;
    stationElement2p* se;
    int n_stationElement2;

    loadData(&ss, &n_stationSite2, &se, &n_stationElement2);

    int j;
    for ( j= 0; j < n_badFlags; ++j )
        printf("%d ", iBadFlags[j]);

    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();

    int i;
    stationElement2p tmp;
    tprintf("Making dataset to be single valued\n");
    for ( i= 0; i < n_stationElement2; ++i )
    {
        tmp= makeSingleValued(se[i], NULL, 0);
        destroySE2(se[i]);
        se[i]= tmp;
    }
    tprintf("Converting to single valued finished\n");

    berkeleyAverage(&se, &n_stationElement2, &ss, &n_stationSite2, bao);

    destroySE2V(se, n_stationElement2);
    destroySS2V(ss, n_stationSite2);

    finalizeDS();

    destroyBAO(bao);

    return 0;
}

int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();

    setPrefix(ot, "openbest averaging tests");
    addUsage(ot, "test-av <option-name> arg1 arg2 ...");

    bool boptions= false;
    bool displaybao= false;
    bool berkeleyav= false;

    int err;

    addOption(ot, "--boptions", OPTION_BOOL, (char*)&boptions, 0, "BerkeleyAverageOptions test");
    addOption(ot, "--displaybao", OPTION_BOOL, (char*)&displaybao, 0, "displays the berkeleyAverageOptoins object");
    addOption(ot, "--berkeleyav", OPTION_BOOL, (char*)&berkeleyav, 0, "berkeleyAverage test");


    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }

    if ( boptions )
        err= boptionsTest(argc, argv);
    else if ( displaybao )
        err= displayBAOTest(argc, argv);
    else if ( berkeleyav )
        err= berkeleyAverageTest(argc, argv);

    destroyOptionTable(ot);

    return err;
}
