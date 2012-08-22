#ifndef _LOAD_OPTIONS_H_
#define _LOAD_OPTIONS_H_

#include "openbest-ds/config-ds.h"
#include "openbest-io/config-io.h"

/**
  * loads max distance option from file
  * @returns the read parameter
  */
int load_max_d();

/**
  * loads the coefficients of the polynomial from file
  * @param t pointer to the array of polynomials
  * @param n pointer to the length of polynomials
  */
void load_r_p(real** t, int* n);

#endif
