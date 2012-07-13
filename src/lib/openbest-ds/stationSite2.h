#ifndef _STATION_SITE2_H_
#define _STATION_SITE2_H_

#include "openbest-ds/config-ds.h"
#include "openbest-ds/geoPoint2.h"
#include "openbest-ds/timeInstant.h"

typedef struct
{
    int id;
    int country;
    char state[100];
    char county[100];
    geoPoint2* location;
    geoPoint2* all_locations;
    timeInstant* all_location_times;
    int timezone;
    char primary_name[50];
    char** alt_names;
    int relocated;
    int possible_relocated;
    int* instrument_changes;
    int* primary_manifests;
    int* secondary_manifests;
    int n_flags;
    int* flags;
    int n_sources;
    int* sources;
    int* archive_keys;
} stationSite2;

typedef stationSite2* stationSite2p;

stationSite2* createSS2N();

void addFlag(stationSite2* ss, int flag);

void displaySS2(stationSite2* ss);

void findByName(stationSite2* sites, int n, char* name, stationSite2** select, int* sn);

void findByFlags(stationSite2* sites, int n, int* flags, int fn, stationSite2** select, int* sn);

void displaySS2(stationSite2* ss);

void destroySS2(stationSite2* ss);

void destroySS2V(stationSite2p* ss, int n);

#endif
