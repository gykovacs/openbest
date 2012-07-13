#ifndef _STATION_ELEMENT2_H_
#define _STATION_ELEMENT2_H_

#include "openbest-ds/config-ds.h"
#include "openbest-ds/timeInstant.h"

typedef struct
{
    int record_type;
    int frequency;
    int site;
    int n_dates;
    real* dates;
    int n_time_of_observation;
    char* time_of_observation;
    int n_data;
    temp_t* data;
    int n_uncertainty;
    real* uncertainty;
    int n_num_measurements;
    short* num_measurements;
    int n_n_flags;
    char* n_flags;
    flag_t** flags;
    int source;
    int n_record_flags;
    int* record_flags;
    int n_primary_record_ids;
    int* primary_record_ids;
} stationElement2;

typedef stationElement2* stationElement2p;

stationElement2* createSE2N();

void displaySE2(stationElement2* se);

void destroySE2(stationElement2* se);

void destroySE2V(stationElement2p* se, int n);

#endif
