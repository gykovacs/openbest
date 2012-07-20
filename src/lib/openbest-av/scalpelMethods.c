#include "openbest-av/scalpelMethods.h"
#include "openbest-ds/basicAlgorithms.h"

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
    seti(start_pos, n_stationElement2, 1);

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
            start_pos[cnt]= 1;
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
    seti(start_pos, n_stationElement2, 1);

    int k;
    int n= n_stationElement2;

    bool* cuts= (bool*)malloc(sizeof(bool)*n);

    for ( k= 0; k < n; ++k )
    {

    }


    tprintf("%d stations in result\n", cnt);
    tprintf("End of Split Station Moves");
}

void splitStationTOBChange(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, int persistence, flag_t* badFlags, int n_badFlags, real min_difference, int** back_mapIO, int** start_posIO)
{

}
