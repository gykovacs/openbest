#include <string.h>

#include "openbest-io/loadCountryNames.h"

void loadCountryNamesPKT()
{
  char filename[100];
  strcpy(filename, DATA_DIR );
  strcat(filename, "countryNames.txt");
  FILE* f= fopen(filename, "rt");
  
  char buffer[200];

  int n;
  char* tmp, *name;
  char delims[]= "\n,";
  while ( fgets(buffer, 200, f) != NULL )
  {
    tmp= strtok(buffer, delims);
    n= atoi(tmp);
    name= strtok(NULL, delims);

    insertIntoPKT(countryNamesPKT, name, n);
  }
  
  fclose(f);
}
