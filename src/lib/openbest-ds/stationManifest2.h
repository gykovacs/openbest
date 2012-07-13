#ifndef _STATION_MANIFEST2_H_
#define _STATION_MANIFEST2_H_

#include "openbest-ds/config-ds.h"
#include "openbest-ds/timeRange.h"
#include "openbest-ds/geoPoint.h"

/**
  * struct for stationManifest2 class
  */
typedef struct
{
  int source;
  int ids[10];
  int climate_division;
  int country;
  char state[100];
  int county;
  timeRange duration;
  int location;
  int time_zone;
  char name[100];
  real alt_elevation;
  real alt_elevation_type;
  char reloc[100];
  int site_flags[];
  char original_line[];
  char archive_key[];
  int station_type;
} stationManifest2;

#endif
