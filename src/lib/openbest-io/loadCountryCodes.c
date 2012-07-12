#include <string.h>

#include "openbest-io/loadCountryCodes.h"

void loadCountryCodesPKT()
{
  char filename[100];
  strcpy(filename, DATA_DIR );
  strcat(filename, "countryCodes.txt");
  FILE* f= fopen(filename, "rt");

  char buffer[200];

  int n;
  char* tmp;
  char delims[]= "\n,";
  while ( fgets(buffer, 200, f) != NULL )
  {
    tmp= strtok(buffer, delims);
    n= atoi(tmp);

    while ( tmp= strtok(NULL, delims) )
    {
        insertIntoPKT(countryCodesPKT, tmp, n);
    }
  }

  fclose(f);
}
