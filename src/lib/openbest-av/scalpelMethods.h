#ifndef _SCALPEL_METHODS_H_
#define _SCALPEL_METHODS_H_

#include "openbest-ds/stationElement2.h"
#include "openbest-ds/stationSite2.h"
#include "openbest-ds/primaryKeyTables.h"
#include "openbest-av/berkeleyAverageOptions.h"

/**
  * split stationElement2 records by station break times
  * @param seIO pointer to the array of stationElement2 instance array
  * @param n_stationElement2IO pointer to the length of array seIO
  * @param ssIO pointer to the array of stationSite2 instance array
  * @param n_stationSite2IO pointer to the length of array ssIO
  * @param gap number of missing months to break the time series at
  * @param badFlags array of bad flags
  * @param n_badFlags length of array of bad flags
  * @param pointer to the array of back_map indeces
  * @param pointer to the array of start_pos indeces
  */
void splitStationBreaks(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, int gap, int* badFlags, int n_badFlags, int** back_mapIO, int** start_posIO);

/**
  * split stationElement2 records by station moves
  * @param seIO pointer to the array of stationElement2 instance array
  * @param n_stationElement2IO pointer to the length of array seIO
  * @param ssIO pointer to the array of stationSite2 instance array
  * @param n_stationSite2IO pointer to the length of array ssIO
  * @param declared_moves if true, the time series are broken at declared moves
  * @param suspected_moves if true, the time series are broken at suspected moves
  * @param pointer to the array of back_map indeces
  * @param pointer to the array of start_pos indeces
  */
void splitStationMoves(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, bool declared_moves, bool suspected_moves, int** back_mapIO, int** start_posIO);

/**
  * split stationElement2 records by TOB changes
  * @param seIO pointer to the array of stationElement2 instance array
  * @param n_stationElement2IO pointer to the length of array seIO
  * @param ssIO pointer to the array of stationSite2 instance array
  * @param n_stationSite2IO pointer to the length of array ssIO
  * @param badFlags array of bad flags
  * @param n_badFlags length of array of bad flags
  * @param min_difference minimum difference of TOB values to break at
  * @param pointer to the array of back_map indeces
  * @param pointer to the array of start_pos indeces
  */
void splitStationTOBChanges(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, real persistence, int* badFlags, int n_badFlags, real min_difference, int** back_mapIO, int** start_posIO);

/**
  * applying some empirical cuts to the stationElement2 records
  * @param seIO pointer to the array of stationElement2 instance array
  * @param n_stationElement2IO pointer to the length of array seIO
  * @param ssIO pointer to the array of stationSite2 instance array
  * @param n_stationSite2IO pointer to the length of array ssIO
  * @param bao berkeleyAverageOptions instance
  * @param pointer to the array of back_map indeces
  * @param pointer to the array of start_pos indeces
  */
void empiricalCuts(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, berkeleyAverageOptions* bao, int** back_mapIO, int** start_posIO);

#endif
