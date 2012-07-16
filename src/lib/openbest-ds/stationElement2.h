#ifndef _STATION_ELEMENT2_H_
#define _STATION_ELEMENT2_H_

#include "openbest-ds/config-ds.h"
#include "openbest-ds/timeInstant.h"

///TODO: n_dates, n_time_of_observation, etc... should be the same, so one field would be enough
/**
  * struct for stationElement2 class
  */
typedef struct
{
    /** record type flag*/
    int record_type;
    /** frequency flag*/
    int frequency;
    /** site identifier*/
    int site;
    /** number of dates*/
    int n_dates;
    /** date vector*/
    real* dates;
    /** number of times of observations*/
    int n_time_of_observation;
    /** array of times of observations, possibly the same num_measurements in Matlab*/
    char* time_of_observation;
    /** length of data vector */
    int n_data;
    /** data array*/
    temp_t* data;
    /** length of uncertainty vector*/
    int n_uncertainty;
    /** uncertainty values*/
    real* uncertainty;
    /** length of num_measurement vector*/
    int n_num_measurements;
    /** number of measurements for a data element*/
    short* num_measurements;
    /** length of flags vector*/
    int n_n_flags;
    /** array containing the number of flags for each data element*/
    char* n_flags;
    /** data flags*/
    flag_t** flags;
    /** source flag*/
    int source;
    /** number of record flags*/
    int n_record_flags;
    /** array of record flags*/
    int* record_flags;
    /** number of primary record ids*/
    int n_primary_record_ids;
    /** primary record ids*/
    int* primary_record_ids;
} stationElement2;

/** stationElement2 pointer type*/
typedef stationElement2* stationElement2p;

/**
  * creates a new stationElement2 instance and initializes the fields with NULLs
  * @returns pointer of the new instance
  */
stationElement2* createSE2N();

stationElement2* createSE2NC(stationElement2* se);

/**
  * prints the properties of the parameter stationElement2 struct to stdout
  * @param se stationElement2 object to print out
  */
void displaySE2(stationElement2* se);

/**
  * frees the fields of the parameter stationElement2 object and the object itself
  * @param se stationElement2 object to free
  */
void destroySE2(stationElement2* se);

/**
  * destroys an array of stationElement2 objects
  * @param se array of stationElement2p objects
  * @param n length of the array
  */
void destroySE2V(stationElement2p* se, int n);

/**
  * Takes a station element record that may be multi-valued and returns a single-valued alternative record. This operates on the assumption that all non-flagged values it is passed are equally acceptable, and expands the uncertainty to accommodate that if necessary.
  * @param se stationElement2 object pointer,
  * @param bf bad flags array
  * @param n length of bad flags array
  * @return new, single-valued stationElement2 object pointer
  */
stationElement2* makeSingleValued(stationElement2* se, int* bf, int n);

bool isSingleValued(stationElement2* se);

bool isMultiValued(stationElement2* se);

stationElement2* structureMerge(stationElement2* se);

stationElement2* mergeCore(stationElement2* se, int* bf, int n, char* options);

#endif
