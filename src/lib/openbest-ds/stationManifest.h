#ifndef _STATION_MANIFEST_H_
#define _STATION_MANIFEST_H_

#include "openbest-ds/config-ds.h"
#include "openbest-ds/timeRange.h"
#include "openbest-ds/geoPoint.h"

/**
  * struct for stationManifest class
  */
typedef struct
{
  int source;
  int ids[10];
  int climate_division;
  int country;
  char state[100];
  timeRange duration;
  int location;
  int time_zone;
  char name[100];
  real location_precision;
  real alt_elevation;
  real alt_elevation_type;
  char reloc[100];
  int station_type;
} stationManifest;

#endif
