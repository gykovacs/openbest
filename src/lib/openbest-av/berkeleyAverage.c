#include "openbest-av/berkeleyAverage.h"

void berkeleyAverage(stationElement2p* se, int n_stationElement2, stationSite2p* ss, int n_stationSite2, berkeleyAverageOptions* options)
{
    tprintf("Berkeley Average\n");

    tprintf("Called with %d temperature time series from %d unique sites\n", n_stationElement2, n_stationSite2);

    displayBAO(options);

    if ( options->useScalpel )
    {
        tprintf("Berkeley Average Scalpel Methods\n");

        /*if ( options->scalpelGaps )
            splitStationBreaks(se, n_stationElement2, ss, n_stationSite2, options->scalpelGapLength, options->badFlags, options->n_badFlags);*/
    }
}
