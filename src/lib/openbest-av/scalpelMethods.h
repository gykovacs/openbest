#ifndef _SCALPEL_METHODS_H_
#define _SCALPEL_METHODS_H_

#include "openbest-ds/stationElement2.h"
#include "openbest-ds/stationSite2.h"
#include "openbest-ds/primaryKeyTables.h"

void splitStationBreaks(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, int gap, flag_t* badFlags, int n_badFlags, int** back_mapIO, int** start_posIO);

void splitStationMoves(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, bool declared_moves, bool suspected_moves, flag_t* badFlags, int n_badFlags, int** back_mapIO, int** start_posIO);

void splitStationTOBChange(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, int persistence, flag_t* badFlags, int n_badFlags, real min_difference, int** back_mapIO, int** start_posIO);

#endif
