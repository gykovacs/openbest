#include <stdio.h>

#include "openbest-ds/init-ds.h"
#include "openbest-ds/optionTable.h"
#include "openbest-io/loadData.h"
#include "openbest-av/scalpelMethods.h"
#include "openbest-av/berkeleyAverage.h"
#include "openbest-av/berkeleyAverageOptions.h"

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

    berkeleyAverageOptions* bao= createBerkeleyAverageOptionsQuick();

    berkeleyAverage(&se, &n_stationElement2, &ss, &n_stationSite2, bao);
    /*int* a, *b;

    splitStationBreaks(&se, &n_stationElement2, &ss, &n_stationSite2, bao->scalpelGapLength, bao->badFlags, bao->n_badFlags, &a, &b);
    free(a);
    free(b);
    splitStationMoves(&se, &n_stationElement2, &ss, &n_stationSite2, bao->scalpelDeclaredMoves, bao->scalpelSuspectedMoves, &a, &b);
    free(a);
    free(b);
    splitStationTOBChanges(&se, &n_stationElement2, &ss, &n_stationSite2, bao->scalpelTOBPersistence, bao->badFlags, bao->n_badFlags, bao->scalpelTOBDifference, &a, &b);

    free(a);
    free(b);*/

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
