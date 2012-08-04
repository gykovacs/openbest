#include <stdio.h>

#include "openbest-io/loadOptions.h"

int load_max_d()
{
    char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, "precomputed/max_d.txt");
    FILE* f= fopen(filename, "rt");

    int result;
    fscanf(f, "%d", &result);
    fclose(f);
    return result;
}

void load_r_p(real** t, int* n)
{
    *t= rnalloc(1000);
    *n= 0;

    char filename[100];
    strcpy(filename, DATA_DIR);
    strcat(filename, "precomputed/r_p.txt");
    FILE* f= fopen(filename, "rt");

    float rtmp;

    while ( !feof(f) )
    {
        fscanf(f, "%f", &rtmp);
        printf("%g\n", rtmp);
        (*t)[(*n)++]= rtmp;
    }

    *t= realloc(*t, sizeof(real)*(*n));

    fclose(f);
}
