#ifndef _BERKELEY_AVERAGE_H_
#define _BERKELEY_AVERAGE_H_

#include "openbest-ds/stationSite2.h"
#include "openbest-ds/stationElement2.h"
#include "openbest-av/berkeleyAverageOptions.h"
#include "openbest-av/berkeleyAverageResults.h"

/**
  * the main berkeleyAverage method
  * @param se array of stationElement2 object pointers
  * @param n_stationElement2 size of the array of stationElement2 objects
  * @param ss array of stationSite2 object pointers
  * @param n_stationSite2 size of the array of stationSite2 objects
  * @param options pointer to berkeleyAverageOptions instance
  */
berkeleyAverageResults* berkeleyAverage(stationElement2p** se, int* n_stationElement2, stationSite2p** ss, int* n_stationSite2, berkeleyAverageOptions* options);

#endif
