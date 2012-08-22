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
    /** length of sources vector*/
    int n_n_sources;
    /** array containing the number of sources for each data element*/
    char* n_sources;
    /** source flags*/
    flag_t** sources;
    /** number of record flags*/
    int n_record_flags;
    /** array of record flags*/
    int* record_flags;
    /** number of primary record ids*/
    int n_primary_record_ids;
    /** primary record ids*/
    int* primary_record_ids;

    /** true, if the contents are compressed, false, otherwise*/
    bool compressed;
    /** true, if the representation is packed, false, otherwise*/
    bool packed;

    /** pointer of the compressed contents*/
    char* compressedRecord;
    /** length of the compressed record*/
    unsigned int compressedRecordLength;
    /** length of the content that is compressed*/
    unsigned int dataLength;
} stationElement2;

/** stationElement2 pointer type*/
typedef stationElement2* stationElement2p;

/**
  * creates a new stationElement2 instance and initializes the fields with NULLs
  * @returns pointer of the new instance
  */
stationElement2* createSE2N();

/**
  * creates a stationElement2 instance with test content
  * @returns pointer of the test stationElement2 instance
  */
stationElement2* createSE2Test();

/**
  * creates a stationElement2 instance capable to contain m data elements
  * @param m number of elements in the stationElement2 object
  * @returns pointer to the new instance
  */
stationElement2* createSE2NM(int m);

/**
  * copies the contents of the parameter stationElement2 into a new instance
  * @param se instance to copy
  * @returns pointer of the new instance
  */
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
  * returns true if the parameter stationElement2 instance is single valued
  * @param se stationElement2 instance to check
  * @return true if se is single valued, false, otherwise
  */
bool isSingleValued(stationElement2* se);

/**
  * returns true if the parameter stationElement2 instance is multi valued
  * @param se stationElement2 instance to check
  * @return true if se is multi valued, false, otherwise
  */
bool isMultiValued(stationElement2* se);

/**
  * puts the indeces of those data elements into f, that is flagged by any of the badFlags
  * @param se stationElement2 instance
  * @param badFlags pointer to bad flags
  * @param n_badFlags number of bad flags
  * @param f preallocated area for the indeces
  * @param n_f output parameter - length of f
  */
void findFlags(stationElement2* se, int* badFlags, int n_badFlags, int* f, int* n_f);

/**
  * creates a new stationElement2 instance by selecting the indexed elements of the parameter stationElement2 instance
  * @param se input stationElement2 parameter
  * @param f array of indeces
  * @param n_f length of f
  * @returns the new stationElement2 instance
  */
stationElement2* createSE2Select(stationElement2p se, int* f, int n_f);

/**
  * compresses the parameter stationElement2 instance
  * @param se pointer to stationElement2 instance to compress
  */
void compressSE2(stationElement2p se);

/**
  * decompresses the parameter stationElement2 instance
  * @param se pointer to stationElement2 instance to decompress
  */
void decompressSE2(stationElement2p se);

/**
  * computes the size required to store the parameter stationElement2 object
  * @param se pointer to a stationElement2 instance
  * @returns size of memory used to store the object, in bytes
  */
unsigned int sizeSE2(stationElement2p se);

/**
  * computes the size of the compressable content of the parameter stationElement2 object
  * @param se pointer to a stationElement2 instance
  * @returns size of memory in bytes, used to store the compressable contents
  */
unsigned int sizeOfCompressableSE2(stationElement2p se);

/**
  * returns the length of the longest data series in the parameter stationElement2 array
  * @param se array of stationElement2 pointers
  * @param n length of the array se
  * @returns size of the longest data series
  */
int longestDataSeries(stationElement2p* se, int n);

/**
  * sorts the content of the parameter stationElement2 instance by date
  * @param se pointer of the stationElement2 instance to sort
  */
void sortSE2DataByDate(stationElement2p se);

/**
  * computes the month range in the array of stationElement2 instances
  * @param se array of stationElement2 instances
  * @param n length of se
  * @param min_month output parameter - minimum month
  * @param max_month output parameter - maximum month
  */
void monthRange(stationElement2p* se, int n, real* min_month, real* max_month);

/**
  * checks whether the parameter stationElement2 instance contains monthly data
  * @param se pointr of the input stationElement2 instance
  * @returns true if se is monthly, false, otherwise
  */
int isMonthly(stationElement2p se);

/**
  * removes bad flagged data elements from an array of stationElement2 instances
  * @param se array of stationElement2 pointers
  * @param n length of se
  * @param bf array of bad flags
  * @param n_bf length of array bf
  */
void removeBadFlaggedDataV(stationElement2p* se, int n, int* bf, int n_bf);

/**
  * removes bad flagged data elements from a stationElement2 instance
  * @param se pointer of stationElement2 instance
  * @param bf array of bad flags
  * @param n_bf length of array bf
  */
void removeBadFlaggedData(stationElement2p se, int* bf, int n_bf);

#endif
