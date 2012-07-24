#ifndef _SCALPEL_METHODS_H_
#define _SCALPEL_METHODS_H_

#include "openbest-ds/stationElement2.h"
#include "openbest-ds/stationSite2.h"
#include "openbest-ds/primaryKeyTables.h"
#include "openbest-av/berkeleyAverageOptions.h"

void splitStationBreaks(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, int gap, int* badFlags, int n_badFlags, int** back_mapIO, int** start_posIO);

void splitStationMoves(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, bool declared_moves, bool suspected_moves, int** back_mapIO, int** start_posIO);

void splitStationTOBChanges(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, real persistence, int* badFlags, int n_badFlags, real min_difference, int** back_mapIO, int** start_posIO);

void empiricalCuts(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, berkeleyAverageOptions* bao, int** back_mapIO, int** start_posIO);

#endif
