#include <stdlib.h>
#include <string.h>

#include "openbest-av/scalpelMethods.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-ds/mathFunctions.h"

void splitStationBreaks(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, int gap, int* badFlags, int n_badFlags, int** back_mapIO, int** start_posIO)
{
    tprintf("Begin Split Station Gaps\n");
    tprintf("Called with %d stations and gap length %d year(s)\n", *n_stationSite2IO, gap);

    int i, j;
    stationElement2p* se= *seIO;
    int n_stationElement2= *n_stationElement2IO;
    stationSite2p* ss= *ssIO;
    int n_stationSite2= *n_stationSite2IO;

    stationElement2p* se2= (stationElement2p*)malloc(sizeof(stationElement2p)*(n_stationElement2 + 20000));
    stationSite2p* ss2= (stationSite2p*)malloc(sizeof(stationSite2p)*(n_stationSite2 + 20000));
    int newLength= n_stationElement2;

    int cnt= 0;
    int* back_map= inalloc(n_stationElement2 + 20000);
    int* start_pos= inalloc(n_stationElement2 + 20000);
    seti(back_map, n_stationElement2, 0);
    seti(start_pos, n_stationElement2, 0);

    int maxn= longestDataSeries(se, n_stationElement2);

    int k;
    real* dates;
    int* f= inalloc(maxn);
    int n_f;
    flag_t* flagged= fnalloc(maxn);
    int* group= inalloc(maxn);
    int* un= inalloc(maxn);

    int last_pos;
    int cur_pos;
    int cnt_start;
    int n;
    int n_un;

    for ( k= 0; k < n_stationElement2; ++k )
    {
        n= se[k]->n_dates;
        dates= se[k]->dates;

        findFlags(se[k], badFlags, n_badFlags, f, &n_f);

        last_pos= 0;
        cur_pos= 0;
        group[last_pos]= 0;

        setf(flagged, n, false);

        //printArrayI("f", f, n_f);

        for ( i= 0; i < n_f; ++i )
            flagged[f[i]]= true;

        seti(group, n, 0);

        while ( cur_pos < n )
        {
            if ( !flagged[cur_pos] )
            {
                if ( !flagged[last_pos] )
                    if ( dates[cur_pos] - dates[last_pos] > gap )
                        for ( i= cur_pos; i < n; ++i )
                            group[i]= group[last_pos] + 1;
                last_pos= cur_pos;
            }
            ++cur_pos;
        }

        memcpy(un, group, n*sizeof(int));
        n_un= n;

        uniqueIA1(un, &n_un);

        if ( newLength < cnt + n_un )
        {
            /*printf("increase capacity %d\n", n_un); fflush(stdout);
            se2= (stationElement2p*)realloc(se2, sizeof(stationElement2p)*(newLength + n_un + 1000));
            //printf("a"); fflush(stdout);
            ss2= (stationSite2p*)realloc(ss2, sizeof(stationSite2p)*(newLength + n_un + 1000));
            //printf("b"); fflush(stdout);
            back_map= (int*)realloc(back_map, sizeof(int)*(newLength + n_un + 1000));
            //printf("c"); fflush(stdout);
            start_pos= (int*)realloc(start_pos, sizeof(int)*(newLength + n_un + 1000));
            //printf("d"); fflush(stdout);
            newLength+= n_un + 1000;*/
        }

        if ( n_un == 1 )
        {
            se2[cnt]= se[k];
            ss2[cnt]= ss[k];
            se[k]= NULL;
            back_map[cnt]= k;
            start_pos[cnt]= 0;
            ++cnt;
        }
        else
        {
            cnt_start= cnt;

            for ( j= 0; j < n_un; ++j )
            {
                n_f= 0;
                for ( i= 0; i < n; ++i )
                    if ( group[i] == un[j] )
                        f[n_f++]= i;
                se2[cnt]= createSE2Select(se[k], f, n_f);
                start_pos[cnt]= f[0];
                ++cnt;
            }
            for ( j= cnt_start; j < cnt; ++j )
            {
                //ss2[j]= ss[k];
                ss2[j]= createSS2NC(ss[k]);
                back_map[j]= k;
            }
            destroySS2(ss[k]);
            destroySE2(se[k]);
            se[k]= NULL;
        }
    }

    se2= (stationElement2p*)realloc(se2, sizeof(stationElement2p)*cnt);
    ss2= (stationSite2p*)realloc(ss2, sizeof(stationSite2p)*cnt);
    back_map= (int*)realloc(back_map, sizeof(int)*cnt);
    start_pos= (int*)realloc(start_pos, sizeof(int)*cnt);

    free(se);
    free(ss);
    free(f);
    free(flagged);
    free(group);
    free(un);

    *seIO= se2;
    *ssIO= ss2;

    *n_stationElement2IO= cnt;
    *n_stationSite2IO= cnt;
    *back_mapIO= back_map;
    *start_posIO= start_pos;

    tprintf("%d stations in result\n", cnt);
    tprintf("End of Split Station Gaps\n");
}


