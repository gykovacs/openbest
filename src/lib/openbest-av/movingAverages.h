#ifndef _MOVING_AVERAGES_H_
#define _MOVING_AVERAGES_H_

#include "openbest-ds/config-ds.h"

/**
  * nPointMovingAverage from the Matlab-BEST code for double
  * @param A vector
  * @param na length of vector A
  * @param n length of average kernel
  * @param B pointer to a double* variable for output
  * @param nb pointer to an int variable for output
  */
void nPointMovingAverageD(double* A, int na, int n, double** B, int* nb);

/**
  * nPointMovingAverage from the Matlab-BEST code for float
  * @param A vector
  * @param na length of vector A
  * @param n length of average kernel
  * @param B pointer to a float* variable for output
  * @param nb pointer to an int variable for output
  */
void nPointMovingAverageF(float* A, int na, int n, float** B, int* nb);

/**
  * nPointMovingAverage from the Matlab-BEST code for real
  * @param A vector
  * @param na length of vector A
  * @param n length of average kernel
  * @param B pointer to a real* variable for output
  * @param nb pointer to an int variable for output
  */
void nPointMovingAverageR(real* A, int na, int n, real** B, int* nb);

/**
  * nPointMovingAverage from the Matlab-BEST code for temp_t
  * @param A vector
  * @param na length of vector A
  * @param n length of average kernel
  * @param B pointer to a temp_t* variable for output
  * @param nb pointer to an int variable for output
  */
void nPointMovingAverageT(temp_t* A, int na, int n, temp_t** B, int* nb);

/**
  * simpleMovingAverage function from the Matlab-BEST code, from real to double
  * @param times array of time values
  * @param n_times length of array times
  * @param values array of values
  * @param n_values length of array values
  * @param n length of average kernel
  * @param timesIO average time series output
  * @param n_timesIO length of timesIO
  * @param valuesIO average value series output
  * @param n_valuesIO length of valuesIO
  */
void simpleMovingAverageRD(real* times, int n_times, double* values, int n_values, int n,
                    real** timesIO, int* n_timesIO, double** valuesIO, int* n_valuesIO);

/**
  * simpleAnnualMovingAverage function from the Matlab-BEST code, from real to double
  * @param times array of time values
  * @param n_times length of array times
  * @param values array of values
  * @param n_values length of array values
  * @param timesIO average time series output
  * @param n_timesIO length of timesIO
  * @param valuesIO average value series output
  * @param n_valuesIO length of valuesIO
  */
void simpleAnnualMovingAverageRD(real* times, int n_times, double* values, int n_values,
                               real** timesIO, int* n_timesIO, double** valuesIO, int* n_valuesIO);

/**
  * simpleMovingAverage function from the Matlab-BEST code, from real to real
  * @param times array of time values
  * @param n_times length of array times
  * @param values array of values
  * @param n_values length of array values
  * @param n length of average kernel
  * @param timesIO average time series output
  * @param n_timesIO length of timesIO
  * @param valuesIO average value series output
  * @param n_valuesIO length of valuesIO
  */
void simpleMovingAverageRR(real* times, int n_times, real* values, int n_values, int n,
                    real** timesIO, int* n_timesIO, real** valuesIO, int* n_valuesIO);

/**
  * simpleAnnualMovingAverage function from the Matlab-BEST code, from real to real
  * @param times array of time values
  * @param n_times length of array times
  * @param values array of values
  * @param n_values length of array values
  * @param timesIO average time series output
  * @param n_timesIO length of timesIO
  * @param valuesIO average value series output
  * @param n_valuesIO length of valuesIO
  */
void simpleAnnualMovingAverageRR(real* times, int n_times, real* values, int n_values,
                               real** timesIO, int* n_timesIO, real** valuesIO, int* n_valuesIO);

#endif
