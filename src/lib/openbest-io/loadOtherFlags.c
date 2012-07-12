#include "openbest-io/loadOtherFlags.h"
#include "openbest-ds/primaryKeyTables.h"

void loadBadFlags()
{
    char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, "badFlags.txt");
    FILE* f= fopen(filename, "rt");

    char buffer[300];
    int n, i;

    fscanf(f, "%d", &n);
    n_badFlags= n;
    badFlags= (char**)malloc(sizeof(char*)*n);

    for ( i= 0; i < n_badFlags; ++i )
    {
        fscanf(f, "%s", buffer);
        badFlags[i]= (char*)malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(badFlags[i], buffer);
    }

    fclose(f);
}

void loadSourceRecordTypes()
{
    char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, "sourceRecordTypes.txt");
    FILE* f= fopen(filename, "rt");

    char buffer[300];
    int n, i;

    fscanf(f, "%d", &n);
    n_sourceRecordTypes= n;
    sourceRecordTypes= (char**)malloc(sizeof(char*)*n);
    for ( i= 0; i < n_sourceRecordTypes; ++i )
    {
        fscanf(f, "%s", buffer);
        sourceRecordTypes[i]= (char*)malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(sourceRecordTypes[i], buffer);
    }

    fclose(f);
}

void loadSourceFileCodes()
{
    char filename[100];
    strcpy(filename, DATA_DIR );
    strcat(filename, "sourceFileCodes.txt");
    FILE* f= fopen(filename, "rt");

    char buffer[300];
    int n, i;
    int flag;

    fscanf(f, "%d", &n);
    n_sourceFileCodes= n;
    sourceFileCodes= (char**)malloc(sizeof(char*)*n);
    int* flags= (int*)malloc(sizeof(int)*n);
    int sum= 0;
    for ( i= 0; i < n_sourceFileCodes; ++i )
    {
        fscanf(f, "%d,%s", &flag, buffer);
        flags[i]= flag;
        sourceFileCodes[i]= (char*)malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(sourceFileCodes[i], buffer);
        if ( !flag )
            ++sum;
    }

    sourceFileCodes= (char**)realloc(sourceFileCodes, sizeof(char*)*(n + sum));

    int tmp= 0;
    for ( i= 0; i < n_sourceFileCodes; ++i )
        if ( !flags[i] )
        {
            sourceFileCodes[n+tmp]= (char*)malloc(sizeof(char*)*(strlen(sourceFileCodes[i])+1+30));
            strcpy(sourceFileCodes[n+tmp], "Monthly_Averaged_");
            strcat(sourceFileCodes[n+tmp], sourceFileCodes[i]);
            ++tmp;
        }

    monthly= (int*)malloc(sizeof(int)*(n_sourceFileCodes + tmp));
    for ( i= 0; i < n_sourceFileCodes; ++i )
        monthly[i]= flags[i];
    for ( i= n_sourceFileCodes; i < n_sourceFileCodes + tmp; ++i )
        monthly[i]= 1;

    n_sourceFileCodes+= tmp;
    fclose(f);
    free(flags);
}

void loadFrequencyTypesPKT()
{
    insertIntoPKT(frequencyTypesPKT, "daily", 0);
    insertIntoPKT(frequencyTypesPKT, "d", 0);
    insertIntoPKT(frequencyTypesPKT, "day", 0);
    insertIntoPKT(frequencyTypesPKT, "dly", 0);

    insertIntoPKT(frequencyTypesPKT, "monthly", 1);
    insertIntoPKT(frequencyTypesPKT, "m", 1);
    insertIntoPKT(frequencyTypesPKT, "mon", 1);
    insertIntoPKT(frequencyTypesPKT, "month", 1);

    insertIntoPKT(frequencyTypesPKT, "annual", 2);
    insertIntoPKT(frequencyTypesPKT, "a", 2);
    insertIntoPKT(frequencyTypesPKT, "year", 2);
    insertIntoPKT(frequencyTypesPKT, "yr", 2);
    insertIntoPKT(frequencyTypesPKT, "y", 2);

    insertIntoPKT(frequencyTypesPKT, "hourly", 3);
    insertIntoPKT(frequencyTypesPKT, "h", 3);
    insertIntoPKT(frequencyTypesPKT, "hour", 3);
    insertIntoPKT(frequencyTypesPKT, "hr", 3);
}

void loadOtherFlags()
{
    loadBadFlags();
    loadSourceRecordTypes();
    loadSourceFileCodes();
    loadFrequencyTypesPKT();
}
