#include "openbest-av/scalpelMethods.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-ds/mathFunctions.h"

void splitStationBreaks(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, int gap, flag_t* badFlags, int n_badFlags, int** back_mapIO, int** start_posIO)
{
    tprintf("Begin Split Station Gaps\n");
    tprintf("Called with %d staitons and gap length %d year(s)\n", *n_stationSite2IO, gap);

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

    int k;
    real* dates;
    int* f= inalloc(se[0]->n_dates);
    int n_f;
    int last_pos;
    int cur_pos;
    int cnt_start;
    flag_t* flagged= fnalloc(se[0]->n_dates);
    int* group= inalloc(se[0]->n_dates);
    int* un;
    int n;
    int n_un;

    for ( k= 0; k < n_stationElement2; ++k )
    {
        n= se[k]->n_dates;
        f= (int*)realloc(f, sizeof(int)*n);
        flagged= (flag_t*)realloc(flagged, sizeof(flag_t)*n);
        group= (int*)realloc(group, sizeof(int)*n);
        dates= se[k]->dates;

        findFlags(se[k], badFlags, n_badFlags, f, &n_f);

        last_pos= 0;
        cur_pos= 0;
        group[last_pos]= 0;

        setf(flagged, n, false);
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

        uniqueIAN(group, n, &un, &n_un);

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
                findEqualsIA(group, n, un[j], f, &n_f);
                se2[cnt]= createSE2Select(se[k], f, n_f);
                start_pos[cnt]= f[0];
                ++cnt;
            }
            for ( j= cnt_start; j < cnt; ++j )
            {
                ss2[j]= ss[k];
                back_map[j]= k;
            }
            destroySE2(se[k]);
            se[k]= NULL;
        }

        free(un);
    }

    printf("reallocs\n"); fflush(stdout);
    se2= (stationElement2p*)realloc(se2, sizeof(stationElement2p)*cnt);
    ss2= (stationSite2p*)realloc(ss2, sizeof(stationSite2p)*cnt);
    back_map= (int*)realloc(back_map, sizeof(int)*cnt);
    start_pos= (int*)realloc(start_pos, sizeof(int)*cnt);

    printf("end\n"); fflush(stdout);
    free(se);
    printf("xxx\n"); fflush(stdout);
    //free(ss);
    printf("yyy\n"); fflush(stdout);
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
    tprintf("End of Split Station Gaps");
}


