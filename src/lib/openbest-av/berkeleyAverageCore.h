#ifndef _BERKELEY_AVERAGE_CORE_H_
#define _BERKELEY_AVERAGE_CORE_H_

#include "openbest-av/berkeleyAverageOptions.h"
#include "openbest-ds/stationSite2.h"
#include "openbest-ds/stationElement2.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/basicAlgorithms.h"

/**
  * berkeleyAvrageCore process
  * @param seIO pointer to an array of stationElement2 object pointers
  * @param n_seIO pointer to the length of seIO array
  * @param ssIO pointer to an array of stationElement2 object pointers
  * @param n_ssIO pointer to the length of ssIO array
  * @param options berkeleyAverageOptions object pointer
  */
void berkeleyAverageCore(stationElement2p** seIO, int* n_seIO, stationSite2p** ssIO, int* n_ssIO, berkeleyAverageOptions* options);

#endif
