#ifndef _STATION_SOURCE_TYPE_H_
#define _STATION_SOURCE_TYPE_H_

#include "openbest-ds/primaryKeyTable.h"

/**
 * primary key table for station source types
 */ 
primaryKeyTable* stationSourcePKT;

/**
 * builds up the primary key tab containing the station source types
 */ 
void loadStationSourcePKT();

#endif