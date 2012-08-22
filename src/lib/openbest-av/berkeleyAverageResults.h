#ifndef _BERKELEY_AVERAGE_RESULTS_H_
#define _BERKELEY_AVERAGE_RESULTS_H_

#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "openbest-ds/config-ds.h"
#include "openbest-ds/geoPoint2.h"
#include "openbest-ds/memFunctions.h"
#include "openbest-ds/printOut.h"
#include "openbest-av/berkeleyAverageOptions.h"

/** structure to represent baselines*/
typedef struct
{
    int* break_positions;
    int n_break_positions;
    int* break_flags;
    int n_break_flags;
    real* baseline;
    int n_baseline;
    real* record_weight;
    int n_record_weight;
    real* site_weight;
    int n_site_weight;
} baselineStruct;

/** structure for the results of the BEST algorithm*/
typedef struct
{
    double network_completeness;
    double quality_of_fit;
    double data_points;
    double time_parameters;
    double baseline_parameters;
    double adjusted_quality_of_fit;
    double reduced_baseline_parameters;

    real* record_weights;
    int n_record_weights;

    real* site_weights;
    int n_site_weights;

    real* times_monthly;
    int n_times_monthly;

    real* values_monthly;
    int n_values_monthly;

    real* times_annual;
    int n_times_annual;

    real* values_annual;
    int n_values_annual;

    real* times_five_year;
    int n_times_five_year;

    real* values_five_year;
    int n_values_five_year;

    real* times_ten_year;
    int n_times_ten_year;

    real* values_ten_year;
    int n_values_ten_year;

    real* times_twenty_year;
    int n_times_twenty_year;

    real* values_twenty_year;
    int n_values_twenty_year;

    real* baselines;
    int n_baselines;

    real* coverage_summary;
    int n_coverage_summary;

    geoPoint2** location_pts;
    int n_location_pts;

    bool* occurance_table;
    int n_occurance_table1;
    int n_occurance_table2;

    berkeleyAverageOptions* options;

    time_t execution_started;
    int initial_time_series;
    int post_scalpel_time_series;

    baselineStruct* baselinesS;
    int n_baselinesS;

    real* gap_baseline_shifts;
    int n_gap_baseline_shifts;

    real* move_baseline_shifts;
    int n_move_baseline_shifts;

    real* tob_baseline_shifts;
    int n_tob_baseline_shifts;

    real* empirical_baseline_shifts;
    int n_empirical_baseline_shifts;

} berkeleyAverageResults;

/**
  * destroys baselineStruct object
  * @param bs baselineStruct object to destroy
  */
void destroyBS(baselineStruct* bs);

/**
  * creates baselineStruct object
  * @returns pointer to new baselineStruct object
  */
baselineStruct* createBS();

/**
  * initializes the parameter baselineStruct object
  * @param bs pointer to baselineStruct instance
  */
void initBS(baselineStruct* bs);

/**
  * destroys the berkeleyAverageResults instance
  * @param bar berkeleyAverageResults to destroy
  */
void destroyBAR(berkeleyAverageResults* bar);

/**
  * creates a new berkeleyAverageResults instance
  * @returns pointer to the new berkeleyAverageResults object
  */
berkeleyAverageResults* createBAR();

#endif
