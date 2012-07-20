#include <string.h>

#include "openbest-ds/config-ds.h"
#include "openbest-ds/init-ds.h"

void initDS()
{
  unknown_value= (char*)malloc(sizeof(char)*(strlen(UNKNOWN_FLAG_STRING)+1));
  strcpy(unknown_value, UNKNOWN_FLAG_STRING);
  compression_area= (char*)malloc(sizeof(char)*COMPRESSION_AREA);

  countryNamesPKT= createPrimaryKeyTableP();
  
  stationSourceTypesPKT= createPrimaryKeyTableP();

  countryCodesPKT= createPrimaryKeyTableP();

  dataFlagsPKT= createPrimaryKeyTableP();

  siteFlagsPKT= createPrimaryKeyTableP();

  stationRecordTypesPKT= createPrimaryKeyTableP();

  matchFlagsPKT= createPrimaryKeyTableP();

  frequencyTypesPKT= createPrimaryKeyTableP();

  badFlags= NULL;
  n_sourceFileCodes= 0;
  sourceFileCodes= NULL;
  monthly= NULL;
  n_sourceRecordTypes= 0;
  sourceRecordTypes= NULL;
}

void finalizeDS()
{
    free(unknown_value);

    destroyPKT(countryNamesPKT);
    destroyPKT(stationSourceTypesPKT);
    destroyPKT(countryCodesPKT);
    destroyPKT(dataFlagsPKT);
    destroyPKT(siteFlagsPKT);
    destroyPKT(stationRecordTypesPKT);
    destroyPKT(matchFlagsPKT);
    destroyPKT(frequencyTypesPKT);

    int i;
    for ( i= 0; i < n_badFlags; ++i )
        free(badFlags[i]);
    free(badFlags);
    free(iBadFlags);

    for ( i= 0; i < n_sourceFileCodes; ++i )
        free(sourceFileCodes[i]);
    free(sourceFileCodes);

    for ( i= 0; i < n_sourceRecordTypes; ++i )
        free(sourceRecordTypes[i]);
    free(sourceRecordTypes);

    if ( monthly )
        free(monthly);

    n_badFlags= 0;
    n_sourceFileCodes= 0;
    n_sourceRecordTypes= 0;
}
