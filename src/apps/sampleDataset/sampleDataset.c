#include <unistd.h>
#include <stdio.h>

#include "openbest-ds/hello-openbest.h"
#include "openbest-ds/geoPoint.h"
#include "openbest-ds/geoPoint2.h"
#include "openbest-ds/geoPoint2Vector.h"
#include "openbest-ds/matrix3.h"
#include "openbest-ds/timeInstant.h"
#include "openbest-ds/timeRange.h"
#include "openbest-ds/primaryKeyTables.h"
#include "openbest-ds/init-ds.h"
#include "openbest-io/loadCountryNames.h"
#include "openbest-io/loadStationSourceTypes.h"
#include "openbest-io/loadCountryCodes.h"
#include "openbest-io/loadCountryNames.h"
#include "openbest-io/loadDataFlags.h"
#include "openbest-io/loadStationRecordTypes.h"
#include "openbest-io/loadOtherFlags.h"
#include "openbest-io/loadData.h"

#include "openbest-ds/optionTable.h"

int n= -1;
char postfix[100];

/**
  * datasets to sample:
  * dat/multi-valued-tavg/
  *                       data.txt
  *                       flags.txt
  *                       site_complete_detail.txt
  *                       site_flags.txt
  *                       sources.txt
  *                       station_change.txt
  */

int sampleDataFile(char* input, int n, char* output)
{
    tprintf("sampling: %s %s\n", input, output);
    FILE* ifile= fopen(input, "r");

    FILE* ofile= fopen(output, "w");

    char line[10000];

    int in;

    while ( 1 )
    {
        if ( feof(ifile) )
            break;

        fgets(line, 10000, ifile);
        if ( line[0] == '%' )
        {
            fputs(line, ofile);
            continue;
        }

        sscanf(line, "%d", &in);
        if ( in <= n )
            fputs(line, ofile);
        else
            break;
    }

    fclose(ifile);
    fclose(ofile);

    return 0;
}

int sampleDataset(int argc, char** argv)
{
    tprintf("sampling preliminary dataset: %d records with postfix %s\n", n, postfix);
    if ( n <= 0 )
        n= 3000;
    if ( strcmp(postfix, "") == 0 )
        sprintf(postfix, "sample.%d", n);

    char output[1000];
    sprintf(output, "%s%ssite_complete_detail.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_SITE_DATA_FILE, n, output);

    sprintf(output, "%s%ssite_flags.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_SITE_FLAGS_FILE, n, output);

    sprintf(output, "%s%sdata.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_TEMP_DATA_FILE, n, output);

    sprintf(output, "%s%sflags.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_TEMP_FLAGS_FILE, n, output);

    sprintf(output, "%s%ssources.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_SOURCES_FILE, n, output);

    sprintf(output, "%s%sstation_change.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_STATION_CHANGE_FILE, n, output);
  
    return 0;
}


/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();
    
    setPrefix(ot, "openbest i/o tests");
    addUsage(ot, "sampleDataset <option-name> arg1 arg2 ...");
    
    bool prelim= false;
    strcpy(postfix, "");
    int err;
    
    addOption(ot, "--sample", OPTION_BOOL, (char*)&prelim, 0, "sample preliminary dataset");
    addOption(ot, "--n", OPTION_INT, (char*)&n, 1, "desired number of records");
    addOption(ot, "--postfix", OPTION_CHAR, (char*)postfix, 1, "postfix of sample files");
    
    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }
    
    if ( prelim )
        err= sampleDataset(argc, argv);
    
    destroyOptionTable(ot);

    return err;
}
