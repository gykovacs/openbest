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

int n_badFlags;
char** badFlags;

int n_sourceFileCodes;
char** sourceFileCodes;
int* monthly;

int n_sourceRecordTypes;
char** sourceRecordTypes;

primaryKeyTable* frequencyTypesPKT;

#endif
