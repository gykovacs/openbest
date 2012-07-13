#ifndef _STATION_SITE_H_
#define _STATION_SITE_H_

#include "openbest-ds/config-ds.h"
#include "openbest-ds/geoPoint.h"

/** struct for stationSite class*/
typedef struct
{
    int id;
    int* other_ids;
    int country;
    char state[100];
    int county;
    geoPoint location;
    geoPoint* all_locations;
    int* all_location_times;
    int timezone;
    char primary_name[50];
    char** alt_names;
    int* relocated;
    int* possible_relocated;
    int* associated_uids;
    int* flags;
    int* sources;
}  stationSite;

#endif
