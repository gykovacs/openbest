#ifndef _LOAD_DATA_H_
#define _LOAD_DATA_H_

#include "openbest-ds/stationElement2.h"
#include "openbest-ds/stationSite2.h"
#include "openbest-ds/init-ds.h"

#include "openbest-io/config-io.h"
#include "openbest-io/loadCountryCodes.h"
#include "openbest-io/loadCountryNames.h"
#include "openbest-io/loadDataFlags.h"
#include "openbest-io/loadMatchFlags.h"
#include "openbest-io/loadSiteFlags.h"
#include "openbest-io/loadStationRecordTypes.h"
#include "openbest-io/loadStationSourceTypes.h"
#include "openbest-io/loadOtherFlags.h"

/**
  * loads countryCodes, countryNames, dataFlags, matchFlags, otherFlags, siteFlags, stationRecordTypes, stationSourceTypes
  */
void loadPreliminaryData();

void loadStationElement2(stationElement2p** se, int* n_stationElement2);

void loadStationSite2(stationSite2p** ss, int* n_stationSite2);

/**
  * loads the station and temperature data from text files
  * @param ss pointer to an empty array of size 0 of stationSite2 pointers; loadData allocates the proper amount of memory
  * @param n_stationSite2 pointer to an integer variable to hold the length of the stationSite2 array loaded
  * @param se pointer to an empty array of size 0 of stationElement2 pointers; loadData allocates the proper amount of memory
  * @param n_stationElement2 pointer to an integer variable to hold the length of the stationElement2 array loaded
  */
void loadData(stationSite2p** ss, int* n_stationSite2, stationElement2p** se, int* n_stationElement2);

void matchStationElementsAndSites(stationSite2p** ss, int* n_stationSite2, stationElement2p** se, int* n_stationElement2);

#endif
