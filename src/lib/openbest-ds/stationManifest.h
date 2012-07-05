#ifndef _STATION_MANIFEST_H_
#define _STATION_MANIFEST_H_

#include "openbest-ds/timeRange.h"

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
  int reloc;
  int station_type;
} stationManifest;

void setStartDateSM(stationManifest* sm, timeInstant* ti);

void setDurationSM(stationManifest* sm, timeInstant* t1, timeInstant* t2);

void setDurationSM(stationManifest* sm, timeRange* tr);



#endif