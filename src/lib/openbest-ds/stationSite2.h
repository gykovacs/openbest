#ifndef _STATION_SITE2_H_
#define _STATION_SITE2_H_

#include "openbest-ds/config-ds.h"
#include "openbest-ds/geoPoint2.h"
#include "openbest-ds/timeInstant.h"

/**
  * struct for stationSite2 class
  */
typedef struct
{
    /** station id*/
    int id;
    /** country flag*/
    int country;
    /** state name*/
    char state[100];
    /** county name*/
    char county[100];
    /** location of the station*/
    geoPoint2* location;
    /** other location values*/
    geoPoint2* all_locations;
    /** location times*/
    timeInstant* all_location_times;
    /** timezone flag*/
    int timezone;
    /** name of the station*/
    char primary_name[50];
    /** alternative names*/
    char** alt_names;
    /** relocation flag*/
    int relocated;
    /** possible relocation flag*/
    int possible_relocated;
    /** instrument changes*/
    int* instrument_changes;
    /** primary manifest identifiers*/
    int* primary_manifests;
    /** secondary manifest identifiers*/
    int* secondary_manifests;
    /** number of site flags*/
    int n_flags;
    /** array of site flags*/
    int* flags;
    /** number of sources*/
    int n_sources;
    /** array of source identifiers*/
    int* sources;
    /** archivation keys*/
    int* archive_keys;

    int n_relocations;
    real* relocations;
    flag_t* relocation_types;
} stationSite2;

/** stationSite2 pointer type definition*/
typedef stationSite2* stationSite2p;

/**
  * allocates memory for a new stationSite2 object, initializes its fields with NULLs and returns its pointer
  * @returns pointer of the new object
  */
stationSite2* createSS2N();

stationSite2* createSS2NC(stationSite2* ss);

/**
  * prints the properties of the parameter stationSite2 object to stdout
  * @param ss stationSite2 object to display
  */
void displaySS2(stationSite2* ss);

/**
  * frees the fields of the parameter stationSite2 object and the object itself
  * @param ss stationSite2 object to destroy
  */
void destroySS2(stationSite2* ss);

/**
  * destroys an array of stationSite2 objects
  * @param ss array of stationSite2 instances
  * @param length of the array
  */
void destroySS2V(stationSite2p* ss, int n);


/**
  * Shrinks the vector of station sites, by collapsing the records of the same stations from different sources
  * @param se array os stationSite2 object pointers
  * @param n size of the array
  */
void shrinkSS2V(stationSite2p** ss, int* n);

int sortByIDSS2V(const void* a, const void* b);

#endif
