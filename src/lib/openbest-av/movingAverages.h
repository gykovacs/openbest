#ifndef _MOVING_AVERAGES_H_
#define _MOVING_AVERAGES_H_

#include "openbest-ds/config-ds.h"

void nPointMovingAverageD(double* A, int na, int n, double** B, int* nb);

void nPointMovingAverageF(float* A, int na, int n, float** B, int* nb);

void nPointMovingAverageR(real* A, int na, int n, real** B, int* nb);

void nPointMovingAverageT(temp_t* A, int na, int n, temp_t** B, int* nb);

void simpleMovingAverageRD(real* times, int n_times, double* values, int n_values, int n,
                    real** timesIO, int* n_timesIO, double** valuesIO, int* n_valuesIO);

void simpleAnnualMovingAverageRD(real* times, int n_times, double* values, int n_values,
                               real** timesIO, int* n_timesIO, double** valuesIO, int* n_valuesIO);

void simpleMovingAverageRR(real* times, int n_times, real* values, int n_values, int n,
                    real** timesIO, int* n_timesIO, real** valuesIO, int* n_valuesIO);

void simpleAnnualMovingAverageRR(real* times, int n_times, real* values, int n_values,
                               real** timesIO, int* n_timesIO, real** valuesIO, int* n_valuesIO);

#endif