void splitStationMoves(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, bool declared_moves, bool suspected_moves, flag_t* badFlags, int n_badFlags, int** back_mapIO, int** start_posIO)
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
    int newLength= n_stationElement2;

    int cnt= 0;
    int* back_map= inalloc(n_stationElement2 + 20000);
    int* start_pos= inalloc(n_stationElement2 + 20000);
    seti(back_map, n_stationElement2, 0);
    seti(start_pos, n_stationElement2, 0);

    int k;
    int n= n_stationElement2;

    real* cuts= rnalloc(10);
    int n_cuts;
    real* dates;
    real* cuts2= rnalloc(10);
    int n_cuts2;
    int* f= inalloc(10);
    bool flag;
    int n_f;

    int cnt_start;
    for ( k= 0; k < n; ++k )
    {
        if ( ss[k]->id == 20 )
            getchar();
        //printf("%d,%d,%d ", k, se[k]->site, ss[k]->id); fflush(stdout);
        if ( ss[k]->n_relocations > 0 )
        {
            if ( declared_moves && suspected_moves )
            {
                //deb();
                cuts= realloc(cuts, sizeof(real)*ss[k]->n_relocations);
                n_cuts= ss[k]->n_relocations;
                memcpy(cuts, ss[k]->relocations, sizeof(real)*n_cuts);
            }
            else if ( declared_moves )
            {
                //dec();
                n_cuts= 0;
                for ( i= 0; i < ss[k]->n_relocations; ++i )
                    if ( ss[k]->relocation_types[i] == 1 )
                        cuts[n_cuts++]= ss[k]->relocation_types[i];
            }
            else
            {
                //ded();
                n_cuts= 0;
                for ( i= 0; i < ss[k]->n_relocations; ++i )
                    if ( ss[k]->relocation_types[i] == 2 )
                        cuts[n_cuts++]= ss[k]->relocation_types[i];
            }
            //printf("a"); fflush(stdout);
            uniqueRA(&cuts, &n_cuts);
            dates= se[k]->dates;
            cuts2= rnalloc(n_cuts + 2);
            //printf("----- %f %f\n", minR(cuts, n_cuts), maxR(cuts, n_cuts));
            cuts2[0]= minR(se[k]->dates, se[k]->n_dates) - 1;
            cuts2[n_cuts + 1]= maxR(se[k]->dates, se[k]->n_dates) + 1;
            memcpy(cuts2 + 1, cuts, sizeof(real)*n_cuts);
            n_cuts2= n_cuts + 2;
            f= realloc(f, sizeof(int)*se[k]->n_dates);
            //printf("b %d ", n_cuts); fflush(stdout);
            n_f= 0;
            cnt_start= cnt;
            for ( j= 0; j < n_cuts2 - 1; ++j )
            {
                n_f= 0;
                for ( i= 0; i < se[k]->n_dates; ++i )
                {
                    //printf("."); fflush(stdout);
                    //printf("%f %f %f\n", se[k]->dates[i], cuts2[j], cuts2[j+1]); fflush(stdout);
                    if ( se[k]->dates[i] >= cuts2[j] && se[k]->dates[i] < cuts2[j+1] )
                    {
                        f[n_f++]= i;
                    }
                }

                if ( n_f )
                {
                    //printf("x %d/%d/%d ", n_f, se[k]->n_dates, cnt); fflush(stdout);
                    stationElement2p tmp= createSE2Select(se[k], f, n_f);
                    //printf("z"); fflush(stdout);
                    se2[cnt]= tmp;
                    start_pos[cnt]= f[0];
                    ++cnt;
                    //printf("y"); fflush(stdout);
                }
            }
            //printf("c"); fflush(stdout);
            destroySE2(se[k]);
            //printf("d"); fflush(stdout);
            se[k]= NULL;
            for ( j= cnt_start; j < cnt; ++j )
            {
                ss2[j]= ss[k];
                back_map[j]= k;
            }
            //printf("e"); fflush(stdout);
        }
        else
        {
            //dee();
            se2[cnt]= se[k];
            ss2[cnt]= ss[k];
            /*se[k]= NULL;
            ss[k]= NULL;*/
            back_map[cnt]= k;
            start_pos[cnt]= 0;
            ++cnt;
            //def();
        }
    }

    se2= (stationElement2p*)realloc(se2, sizeof(stationElement2p)*cnt);
    ss2= (stationSite2p*)realloc(ss2, sizeof(stationSite2p)*cnt);
    back_map= (int*)realloc(back_map, sizeof(int)*cnt);
    start_pos= (int*)realloc(start_pos, sizeof(int)*cnt);

    printf("end\n"); fflush(stdout);
    free(se);
    printf("xxx\n"); fflush(stdout);
    //free(ss);
    printf("yyy\n"); fflush(stdout);
    *seIO= se2;
    *ssIO= ss2;
    *n_stationElement2IO= cnt;
    *n_stationSite2IO= cnt;
    *back_mapIO= back_map;
    *start_posIO= start_pos;

    tprintf("%d stations in result\n", cnt);
    tprintf("End of Split Station Moves");
}

void splitStationTOBChange(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, real persistence, flag_t* badFlags, int n_badFlags, real min_difference, int** back_mapIO, int** start_posIO)
{
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

    int k;
    real* dates;
    char* tob;
    int n;
    int* f;
    int n_f;
    int next_pos;
    flag_t* flagged;
    int* group;
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

        if ( maxI(tob, n) < 0 || maxI(tob, n) == minI(tob, n) )
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
                ss2[i]= ss[k];
                back_map[i]= k;
            }
        }
        free(un);
    }

    printf("reallocs\n"); fflush(stdout);
    se2= (stationElement2p*)realloc(se2, sizeof(stationElement2p)*cnt);
    ss2= (stationSite2p*)realloc(ss2, sizeof(stationSite2p)*cnt);
    back_map= (int*)realloc(back_map, sizeof(int)*cnt);
    start_pos= (int*)realloc(start_pos, sizeof(int)*cnt);

    printf("end\n"); fflush(stdout);
    free(se);
    printf("xxx\n"); fflush(stdout);
    //free(ss);
    printf("yyy\n"); fflush(stdout);
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
    tprintf("End of Split Station Gaps");
}
