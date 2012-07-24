#ifndef _BERKELEY_AVERAGE_CORE_H_
#define _BERKELEY_AVERAGE_CORE_H_

#include "openbest-av/berkeleyAverageOptions.h"
#include "openbest-ds/stationSite2.h"
#include "openbest-ds/stationElement2.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/basicAlgorithms.h"

void berkeleyAverageCore(stationElement2p** seIO, int* n_seIO, stationSite2p** ssIO, int* n_ssIO, berkeleyAverageOptions* options);

#endif
