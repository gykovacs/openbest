#include <stdlib.h>

#include "openbest-av/movingAverages.h"
#include "openbest-ds/memFunctions.h"

void nPointMovingAverageD(double* A, int na, int n, double** BIO, int* nbIO)
{
    // Simple N_point moving average

    double* B= dnalloc(na - n + 1);
    int n_B= na - n + 1;
    int i, j, k;

    for ( i= 0; i < na - n + 1; ++i )
        B[i]= A[i];

    for ( k= 0; k < n; ++k )
        for ( i= 0; i < n_B; ++i )
            B[i]= B[i] + A[k + i];
    for ( i= 0; i < n_B; ++i )
        B[i]/= n;

    *BIO= B;
    *nbIO= n_B;
}

void nPointMovingAverageF(float* A, int na, int n, float** BIO, int* nbIO)
{
    float* B= (float*)malloc(sizeof(float)*(na - n + 1));
    int n_B= na - n + 1;
    int i, j, k;

    for ( i= 0; i < na - n + 1; ++i )
        B[i]= A[i];

    for ( k= 0; k < n; ++k )
        for ( i= 0; i < n_B; ++i )
            B[i]= B[i] + A[k + i];
    for ( i= 0; i < n_B; ++i )
        B[i]/= n;

    *BIO= B;
    *nbIO= n_B;
}

void nPointMovingAverageR(real* A, int na, int n, real** BIO, int* nbIO)
{
    real* B= rnalloc(na - n + 1);
    int n_B= na - n + 1;
    int i, j, k;

    for ( i= 0; i < na - n + 1; ++i )
        B[i]= A[i];

    for ( k= 0; k < n; ++k )
        for ( i= 0; i < n_B; ++i )
            B[i]= B[i] + A[k + i];
    for ( i= 0; i < n_B; ++i )
        B[i]/= n;

    *BIO= B;
    *nbIO= n_B;
}

void nPointMovingAverageT(temp_t* A, int na, int n, temp_t** BIO, int* nbIO)
{
    temp_t* B= tnalloc(na - n + 1);
    int n_B= na - n + 1;
    int i, j, k;

    for ( i= 0; i < na - n + 1; ++i )
        B[i]= A[i];

    for ( k= 0; k < n; ++k )
        for ( i= 0; i < n_B; ++i )
            B[i]= B[i] + A[k + i];
    for ( i= 0; i < n_B; ++i )
        B[i]/= n;

    *BIO= B;
    *nbIO= n_B;
}

void simpleMovingAverageRD(real* times, int n_times, double* values, int n_values, int n,
                    real** timesIO, int* n_timesIO, double** valuesIO, int* n_valuesIO)
{
    nPointMovingAverageR(times, n_times, n, timesIO, n_timesIO);
    nPointMovingAverageD(values, n_values, n, valuesIO, n_valuesIO);
}

void simpleAnnualMovingAverageRD(real* times, int n_times, double* values, int n_values,
                               real** timesIO, int* n_timesIO, double** valuesIO, int* n_valuesIO)
{
    simpleMovingAverageRD(times, n_times, values, n_values, 12, timesIO, n_timesIO, valuesIO, n_valuesIO);
}

void simpleMovingAverageRR(real* times, int n_times, real* values, int n_values, int n,
                    real** timesIO, int* n_timesIO, real** valuesIO, int* n_valuesIO)
{
    nPointMovingAverageR(times, n_times, n, timesIO, n_timesIO);
    nPointMovingAverageR(values, n_values, n, valuesIO, n_valuesIO);
}

void simpleAnnualMovingAverageRR(real* times, int n_times, real* values, int n_values,
                               real** timesIO, int* n_timesIO, real** valuesIO, int* n_valuesIO)
{
    simpleMovingAverageRR(times, n_times, values, n_values, 12, timesIO, n_timesIO, valuesIO, n_valuesIO);
}
