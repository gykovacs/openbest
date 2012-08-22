#ifndef _PRIMARY_KEY_TABLES_H_
#define _PRIMARY_KEY_TABLES_H_

#include "openbest-ds/primaryKeyTable.h"

/**
 * primary key table for country names
 */ 
primaryKeyTable* countryNamesPKT;

/**
 * primary key table for station source types
 */ 
primaryKeyTable* stationSourceTypesPKT;

/**
 * primary key table for country codes;
 */
primaryKeyTable* countryCodesPKT;

/**
 * primary key table for data flags
 */
primaryKeyTable* dataFlagsPKT;

/**
 * primary key table for site flags
 */
primaryKeyTable* siteFlagsPKT;

/**
 * primary key table for station record types
 */
primaryKeyTable* stationRecordTypesPKT;

/**
 * primary key table for match flags
 */
primaryKeyTable* matchFlagsPKT;

/**
  * number of bad flags
  */
int n_badFlags;

/**
  * bad flag names
  */
char** badFlags;

/**
  * bad flag codes array
  */
int* iBadFlags;

/**
  * number of source file codes
  */
int n_sourceFileCodes;

/**
  * source file codes
  */
char** sourceFileCodes;

/**
  * monthly array
  */
int* monthly;

/**
  * number of source record types
  */

int n_sourceRecordTypes;
/**
  * source record types
  */
char** sourceRecordTypes;

/**
  * primary key table for frequency types
  */
primaryKeyTable* frequencyTypesPKT;

#endif