void splitStationMoves(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, bool declared_moves, bool suspected_moves, int** back_mapIO, int** start_posIO)
{
    tprintf("Begin Split Station Moves\n");
    tprintf("Called with %d stations and declared_moves: %d, suspected_moves: %d\n", *n_stationSite2IO, declared_moves, suspected_moves);

    int i, j;
    stationElement2p* se= *seIO;
    int n_stationElement2= *n_stationElement2IO;
    stationSite2p* ss= *ssIO;
    int n_stationSite2= *n_stationSite2IO;

    stationElement2p* se2= (stationElement2p*)malloc(sizeof(stationElement2p)*(n_stationElement2 + 20000));
    stationSite2p* ss2= (stationSite2p*)malloc(sizeof(stationSite2p)*(n_stationSite2 + 20000));

    int cnt= 0;
    int* back_map= inalloc(n_stationElement2 + 20000);
    int* start_pos= inalloc(n_stationElement2 + 20000);
    seti(back_map, n_stationElement2, 0);
    seti(start_pos, n_stationElement2, 0);

    int k;
    int n= n_stationElement2;

    int maxn= longestDataSeries(se, n_stationElement2);
    real* cuts= rnalloc(maxn);
    int n_cuts;
    real* dates;
    real* cuts2= rnalloc(maxn);
    int n_cuts2;
    int* f= inalloc(maxn);
    int n_f;

    int cnt_start;
    for ( k= 0; k < n; ++k )
    {
        if ( ss[k]->n_relocations > 0 )
        {
            if ( declared_moves && suspected_moves )
            {
                n_cuts= ss[k]->n_relocations;
                memcpy(cuts, ss[k]->relocations, sizeof(real)*n_cuts);
            }
            else if ( declared_moves )
            {
                n_cuts= 0;
                for ( i= 0; i < ss[k]->n_relocations; ++i )
                    if ( ss[k]->relocation_types[i] == DECLARED_MOVE )
                        cuts[n_cuts++]= ss[k]->relocations[i];
            }
            else if ( suspected_moves )
            {
                n_cuts= 0;
                for ( i= 0; i < ss[k]->n_relocations; ++i )
                    if ( ss[k]->relocation_types[i] == SUSPECTED_MOVE )
                        cuts[n_cuts++]= ss[k]->relocations[i];
            }
            else
            {
                se2[cnt]= se[k];
                ss2[cnt]= ss[k];
                back_map[cnt]= k;
                start_pos[cnt]= 0;
                ++cnt;
                continue;
            }

            dates= se[k]->dates;

            uniqueRA1(cuts, &n_cuts);
            cuts2[0]= minR(se[k]->dates, se[k]->n_dates) - 1;
            cuts2[n_cuts + 1]= maxR(se[k]->dates, se[k]->n_dates) + 1;
            memcpy(cuts2 + 1, cuts, sizeof(real)*n_cuts);
            n_cuts2= n_cuts + 2;

            n_f= 0;

            cnt_start= cnt;

            for ( j= 0; j < n_cuts2-1; ++j )
            {
                n_f= 0;
                for ( i= 0; i < se[k]->n_dates; ++i )
                {
                    if ( se[k]->dates[i] >= cuts2[j] && se[k]->dates[i] < cuts2[j+1] )
                    {
                        f[n_f++]= i;
                    }
                }

                if ( n_f )
                {
                    stationElement2p tmp= createSE2Select(se[k], f, n_f);
                    se2[cnt]= tmp;
                    start_pos[cnt]= f[0];
                    ++cnt;
                }
            }
            destroySE2(se[k]);
            se[k]= NULL;
            for ( j= cnt_start; j < cnt; ++j )
            {
                ss2[j]= createSS2NC(ss[k]);
                back_map[j]= k;
            }
            destroySS2(ss[k]);
        }
        else
        {
            se2[cnt]= se[k];
            ss2[cnt]= ss[k];
            back_map[cnt]= k;
            start_pos[cnt]= 0;
            ++cnt;
        }
    }

    se2= (stationElement2p*)realloc(se2, sizeof(stationElement2p)*cnt);
    ss2= (stationSite2p*)realloc(ss2, sizeof(stationSite2p)*cnt);
    back_map= (int*)realloc(back_map, sizeof(int)*cnt);
    start_pos= (int*)realloc(start_pos, sizeof(int)*cnt);

    free(se);
    free(ss);
    free(cuts);
    free(cuts2);
    free(f);

    *seIO= se2;
    *ssIO= ss2;
    *n_stationElement2IO= cnt;
    *n_stationSite2IO= cnt;
    *back_mapIO= back_map;
    *start_posIO= start_pos;


    tprintf("%d stations in result\n", cnt);
    tprintf("End of Split Station Moves\n");
}

