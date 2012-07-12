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

void loadPreliminaryData();

void loadData(stationSite2p** ss, int* n_stationSite2, stationElement2p** se, int* n_stationElement2);

#endif
