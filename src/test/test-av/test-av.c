#include <stdio.h>

#include "openbest-ds/optionTable.h"
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

int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();

    setPrefix(ot, "openbest averaging tests");
    addUsage(ot, "test-av <option-name> arg1 arg2 ...");

    bool boptions= false;
    bool displaybao= false;

    int err;

    addOption(ot, "--boptions", OPTION_BOOL, (char*)&boptions, 0, "BerkeleyAverageOptions test");
    addOption(ot, "--displaybao", OPTION_BOOL, (char*)&displaybao, 0, "displays the berkeleyAverageOptoins object");


    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }

    if ( boptions )
        err= boptionsTest(argc, argv);
    else if ( displaybao )
        err= displayBAOTest(argc, argv);

    return err;
}