void splitStationTOBChanges(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, real persistence, int* badFlags, int n_badFlags, real min_difference, int** back_mapIO, int** start_posIO)
{
    tprintf("Begin Split TOB Changes\n");
    tprintf("Called with %d stations and %f persistence %f min_difference\n", *n_stationSite2IO, persistence, min_difference);

    int i, j;
    stationElement2p* se= *seIO;
    int n_stationElement2= *n_stationElement2IO;
    stationSite2p* ss= *ssIO;
    int n_stationSite2= *n_stationSite2IO;

    stationElement2p* se2= (stationElement2p*)malloc(sizeof(stationElement2p)*(n_stationElement2 + 20000));
    stationSite2p* ss2= (stationSite2p*)malloc(sizeof(stationSite2p)*(n_stationSite2 + 20000));

    int cnt= 0;
    int* back_map= inalloc(n_stationElement2 + 20000);
    int* start_pos= inalloc(n_stationElement2 + 20000);
    seti(back_map, n_stationElement2, 0);
    seti(start_pos, n_stationElement2, 0);

    int k;
    real* dates;
    char* tob;
    int n;
    int* f= inalloc(10);
    int n_f;
    int next_pos;
    flag_t* flagged= fnalloc(10);
    int* group= inalloc(10);
    int last_pos;
    int cur_pos;
    char last_tob;
    bool good;
    int* un;
    int n_un;
    int cnt_start;

    for ( k= 0; k < n_stationElement2; ++k )
    {
        dates= se[k]->dates;
        tob= se[k]->time_of_observation;
        n= se[k]->n_dates;

        if ( maxC(tob, n) < 0 || maxC(tob, n) == minC(tob, n) )
        {
            se2[cnt]= se[k];
            ss2[cnt]= ss[k];
            back_map[cnt]= k;
            start_pos[cnt]= 0;
            ++cnt;
            continue;
        }

        f= (int*)realloc(f, sizeof(int)*n);
        flagged= (flag_t*)realloc(flagged, sizeof(flag_t)*n);
        group= (int*)realloc(group, sizeof(int)*n);
        seti(group, n, 0);

        last_pos= 0;
        cur_pos= 0;

        findFlags(se[k], badFlags, n_badFlags, f, &n_f);
        setf(flagged, n, false);
        for ( i= 0; i < n_f; ++i )
            flagged[f[i]]= true;

        last_tob= tob[0];

        while ( cur_pos < n )
        {
            if ( !flagged[cur_pos] )
            {
                if ( last_tob >= 0 && !flagged[last_pos] && tob[cur_pos] >= 0 && abs(tob[cur_pos] - last_tob) >= min_difference )
                {
                    next_pos= cur_pos;
                    good= true;
                    while ( dates[next_pos] - dates[cur_pos] < persistence )
                    {
                        ++next_pos;
                        if ( next_pos >= n )
                        {
                            good= false;
                            break;
                        }
                        if ( !flagged[next_pos] && tob[next_pos] < 0 || abs(tob[next_pos] - last_tob) < min_difference )
                        {
                            good= false;
                            break;
                        }
                    }
                    if ( good )
                    {
                        for ( i= cur_pos; i < n; ++i )
                            group[i]= group[last_pos] + 1;
                    }
                }
                last_pos= cur_pos;
                last_tob= tob[cur_pos];
            }
            ++cur_pos;
        }

        uniqueIAN(group, n, &un, &n_un);
        if ( n_un == 1 )
        {
            se2[cnt]= se[k];
            ss2[cnt]= ss[k];
            back_map[cnt]= k;
            start_pos[cnt]= 0;
            ++cnt;
        }
        else
        {
            cnt_start= cnt;
            stationElement2p tmp;
            for ( j= 0; j < n_un; ++j )
            {
                f= (int*)realloc(f, sizeof(int)*n);
                n_f= 0;
                for ( i= 0; i < n; ++i )
                    if ( group[i] == un[j] )
                        f[n_f++]= i;
                tmp= createSE2Select(se[k], f, n_f);
                se2[cnt]= tmp;
                start_pos[cnt]= f[0];
                ++cnt;
            }
            for ( i= cnt_start; i < cnt-1; ++i )
            {
                //ss2[i]= ss[k];
                ss2[i]= createSS2NC(ss[k]);
                back_map[i]= k;
            }
            destroySS2(ss[k]);
        }
        free(un);
    }

    se2= (stationElement2p*)realloc(se2, sizeof(stationElement2p)*cnt);
    ss2= (stationSite2p*)realloc(ss2, sizeof(stationSite2p)*cnt);
    back_map= (int*)realloc(back_map, sizeof(int)*cnt);
    start_pos= (int*)realloc(start_pos, sizeof(int)*cnt);

    free(se);
    free(ss);
    *seIO= se2;
    *ssIO= ss2;
    *n_stationElement2IO= cnt;
    *n_stationSite2IO= cnt;
    *back_mapIO= back_map;
    *start_posIO= start_pos;

    free(f);
    free(flagged);
    free(group);

    tprintf("%d stations in result\n", cnt);
    tprintf("End of Split TOB Changes\n");
}

