#include "openbest-av/berkeleyAverage.h"
#include "openbest-av/scalpelMethods.h"
#include "openbest-ds/printOut.h"
#include "openbest-ds/memFunctions.h"

void berkeleyAverage(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, berkeleyAverageOptions* options)
{
    stationElement2p* se= *seIO;
    stationSite2p* ss= *ssIO;

    tprintf("Berkeley Average\n");

    tprintf("Called with %d temperature time series from %d unique sites\n", *n_stationElement2IO, *n_stationSite2IO);

    displayBAO(options);

    int* back_map;
    int* start_pos;
    int* break_flags;
    if ( options->useScalpel )
    {
        tprintf("Berkeley Average Scalpel Methods\n");

        int* back_map1= NULL;
        int* start_pos1= NULL;
        int* back_map2= NULL;
        int* start_pos2= NULL;
        int* back_map3= NULL;
        int* start_pos3= NULL;
        int* back_map4= NULL;
        int* start_pos4= NULL;

        int i;

        // Metadata section
        if ( options->scalpelMetadata)
        {
            // Gaps in station records
            if ( options->scalpelGaps )
                splitStationBreaks(seIO, n_stationElement2IO, ssIO, n_stationSite2IO, options->scalpelGapLength, options->badFlags, options->n_badFlags, &back_map1, &start_pos1);
            else
            {
                back_map1= inalloc(*n_stationElement2IO);
                start_pos1= inalloc(*n_stationElement2IO);
                for ( i= 0; i < *n_stationElement2IO; ++i )
                {
                    back_map1[i]= i;
                    start_pos1[i]= 0;
                }
            }

            // Declared and suspected station moves
            splitStationMoves(seIO, n_stationElement2IO, ssIO, n_stationSite2IO, options->scalpelDeclaredMoves, options->scalpelSuspectedMoves, &back_map2, &start_pos2);

            // Declared changes in time of observation
            if ( options->scalpelTOBChanges )
                splitStationTOBChanges(seIO, n_stationElement2IO, ssIO, n_stationSite2IO, options->scalpelTOBPersistence, options->badFlags, options->n_badFlags, options->scalpelTOBDifference, &back_map3, &start_pos3);
            else
            {
                back_map3= inalloc(*n_stationElement2IO);
                start_pos3= inalloc(*n_stationElement2IO);
                for ( i= 0; i < *n_stationElement2IO; ++i )
                {
                    back_map3[i]= i;
                    start_pos3[i]= 0;
                }
            }
        }
        else
        {
            for ( i= 0; i < *n_stationElement2IO; ++i )
            {
                back_map1[i]= back_map2[i]= back_map3[i]= i;
                start_pos1[i]= start_pos2[i]= start_pos3[i]= 0;
            }
        }

        // Empirical section
        if ( options->scalpelEmpirical )
        {
            empiricalCuts(seIO, n_stationElement2IO, ssIO, n_stationSite2IO, options, &back_map4, &start_pos4);
        }
        else
        {
            back_map4= inalloc(*n_stationElement2IO);
            start_pos4= inalloc(*n_stationElement2IO);
            for ( i= 0; i < *n_stationElement2IO; ++i )
            {
                back_map4[i]= i;
                start_pos4[i]= 0;
            }
        }

        tprintf("End of Berkeley Average Scalpel Methods\n");

        //Build Reverse Split Lookup Table;

        int n= *n_stationElement2IO;
        back_map= back_map4;
        start_pos= start_pos4;
        int* break_flags= inalloc(n);
        seti(break_flags, n, 0);
        for ( i= 0; i < n; ++i )
            if ( start_pos4[i] != 1 )
                break_flags[i]= 4;

        for ( i= 0; i < n; ++i )
        {
            start_pos[i]= start_pos[i] + start_pos3[back_map[i]] - 1;
            if ( start_pos3[back_map[i]] != 1 && !break_flags[i] )
                break_flags[i]= 3;
            back_map[i]= back_map3[back_map[i]];
        }

        for ( i= 0; i < n; ++i )
        {
            start_pos[i]= start_pos[i] + start_pos2[back_map[i]] - 1;
            if ( start_pos2[back_map[i]] != 1 && !break_flags[i] )
                break_flags[i]= 2;
            back_map[i]= back_map2[back_map[i]];
        }

        for ( i= 0; i < n; ++i )
        {
            start_pos[i]= start_pos[i] + start_pos1[back_map[i]] - 1;
            if ( start_pos1[back_map[i]] != 1 && !break_flags[i] )
                break_flags[i]= 1;
            back_map[i]= back_map1[back_map[i]];
        }

        free(back_map1);
        free(start_pos1);
        free(back_map2);
        free(start_pos2);
        free(back_map3);
        free(start_pos3);
/*        free(back_map4);
        free(start_pos4);*/
    }
    else
    {
        start_pos= inalloc(*n_stationElement2IO);
        seti(start_pos, *n_stationElement2IO, 0);
        break_flags= inalloc(*n_stationElement2IO);
        seti(break_flags, *n_stationElement2IO, 0);
        back_map= inalloc(*n_stationElement2IO);
        int i;
        for ( i= 0; i < *n_stationElement2IO; ++i )
            back_map[i]= i;
    }
}
