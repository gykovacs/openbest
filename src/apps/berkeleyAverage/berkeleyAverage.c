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
#include "openbest-ds/makeSingleValued.h"
#include "openbest-av/scalpelMethods.h"
#include "openbest-ds/printOut.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/config-ds.h"


#include "openbest-ds/optionTable.h"
#include "openbest-av/berkeleyAverage.h"

bool printMonthlyTimeSeries= false;
bool fileOutput= false;
bool summaryOutput= false;

bool verbose= false;

bool quick= false;

int gridsize= 16000;

bool txtdata= false;
bool matlabdata= false;

void processResults(berkeleyAverageResults* results)
{
    int i;

    if ( printMonthlyTimeSeries )
    {
        for ( i= 0; i < results->n_times_monthly; ++i )
            fprintf(stderr, "%f %f\n", results->times_monthly[i], results->values_monthly[i]);
    }
    if ( summaryOutput )
    {
        printf("network_completeness: %f\n", results->network_completeness);
        printf("quality_of_fit: %f\n", results->quality_of_fit);
        printf("data_points: %f\n", results->data_points);
        printf("time_parameters: %f\n", results->time_parameters);
        printf("baseline_parameters: %f\n", results->baseline_parameters);
        printf("initial_time_series: %d\n", results->initial_time_series);
        printf("post_scalpel_time_series: %d\n", results->post_scalpel_time_series);
    }
    if ( fileOutput )
    {
        FILE* rw= fopen("record_weights.result", "wt");
        for ( i= 0; i < results->n_record_weights; ++i )
            fprintf(rw, "%f\n", results->record_weights[i]);
        fclose(rw);

        FILE* sw= fopen("site_weights.result", "wt");
        for ( i= 0; i < results->n_site_weights; ++i )
            fprintf(sw, "%f\n", results->site_weights[i]);
        fclose(sw);

        FILE* monthly= fopen("monthly.result", "wt");
        for ( i= 0; i < results->n_times_monthly; ++i )
            fprintf(monthly, "%f %f\n", results->times_monthly[i], results->values_monthly[i]);
        fclose(monthly);

        FILE* annual= fopen("annual.result", "wt");
        for ( i= 0; i < results->n_times_annual; ++i )
            fprintf(annual, "%f %f\n", results->times_annual[i], results->values_annual[i]);
        fclose(annual);

        FILE* five_year= fopen("five_year.result", "wt");
        for ( i= 0; i < results->n_times_five_year; ++i )
            fprintf(five_year, "%f %f\n", results->times_five_year[i], results->values_five_year[i]);
        fclose(five_year);

        FILE* ten_year= fopen("ten_year.result", "wt");
        for ( i= 0; i < results->n_times_ten_year; ++i )
            fprintf(ten_year, "%f %f\n", results->times_ten_year[i], results->values_ten_year[i]);
        fclose(ten_year);

        FILE* twenty_year= fopen("twenty_year.result", "wt");
        for ( i= 0; i < results->n_times_twenty_year; ++i )
            fprintf(twenty_year, "%f %f\n", results->times_twenty_year[i], results->values_twenty_year[i]);
        fclose(twenty_year);

        FILE* baselines= fopen("baselines.result", "wt");
        for ( i= 0; i < results->n_baselines; ++i )
            fprintf(baselines, "%f\n", results->baselines[i]);
        fclose(baselines);

        FILE* coverage_summary= fopen("coverage_summary.result", "wt");
        for ( i= 0; i < results->n_coverage_summary; ++i )
            fprintf(coverage_summary, "%f\n", results->coverage_summary[i]);
        fclose(coverage_summary);

        FILE* occurance_table= fopen("occurence_table.result", "wt");
        int j;
        for ( i= 0; i < results->n_occurance_table1; ++i )
        {
            for ( j= 0; j < results->n_occurance_table2; ++j )
                fprintf(occurance_table, "%d ", results->occurance_table[i*(results->n_occurance_table2) + j]);
            fprintf(occurance_table, "\n");
        }
        fclose(occurance_table);

        FILE* gbs= fopen("gap_baseline_shifts.result", "wt");
        for ( i= 0; i < results->n_gap_baseline_shifts; ++i )
            fprintf(gbs, "%f\n", results->gap_baseline_shifts[i]);
        fclose(gbs);

        FILE* mbs= fopen("move_baseline_shifts.result", "wt");
        for ( i= 0; i < results->n_move_baseline_shifts; ++i )
            fprintf(mbs, "%f\n", results->move_baseline_shifts[i]);
        fclose(mbs);

        FILE* tbs= fopen("tob_baseline_shifts.result", "wt");
        for ( i= 0; i < results->n_tob_baseline_shifts; ++i )
            fprintf(tbs, "%f\n", results->tob_baseline_shifts[i]);
        fclose(tbs);
    }
}