void empiricalCuts(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, berkeleyAverageOptions* options, int** back_mapIO, int** start_posIO)
{
    tprintf("Scalpel Method Empirical Cuts\n");
    tprintf("Called with %d stations\n", *n_stationSite2IO);

    stationElement2p* se= *seIO;
    stationSite2p* ss= *ssIO;

    int cnt= 0;
    int selected= options->scalpelEmpiricalBestPairs;
    int consider= options->scalpelEmpiricalMaxPairs;
    int max_dist= options->scalpelEmpiricalMaxDistance;
    real primary_cut= options->scalpelEmpiricalCut;
    int segment_length=options->scalpelEmpiricalMaxSegment;

    int* bf= options->badFlags;
    int n_bf= options->n_badFlags;

    int n= *n_stationElement2IO;

    bool* truly_bad= bnalloc(n);
    setb(truly_bad, n, false);

    real* limsMin= rnalloc(n);
    real* limsMax= rnalloc(n);

    int i, j, k;
    for ( k= 0; k < n; ++k )
    {
        if ( se[k]->n_dates < 12 )
            truly_bad[k]= true;
        limsMin[k]= minR(se[k]->dates, se[k]->n_dates);
        limsMax[k]= maxR(se[k]->dates, se[k]->n_dates);
    }

    int block_size;

    if ( !options->clusterMode )
        block_size= 1000;
    else
        block_size= 50000;

    real* x= rnalloc(n);
    real* y= rnalloc(n);
    real* z= rnalloc(n);

    for ( k= 0; k < n; ++k )
    {
        x[k]= ss[k]->location->x;
        y[k]= ss[k]->location->y;
        z[k]= ss[k]->location->z;
    }

    tprintf("Distance Mapping\n");

    int** neighbor_list= (int**)malloc(sizeof(int*)*n);
    for ( k= 0; k < n; ++k )
        neighbor_list[k]= inalloc(consider + 1);
    real* distances= rnalloc(n);
    int* neighborhood= inalloc(n);
    int n_neighborhood;

    int block;
    for ( block= 0; block < n; block+= block_size )
    {
        int max_block= (block + block_size - 1) < (n - 1) ? (block + block_size - 1) : (n - 1);

        for ( k= block; k < max_block; ++k )
        {
            for ( i= 0; i < n; ++i )
                distances[i]= sqrt((x[i] - x[k]) * (x[i] - x[k]) + (y[i] - y[k])*(y[i] - y[k]) + (z[i] - z[k])*(z[i] - z[k]));
            distances[k]= FLT_MAX;

            n_neighborhood= 0;

            for ( i= 0; i < n; ++i )
                if ( distances[i] < max_dist && ! truly_bad[i] )
                    neighborhood[n_neighborhood++]= i;

            if ( n_neighborhood < consider )
            {
                for ( i= 0; i < n_neighborhood; ++i )
                    neighbor_list[k][i]= neighborhood[i];
                neighbor_list[k][i]= -1;
            }
            else
            {
                outer_array= distances;
                qsortORIA(neighborhood, n_neighborhood);

                for ( i= 0; i < consider; ++i )
                    neighbor_list[k][i]= neighborhood[i];
                neighbor_list[k][i]= -1;
            }
        }
    }

    tprintf("End of Distance Mapping\n");

    int** breaks;
    breaks= (int**)malloc(sizeof(int*)*n);
    int step_size= 1000;

    tprintf("Determine Cuts\n");

    for ( block= 0; block < n; block+= step_size )
    {
        int max_block= n < (block + step_size - 1) ? n : (block + step_size - 1);


    }

    //TODO continue on demand

    tprintf("%d stations in result\n", cnt);
    tprintf("End of Scalpel Method Empirical Cuts\n");
}
