#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

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
#include "openbest-ds/makeSingleValued.h"
#include "openbest-av/scalpelMethods.h"
#include "openbest-ds/printOut.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/config-ds.h"

#include "openbest-ds/optionTable.h"
#include "openbest-av/berkeleyAverage.h"

int sampleSize= -1;
char postfix[100];

bool seq= false;
bool rnd= false;

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

int sampleDatasetTXT(int argc, char** argv)
{
    tprintf("Sampling preliminary dataset: %d records with postfix %s\n", sampleSize, postfix);
    if ( sampleSize <= 0 )
        sampleSize= 3000;
    if ( strcmp(postfix, "") == 0 )
        sprintf(postfix, "sample.%d", sampleSize);

    char output[1000];
    sprintf(output, "%s%ssite_complete_detail.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_SITE_DATA_FILE, sampleSize, output);

    sprintf(output, "%s%ssite_flags.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_SITE_FLAGS_FILE, sampleSize, output);

    sprintf(output, "%s%sdata.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_TEMP_DATA_FILE, sampleSize, output);

    sprintf(output, "%s%sflags.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_TEMP_FLAGS_FILE, sampleSize, output);

    sprintf(output, "%s%ssources.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_SOURCES_FILE, sampleSize, output);

    sprintf(output, "%s%sstation_change.txt.%s.txt", DATA_DIR, FILE_SEP, postfix);
    sampleDataFile(FULL_STATION_CHANGE_FILE, sampleSize, output);
  
    return 0;
}

int* initIndexSet(int nse)
{
    tprintf("Generating index set\n");
    int* idxset= inalloc(sampleSize);
    int i;

    srand(time(NULL));

    if ( seq )
    {
        for ( i= 0; i < sampleSize; ++i )
            idxset[i]= i;
        return idxset;
    }
    else if ( rnd )
    {
        seti(idxset, sampleSize, 0);
        bool* flags= bnalloc(nse);
        setf(flags, nse, false);

        int idx;
        for ( i= 0; i < sampleSize; ++i )
        {
            idx= rand()%nse;
            while ( flags[idx] )
                idx= rand()%nse;
            idxset[i]= idx;
            flags[idx]= true;
        }

        free(flags);
        return idxset;
    }
    tprintf("Index set generated\n");
    return idxset;
}

int sampleDatasetMATLAB(int argc, char** argv)
{
    tprintf("Sampling Matlab exported dataset: %d records are required\n", sampleSize);

    int i, j, k, m;

    stationSite2p* ss;
    int n_stationSite2;
    stationElement2p* se;
    int n_stationElement2;

    tprintf("Reading station elements...\n");

    FILE* fse= fopen(argv[1], "r");
    int n;
    fscanf(fse, "%d", &n);
    se= (stationElement2**)malloc(sizeof(stationElement2*)*n);
    float a, b, c;
    int d, e, f;
    char line[1000];
    for ( i= 0; i < n; ++i )
    {
        fscanf(fse, "%d\n", &m);
        se[i]= createSE2NM(m);

        for ( j= 0; j < m; ++j )
        {
            fgets(line, 1000, fse);
            f= sscanf(line, "%f %f %f %d %d", &a, &b, &c, &d, &e);
            se[i]->dates[j]= a;
            se[i]->data[j]= b;
            se[i]->uncertainty[j]= c;
            if ( f > 3 )
                se[i]->time_of_observation[j]= d;
            else
                se[i]->time_of_observation[j]= -99;
            if ( f > 4 )
                se[i]->num_measurements[j]= e;
            else
                se[i]->num_measurements[j]= -99;
        }
    }
    fclose(fse);

    tprintf("reading station sites...\n");
    int nn;
    int tmps;
    fse= fopen(argv[2], "r");
    fscanf(fse, "%d", &nn);
    ss= (stationSite2**)malloc(sizeof(stationSite2*)*nn);
    for ( i= 0; i < nn; ++i )
    {
        ss[i]= createSS2N();
        k= fscanf(fse, "%f %f %f", &a, &b, &c);
        if ( k == 3 && c == c )
            ss[i]->location= createGeoPoint23(a, b, c);
        else
        {
            ss[i]->location= createGeoPoint22(a, b);
        }
        fscanf(fse, "%d\n", &d);
        ss[i]->relocations= rnalloc(d);
        ss[i]->relocation_types= fnalloc(d);
        if ( d > 0 )
        {
            fgets(line, 1000, fse);
            tmps= 0;
            for ( j= 0; j < d; ++j )
            {
                sscanf(line + tmps, "%f%n", &a, &tmps);
                ss[i]->relocations[j]= a;
                ss[i]->relocation_types[j]= DECLARED_MOVE;
            }
        }
        fscanf(fse, "%d\n", &e);
        if ( e > 0 )
        {
            ss[i]->relocations= (real*)realloc(ss[i]->relocations, sizeof(real)*(d + e));
            ss[i]->relocation_types= (flag_t*)realloc(ss[i]->relocation_types, sizeof(flag_t)*(d + e));
            fgets(line, 1000, fse);
            tmps= 0;
            for ( j= 0; j < e; ++j )
            {
                sscanf(line + tmps, "%f%n", &a, &tmps);
                ss[i]->relocations[j]= a;
                ss[i]->relocation_types[j]= SUSPECTED_MOVE;
            }
        }
        ss[i]->n_relocations= d + e;
    }
    fclose(fse);

    n_stationElement2= n;
    n_stationSite2= nn;

    int* is= initIndexSet(n_stationElement2);

    tprintf("Writing data elements...\n");

    FILE* ose= fopen(argv[3], "wt");

    fprintf(ose, "%d\n", sampleSize);
    for ( i= 0; i < sampleSize; ++i )
    {
        fprintf(ose, "%d\n", se[is[i]]->n_data);
        for ( j= 0; j < se[is[i]]->n_data; ++j )
            fprintf(ose, "%f %f %f %d %d\n", se[is[i]]->dates[j], se[is[i]]->data[j], se[is[i]]->uncertainty[j], se[is[i]]->time_of_observation[j], se[is[i]]->num_measurements[j]);
    }
    fclose(ose);

    FILE* oss= fopen(argv[4], "wt");

    fprintf(oss, "%d\n", sampleSize);
    for ( i= 0; i < sampleSize; ++i )
    {
        fprintf(oss, "%f %f %f\n", ss[is[i]]->location->latitude, ss[is[i]]->location->longitude, ss[is[i]]->location->elevation);
        int numdm= 0;
        for ( j= 0; j < ss[is[i]]->n_relocations; ++j )
            if ( ss[is[i]]->relocation_types[j] == DECLARED_MOVE )
                ++numdm;
        fprintf(oss, "%d\n", numdm);
        if ( numdm > 0 )
        {
            for ( j= 0; j < ss[is[i]]->n_relocations; ++j )
                if ( ss[is[i]]->relocation_types[j] == DECLARED_MOVE )
                    fprintf(oss, "%f ", ss[is[i]]->relocations[j]);
            fprintf(oss, "\n");
        }

        int numsm= 0;
        for ( j= 0; j < ss[is[i]]->n_relocations; ++j )
            if ( ss[is[i]]->relocation_types[j] == SUSPECTED_MOVE )
                ++numsm;
        fprintf(oss, "%d\n", numsm);
        if ( numsm > 0 )
        {
            for ( j= 0; j < ss[is[i]]->n_relocations; ++j )
                if ( ss[is[i]]->relocation_types[j] == SUSPECTED_MOVE )
                    fprintf(oss, "%f ", ss[is[i]]->relocations[j]);
            fprintf(oss, "\n");
        }
    }

    fclose(oss);

    free(is);

    destroySE2V(se, n_stationElement2);
    destroySS2V(ss, n_stationSite2);

    return 0;
}

/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();
    
    setPrefix(ot, "\nsampleDataset application \n=========================\n\t written by Gyorgy Kovacs (gyuriofkovacs@gmail.com)");
    addUsage(ot, "\t\tsampleDataset <option-name> arg1 arg2 ...\n\t\tsampleDataset --sample --txtdata --n 200 --seq --postfix sample200\n\t\tsampleDataset --sample --matlabdata --n 200 --rand se.dat sites.dat se200.dat sites200.dat");
    
    bool sample= false;
    bool matlabdata= false;
    bool txtdata= false;
    bool verbose= false;
    strcpy(postfix, "");
    int err;
    
    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "GENERAL OPTIONS");
    addOption(ot, "--sample", OPTION_BOOL, (char*)&sample, 0, "sample preliminary dataset");
    addOption(ot, "--n", OPTION_INT, (char*)&sampleSize, 1, "desired number of records");
    addOption(ot, "--txtdata", OPTION_BOOL, (char*)&txtdata, 0, "sample txtdata - no command line arguments required");
    addOption(ot, "--matlabdata", OPTION_BOOL, (char*)&matlabdata, 0, "sample matlab exported data - in this case the 4 command line arguments are reguired: stationElement input, stationSite input, stationElement output and stationSite output files, respectively.");
    addOption(ot, "--seq", OPTION_BOOL, (char*)&seq, 0, "extract the first 'n' records");
    addOption(ot, "--rand", OPTION_BOOL, (char*)&rnd, 0, "extract randomized 'n' records");

    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "TXT DATA SPECIFIC OPTIONS");
    addOption(ot, "--postfix", OPTION_CHAR, (char*)postfix, 1, "postfix of sample files");

    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "MATLAB EXPORTED DATA SPECIFIC OPTIONS");

    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "OTHER OPTIONS");
    addOption(ot, "--verbose", OPTION_BOOL, (char*)&verbose, 0, "verbose mode");

    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }

    printOut= verbose;
    
    if ( !sample )
        return eprintf("no main option specified");
    if ( !txtdata && !matlabdata )
        return eprintf("no data specified");
    if ( txtdata && matlabdata )
        return eprintf("both txtdata and matlabdata specified");
    if ( sampleSize <= 0 )
        return eprintf("invalid n: %d\n", sampleSize);
    if ( !seq && !rnd )
        return eprintf("nor seq neither rand specified");
    if ( txtdata && rnd )
        return eprintf("rand mode is supported currently when matlabdata is sampled");
    if ( seq && rnd )
        return eprintf("both seq and rand mode are specified");

    if ( sample && txtdata )
        err= sampleDatasetTXT(argc, argv);
    if ( sample && matlabdata )
        err= sampleDatasetMATLAB(argc, argv);
    
    destroyOptionTable(ot);

    return err;
}
