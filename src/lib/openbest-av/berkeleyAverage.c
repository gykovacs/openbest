#include <time.h>
#include <float.h>
#include "openbest-av/berkeleyAverage.h"
#include "openbest-av/scalpelMethods.h"
#include "openbest-ds/printOut.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/basicAlgorithms.h"
#include "openbest-av/berkeleyAverageResults.h"

berkeleyAverageResults* berkeleyAverage(stationElement2p** seIO, int* n_stationElement2IO, stationSite2p** ssIO, int* n_stationSite2IO, berkeleyAverageOptions* options)
{
    stationElement2p* se= *seIO;
    stationSite2p* ss= *ssIO;

    time_t start_time= time(NULL);
    int calling_records= *n_stationElement2IO;

    tprintf("Berkeley Average\n");

    tprintf("Called with %d temperature time series from %d unique sites\n", *n_stationElement2IO, *n_stationSite2IO);

    displayBAO(options);

    int* back_map= NULL;
    int n_back_map;
    int* start_pos= NULL;
    int n_start_pos;
    int* break_flags= NULL;
    int n_break_flags;

    int i, j, k;

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

        n_back_map= n;
        n_start_pos= n;
        n_break_flags= n;

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
        n_back_map= *n_stationElement2IO;
        n_break_flags= *n_stationElement2IO;
        n_start_pos= *n_stationElement2IO;
    }

    berkeleyAverageResults* results;

    // This is where the real heavy lifting is actually done
    results= berkeleyAverageCore(seIO, n_stationElement2IO, ssIO, n_stationSite2IO, options);
    results->options= options;

    // Add some additional meta data
    results->execution_started= start_time;
    results->initial_time_series= calling_records;
    results->post_scalpel_time_series= *n_stationElement2IO;

    // Reconstruct the baselines and shift statistics
    int* un= copyIA(back_map, n_back_map);
    int n_un;
    uniqueIA(&un, &n_un);

    int max_un= maxI(un, n_un);
    baselineStruct* baselines= (baselineStruct*)malloc(sizeof(baselineStruct)*max_un);
    int n_baselines= max_un;

    if ( start_pos )
        free(start_pos);
    if ( break_flags )
        free(break_flags);
    if ( back_map )
        free(back_map);

    real* emp_shifts= rnalloc(*n_stationElement2IO);
    int n_emp_shifts= *n_stationElement2IO;
    set(emp_shifts, n_emp_shifts, 0);
    real* move_shifts= copyRA(emp_shifts, n_emp_shifts);
    int n_move_shifts= n_emp_shifts;
    real* tob_shifts= copyRA(emp_shifts, n_emp_shifts);
    int n_tob_shifts= n_emp_shifts;
    real* gap_shifts= copyRA(emp_shifts, n_emp_shifts);
    int n_gap_shifts= n_emp_shifts;

    real* new_record_weights= rnalloc(n_un);
    int n_new_record_weights= n_un;
    set(new_record_weights, n_new_record_weights, 0);

    // Group baseline information by original site.
    int* f= inalloc(n_back_map);
    int n_f;
    for ( k= 0; k < n_un; ++k )
    {
        n_f= 0;
        for( i= 0; i < n_back_map; ++i )
            if ( back_map[i] == un[k] )
                f[n_f++]= i;

        baselines[un[k]].break_positions= inalloc(n_f);
        baselines[un[k]].break_flags= inalloc(n_f);
        baselines[un[k]].baseline= rnalloc(n_f);
        baselines[un[k]].record_weight= rnalloc(n_f);
        baselines[un[k]].site_weight= rnalloc(n_f);
        baselines[un[k]].n_break_positions= baselines[un[k]].n_break_flags=
                baselines[un[k]].n_baseline= baselines[un[k]].n_record_weight=
                baselines[un[k]].n_site_weight= n_f;
        for ( i= 0; i < n_f; ++i )
        {
            baselines[un[k]].break_positions[i]= start_pos[f[i]];
            baselines[un[k]].break_flags[i]= break_flags[f[i]];
            baselines[un[k]].baseline[i]= results->baselines[f[i]];
            baselines[un[k]].record_weight[i]= results->record_weights[f[i]];
            baselines[un[k]].site_weight[i]= results->site_weights[f[i]];
        }

        real sum= 0;
        for ( j= 0; j < baselines[un[k]].n_record_weight; ++j )
            if ( baselines[un[k]].record_weight[j] != -FLT_MAX )
                sum+= baselines[un[k]].record_weight[j];

        new_record_weights[un[k]]= sum;

        int m;
        int* f2= inalloc(baselines[un[k]].n_break_flags);
        int n_f2= 0;
        for ( m= 0; m < 4; ++i )
        {
            for ( i= 0; i < baselines[un[k]].n_break_flags; ++i )
                if ( baselines[un[k]].break_flags[i] == m )
                    f2[n_f2++]= i;
            real* b_end= rnalloc(n_f2);
            int n_b_end= n_f2;
            for ( i= 0; i < n_f2; ++i )
                b_end[i]= baselines[un[k]].baseline[f2[i]];
            real* b_start= rnalloc(n_f2);
            int n_b_start= n_f2;
            set(b_start, n_b_start, -FLT_MAX);

            int offset= 1;
            int* fx= inalloc(n_f2);
            int n_fx= n_f2;
            bool any_fx= false;
            for ( i= 0; i < n_f2; ++i )
            {
                fx[i]= b_start[i] != -FLT_MAX && f2[i] - offset > 0;
                any_fx|= fx[i];
            }

            while ( any_fx )
            {
                for ( i= 0; i < n_b_start; ++i )
                {
                    b_start[fx[i]]= baselines[un[k]].baseline[f2[fx[i]] - offset];
                    ++offset;

                    any_fx= false;
                    for ( j= 0; j < n_f2; ++j )
                    {
                        fx[j]= b_start[j] != -FLT_MAX && f2[j] - offset > 0;
                        any_fx|= fx[j];
                    }
                }
            }

            // Store some summary data on the effect of scalpel
            switch (m)
            {
                case 1:
                    for ( i= 0; i < n_f2; ++i )
                        gap_shifts[f[f2[i]]]= b_end[i] - b_start[i];
                    break;
                case 2:
                    for ( i= 0; i < n_f2; ++i )
                        move_shifts[f[f2[i]]]= b_end[i] - b_start[i];
                    break;
                case 3:
                    for ( i= 0; i < n_f2; ++i )
                        tob_shifts[f[f2[i]]]= b_end[i] - b_start[i];
                    break;
                case 4:
                    for ( i= 0; i < n_f2; ++i )
                        emp_shifts[f[f2[i]]]= b_end[i] - b_start[i];
                    break;
            }
        }
        free(f2);
    }

    // Update results structure with structured baseline results
    results->baselinesS= baselines;
    results->n_baselinesS= n_baselines;
    results->record_weights= new_record_weights;
    results->n_record_weights= n_new_record_weights;
    free(results->site_weights);
    results->site_weights= NULL;
    results->n_site_weights= 0;

    if ( options->fullBaselineMapping )
    {
        // TODO
    }

    // Append shift statistics
    if ( options->useScalpel )
    {
        // Metadata section
        if ( options->scalpelMetadata )
        {
            // Gaps in station records
            if ( options->scalpelGaps )
            {
                int n= 0;
                for ( i= 0; i < n_gap_shifts; ++i )
                    if ( fabs(gap_shifts[i]) < FLT_MAX/2 )
                        ++n;
                results->gap_baseline_shifts= rnalloc(n);
                results->n_gap_baseline_shifts= 0;

                for ( i= 0; i < n_gap_shifts; ++i )
                    if ( fabs(gap_shifts[i]) < FLT_MAX/2 )
                        results->gap_baseline_shifts[(results->n_gap_baseline_shifts)++]= gap_shifts[i];
            }

            // Declared and suspected station moves
            // Declared changes in time of observation
            if ( options->scalpelSuspectedMoves || options->scalpelDeclaredMoves )
            {
                int n= 0;
                for ( i= 0; i < n_move_shifts; ++i )
                    if ( fabs(move_shifts[i]) < FLT_MAX/2 )
                        ++n;
                results->move_baseline_shifts= rnalloc(n);
                results->n_move_baseline_shifts= 0;

                for ( i= 0; i < n_move_shifts; ++i )
                    if ( fabs(move_shifts[i]) < FLT_MAX/2 )
                        results->move_baseline_shifts[(results->n_move_baseline_shifts)++]= move_shifts[i];
            }

            // Declared changes in time of observation
            if ( options->scalpelTOBChanges )
            {
                int n= 0;
                for ( i= 0; i < n_tob_shifts; ++i )
                    if ( fabs(tob_shifts[i]) < FLT_MAX/2 )
                        ++n;
                results->tob_baseline_shifts= rnalloc(n);
                results->n_tob_baseline_shifts= 0;

                for ( i= 0; i < n_tob_shifts; ++i )
                    if ( fabs(tob_shifts[i]) < FLT_MAX/2 )
                        results->tob_baseline_shifts[(results->n_tob_baseline_shifts)++]= tob_shifts[i];
            }
        }

        // Empirical section
        if ( options->scalpelEmpirical )
        {
            int n= 0;
            for ( i= 0; i < n_emp_shifts; ++i )
                if ( fabs(emp_shifts[i]) < FLT_MAX/2 )
                    ++n;
            results->empirical_baseline_shifts= rnalloc(n);
            results->n_empirical_baseline_shifts= 0;

            for ( i= 0; i < n_emp_shifts; ++i )
                if ( fabs(emp_shifts[i]) < FLT_MAX/2 )
                    results->empirical_baseline_shifts[(results->n_empirical_baseline_shifts)++]= emp_shifts[i];
        }
    }

    // If requested, perform uncertainty calculation.
    if ( options->computeUncertainty )
    {
        tprintf("Berkeley Average Compute Uncertainty\n");

        // This section computes spatial uncertainty, that is the uncertainty
        // that results from incomplete spatial sampling of the globe.
        // It is only available if the local mode mapping functions are enabled.
        if ( options->computeEmpiricalSpatialUncertainty )
        {
            // Empirical mode spatial uncertainty (preferred).
            // TODO
        }
        else if ( options->computeAnalyticSpatialUncertainty )
        {
            // Alternative analytic uncertainty with no empirical
            // TODO
        }

        // This sections computes the statistical uncertainty, that is the
        // uncertainty resulting from noise and likely bias in the available
        // temperature records for the parts of the world that were sampled.
        if ( options->computeStatisticalUncertainty )
        {
            if ( options->localMode && !options->statisticalUncertaintyLocal )
            {
                // Used for a much faster, but highly approximate uncertainty
                // TODO
            }

            // The real effort for statistical uncertainty
            // TODO
        }
        // Build complete uncertainty from the two halves
        if ( options->computeStatisticalUncertainty && options->computeEmpiricalSpatialUncertainty )
        {
            // TODO
        }

        tprintf("Berkeley Average Compute Uncertainty\n");
    }

    // This section generates an animated representation of the data, if
    // requested.
    // if ( options->renderMovie )
    //  // TODO

    // Save results to disk cache to accelerate any future calls using the same configuration.
    // TODO

    /*if ( options->saveResults )
    {
        // TODO
    }*/
    tprintf("End of Berkeley Average\n");

    return results;
}