int berkeleyAverageFunction(int argc, char** argv)
{
    int i, j, k, m;

    initDS();

    printOut= verbose;

    stationSite2p* ss;
    int n_stationSite2;
    stationElement2p* se;
    int n_stationElement2;

    loadPreliminaryData();

    berkeleyAverageOptions* bao;

    if ( quick )
        bao= createBerkeleyAverageOptionsQuick();

    bao->gridSize= gridsize;

    stationElement2p tmp;

    tprintf("Reading station elements...\n");

    if ( matlabdata )
    {
        //reading the data files exported from Matlab

        //read flags, etc.

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
    }
    else if ( txtdata )
    {
        // reading the .txt data files
        loadData(&ss, &n_stationSite2, &se, &n_stationElement2);
    }

    tprintf("reading finished\n");

    tprintf("Making dataset to be single valued\n");
    for ( i= 0; i < n_stationElement2; ++i )
    {
        tmp= makeSingleValued(se[i], NULL, 0);
        destroySE2(se[i]);
        se[i]= tmp;
    }
    tprintf("Converting to single valued finished\n");

    berkeleyAverageResults* results;

    bao->useScalpel=true;
    bao->scalpelGaps= false;
    bao->scalpelDeclaredMoves= false;
    bao->scalpelSuspectedMoves= false;
    bao->scalpelTOBChanges=false;
    bao->scalpelMetadata= false;

    results= berkeleyAverage(&se, &n_stationElement2, &ss, &n_stationSite2, bao);

    processResults(results);

    destroyBAR(results);

    destroySE2V(se, n_stationElement2);

    destroySS2V(ss, n_stationSite2);

    tprintf("aaa\n");
    finalizeDS();
    tprintf("bbb\n");

    return 0;
}

/**
* This simple test application calls a test function from the openbest-ds (data structures) shared object file.
*/
int main(int argc, char** argv)
{
    optionTable* ot= createOptionTable();
    
    setPrefix(ot, "\nberkeleyAverage application \n===========================\n\t written by Gyorgy Kovacs (gyuriofkovacs@gmail.com)");
    addUsage(ot, "\t\tberkeleyAverage <option-name> arg1 arg2 ...\n\t\tberkeleyAverage --berkeleyav --quick --summary --files --matlabdata --verbose se200.dat sites200.dat\n\t\tberkeleyAverage --berkeleyav --quick --summary --files --txtdata --verbose");
    
    bool berkeleyav= false;
    
    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "GENERAL OPTIONS");
    addOption(ot, "--berkeleyav", OPTION_BOOL, (char*)&berkeleyav, 0, "run berkeley average algorithm");
    addOption(ot, "--txtdata", OPTION_BOOL, (char*)&txtdata, 0, "use the txt data, no additional command line arguments are required");
    addOption(ot, "--matlabdata", OPTION_BOOL, (char*)&matlabdata, 0, "use the exported data from Matlab, additinally give the names of the files containing the stationElement and stationSite data, as command line arguments, respectively.");
    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "OPTION PRESETS");
    addOption(ot, "--quick", OPTION_BOOL, (char*)&quick, 0, "quick mode");
    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "RESULTS EXPORT");
    //addOption(ot, "--pmts", OPTION_BOOL, (char*)&printMonthlyTimeSeries, 0, "print monthly time series to stderr");
    addOption(ot, "--verbose", OPTION_BOOL, (char*)&verbose, 0, "verbose mode");
    addOption(ot, "--files", OPTION_BOOL, (char*)&fileOutput, 0, "save the results to files");
    addOption(ot, "--summary", OPTION_BOOL, (char*)&summaryOutput, 0, "print summary to stdout");
    addOption(ot, "", OPTION_SEPARATOR, NULL, 0, "BERKELEY AVERAGE OPTIONS");
    addOption(ot, "--gridsize", OPTION_INT, (char*)&gridsize, 1, "number of grid cells");
    
    if ( processArgs(ot, &argc, argv) )
    {
        destroyOptionTable(ot);
        return -1;
    }
    
    if ( !txtdata && !matlabdata )
        return eprintf("no data specified");
    if ( txtdata && matlabdata )
        return eprintf("txtdata and matlabdata specified in the same time");
    if ( !fileOutput && !summaryOutput )
        return eprintf("no result processing specified");
    if ( !quick )
        return eprintf("no option preset specified");
    if ( !berkeleyav )
        return eprintf("main algorithm not specified");

    int err;
    if ( berkeleyav )
        err= berkeleyAverageFunction(argc, argv);
    
    destroyOptionTable(ot);

    return err;
}
