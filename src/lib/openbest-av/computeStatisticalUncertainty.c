#include "openbest-av/computeStatisticalUncertainty.h"

/*void computeStatisticalUncertaitny(berkeleyAverageOptions* options)
{
    int inner_loops= options->statisticalUncertaintyInnerLoops;
    int outer_loops= options->statisticalUncertaintyOuterLoops;
    int min_repeats= options->statisticalUncertaintyMinRepeats;
    int min_align_date= options->statisticalUncertaintyBenchmarkMinDate;
    int max_align_date= options->statisticalUncertaintyBenchmarkMaxDate;

    tprintf("Compute Statistical Uncertainty\n");

    tprintf("Records: %d\n", n_se);
    tprintf("Outer Loops: %d\n", outer_loops);
    tprintf("Inner Loops: %d\n", inner_loops);
    tprintf("Min Repeats: %d\n", min_repeats);
    tprintf("Min Align: %d\n", min_align_date);
    tprintf("Max Align: %d\n", max_align_date);

    char types[14][5]={"monthly", "annual", "five_year", "ten_year", "twenty_year"};
    int n_types= 5;

    float** o_times_sub= (float**)malloc(sizeof(float*)*outer_loops*n_types);
    float** o_values_sub= (float**)malloc(sizeof(float*)*outer_loops*n_types);

    // Compute all now (random number generator may get upset later)
    float* RT= (float*)malloc(sizeof(float)*n_se * outer_loops);

    int i, j, k, l;
    for ( k= 0; k < outer_loops; ++k )
    {
        // Load individually so we will get the same random numbers in the k-th
        // row regardsless of the number of outer loops.
        for ( i= 0; i < n_se; ++i )
            RT[i*outer_loops + k]= ceil()
    }

}*/
