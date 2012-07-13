#include <string.h>

#include "loadStationRecordTypes.h"

void loadStationRecordTypesPKT()
{
    /*char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, "stationRecordTypes.txt");
    FILE* f= fopen(filename, "rt");

    char buffer[300];

    char* tmp;
    char delims[]= "\n,";
    int i= 0;
    while ( fgets(buffer, 300, f) != NULL )
    {
      ++i;
      tmp= strtok(buffer, delims);

      insertIntoPKT(stationRecordTypesPKT, tmp, i);
      while ( tmp= strtok(NULL, delims) )
      {
          insertIntoPKT(stationRecordTypesPKT, tmp, i);
      }
    }

    fclose(f);*/

    char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, "recordTypes2.txt");
    FILE* f= fopen(filename, "rt");

    char buffer[300];

    char* tmp;
    char delims[]= "\n,";
    int n;
    while ( fgets(buffer, 300, f) != NULL )
    {
      tmp= strtok(buffer, delims);
      sscanf(tmp, "%d", &n);
      while ( tmp= strtok(NULL, delims) )
      {
          insertIntoPKT(stationRecordTypesPKT, tmp, n);
      }
    }

    fclose(f);
}
