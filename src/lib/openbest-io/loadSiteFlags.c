#include <string.h>

#include "loadSiteFlags.h"

void loadSiteFlagsPKT()
{
    char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, "siteFlags.txt");
    FILE* f= fopen(filename, "rt");

    char buffer[300];

    char* tmp;
    char delims[]= "\n,";
    int i= 0;
    while ( fgets(buffer, 300, f) != NULL )
    {
      ++i;
      tmp= strtok(buffer, delims);
      insertIntoPKT(siteFlagsPKT, tmp, i);
      while ( tmp= strtok(NULL, delims) )
      {
          insertIntoPKT(siteFlagsPKT, tmp, i);
      }
    }

    fclose(f);
}
