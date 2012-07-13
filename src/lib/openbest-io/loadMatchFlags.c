#include <string.h>

#include "loadMatchFlags.h"

void loadMatchFlagsPKT()
{
    /*char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, "matchFlags.txt");
    FILE* f= fopen(filename, "rt");

    char buffer[300];

    int n;
    char* tmp;
    char delims[]= "\n,";
    int i= 0;
    while ( fgets(buffer, 300, f) != NULL )
    {
      ++i;
      tmp= strtok(buffer, delims);
      insertIntoPKT(matchFlagsPKT, tmp, i);
      while ( tmp= strtok(NULL, delims) )
      {
          insertIntoPKT(matchFlagsPKT, tmp, i);
      }
    }

    fclose(f);*/

    char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, "matchFlags2.txt");
    FILE* f= fopen(filename, "rt");

    char buffer[300];

    int n;
    char* tmp;
    char delims[]= "\n,";
    int i= 0;
    while ( fgets(buffer, 300, f) != NULL )
    {
      tmp= strtok(buffer, delims);
      sscanf(tmp, "%d", &n);
      while ( tmp= strtok(NULL, delims) )
      {
          insertIntoPKT(matchFlagsPKT, tmp, n);
      }
    }

    fclose(f);
}
