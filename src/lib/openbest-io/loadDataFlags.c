#include <string.h>

#include "openbest-io/loadDataFlags.h"

void loadDataFlagsPKT()
{
  /*char filename[100];
  strcpy(filename, DATA_DIR );
  strcat(filename, "dataFlags.txt");
  FILE* f= fopen(filename, "rt");

  char buffer[300];

  char* tmp;
  char delims[]= "\n,";
  int i= 0;
  while ( fgets(buffer, 300, f) != NULL )
  {
    ++i;
    tmp= strtok(buffer, delims);
    insertIntoPKT(dataFlagsPKT, tmp, i);
    while ( tmp= strtok(NULL, delims) )
    {
        insertIntoPKT(dataFlagsPKT, tmp, i);
    }
  }

  fclose(f);*/

  char filename[100];
  strcpy(filename, DATA_DIR );
  strcat(filename, "dataFlags2.txt");
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
        insertIntoPKT(dataFlagsPKT, tmp, n);
    }
  }

  fclose(f);
}
