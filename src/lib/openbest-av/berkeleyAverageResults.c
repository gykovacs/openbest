#include "openbest-av/berkeleyAverageResults.h"

baselineStruct* createBS()
{
    baselineStruct* tmp= (baselineStruct*)malloc(sizeof(baselineStruct));

    tmp->break_positions= NULL;
    tmp->n_break_positions= 0;
    tmp->break_flags= NULL;
    tmp->n_break_flags= 0;
    tmp->baseline= NULL;
    tmp->n_baseline= 0;
    tmp->record_weight= NULL;
    tmp->n_record_weight= 0;
    tmp->site_weight= NULL;
    tmp->n_site_weight= 0;

    return tmp;
}

void initBS(baselineStruct* tmp)
{
    tmp->break_positions= NULL;
    tmp->n_break_positions= 0;
    tmp->break_flags= NULL;
    tmp->n_break_flags= 0;
    tmp->baseline= NULL;
    tmp->n_baseline= 0;
    tmp->record_weight= NULL;
    tmp->n_record_weight= 0;
    tmp->site_weight= NULL;
    tmp->n_site_weight= 0;
}

void destroyBS(baselineStruct* bs)
{
    if ( bs->break_positions )
        free(bs->break_positions);
    if ( bs->break_flags )
        free(bs->break_flags);
    if ( bs->baseline )
        free(bs->baseline);
    if ( bs->record_weight )
        free(bs->record_weight);
    if ( bs->site_weight )
        free(bs->site_weight);
    free(bs);
}

berkeleyAverageResults* createBAR()
{
    berkeleyAverageResults* bar= (berkeleyAverageResults*)malloc(sizeof(berkeleyAverageResults));

    bar->record_weights= NULL;
    bar->n_record_weights= 0;

    bar->site_weights= NULL;
    bar->n_site_weights= 0;

    bar->times_monthly= NULL;
    bar->n_times_monthly= 0;

    bar->values_monthly= NULL;
    bar->n_values_monthly= 0;

    bar->times_annual= NULL;
    bar->n_times_annual= 0;

    bar->values_annual= NULL;
    bar->n_values_annual= 0;

    bar->times_five_year= NULL;
    bar->n_times_five_year= 0;

    bar->values_five_year= NULL;
    bar->n_values_five_year= 0;

    bar->times_ten_year= NULL;
    bar->n_times_ten_year= 0;

    bar->values_ten_year= NULL;
    bar->n_values_ten_year= 0;

    bar->times_twenty_year= NULL;
    bar->n_times_twenty_year= 0;

    bar->values_twenty_year= NULL;
    bar->n_values_twenty_year= 0;

    bar->baselines= NULL;
    bar->n_baselines= 0;

    bar->coverage_summary= NULL;
    bar->n_coverage_summary= 0;

    bar->location_pts= NULL;
    bar->n_location_pts= 0;

    bar->occurance_table= NULL;
    bar->n_occurance_table1= 0;
    bar->n_occurance_table2= 0;

    bar->options= NULL;

    bar->baselinesS= NULL;
    bar->n_baselinesS= 0;

    bar->gap_baseline_shifts= NULL;
    bar->n_gap_baseline_shifts= 0;

    bar->move_baseline_shifts= NULL;
    bar->n_move_baseline_shifts= 0;

    bar->tob_baseline_shifts= NULL;
    bar->n_tob_baseline_shifts= 0;

    bar->empirical_baseline_shifts= NULL;
    bar->n_empirical_baseline_shifts= 0;

    return bar;
}

void destroyBAR(berkeleyAverageResults* bar)
{
    if ( bar->record_weights )
        free(bar->record_weights);
    if ( bar->site_weights )
        free(bar->site_weights);

    if ( bar->times_monthly )
        free(bar->times_monthly);
    if ( bar->values_monthly )
        free(bar->values_monthly);
    if ( bar->times_annual )
        free(bar->times_annual);
    if ( bar->values_annual )
        free(bar->values_annual);
    if ( bar->times_five_year )
        free(bar->times_five_year);
    if ( bar->values_five_year )
        free(bar->values_five_year);
    if ( bar->times_ten_year )
        free(bar->times_ten_year);
    if ( bar->values_ten_year )
        free(bar->values_ten_year);
    if ( bar->times_twenty_year)
        free(bar->times_twenty_year);
    if ( bar->values_twenty_year)
        free(bar->values_twenty_year);

    if ( bar->baselines )
        free(bar->baselines);
    if ( bar->coverage_summary )
        free(bar->coverage_summary);
    if ( bar->location_pts )
    {
        int i;
        for ( i= 0; i < bar->n_location_pts; ++i )
            free(bar->location_pts[i]);
        free(bar->location_pts);
    }
    if ( bar->occurance_table )
        free(bar->occurance_table);
    if ( bar->options )
        destroyBAO(bar->options);

    if ( bar->gap_baseline_shifts )
        free(bar->gap_baseline_shifts );
    if ( bar->move_baseline_shifts )
        free(bar->move_baseline_shifts);
    if ( bar->tob_baseline_shifts )
        free(bar->tob_baseline_shifts);
    if ( bar->empirical_baseline_shifts )
        free(bar->empirical_baseline_shifts);

    if ( bar->baselinesS )
    {
        int i;

        for ( i= 0; i < bar->n_baselinesS; ++i )
        {
            if ( bar->baselinesS[i].break_positions ) free(bar->baselinesS[i].break_positions);
            if ( bar->baselinesS[i].break_flags ) free(bar->baselinesS[i].break_flags);
            if ( bar->baselinesS[i].baseline ) free(bar->baselinesS[i].baseline);
            if ( bar->baselinesS[i].record_weight ) free(bar->baselinesS[i].record_weight);
            if ( bar->baselinesS[i].site_weight ) free(bar->baselinesS[i].site_weight);
        }
        free(bar->baselinesS);
    }
    free(bar);
}
