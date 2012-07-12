#ifndef _STATION_ELEMENT_H_
#define _STATION_ELEMENT_H_

#include "openbest-ds/config-ds.h"

typedef struct
{
    int record_type;
    int frequency;
    timeInstant* dates;
    timeInstant time_of_observation;
    real* data;
    int* num_measurements;
    int* flags;
    int source;
} stationElement;

#endif
