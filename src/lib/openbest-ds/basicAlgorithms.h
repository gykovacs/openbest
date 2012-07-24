#ifndef _BASIC_ALGORITHMS_H_
#define _BASIC_ALGORITHMS_H_

#include <stdio.h>
#include <stdlib.h>

#include "openbest-ds/config-ds.h"

#include <stdio.h>
#include <stdlib.h> 

real* outer_array;

/**
  * swaps two real variables
  * @param x pointer to one variable to swap
  * @param y pointer to the other variable to swap
  */
void swap(real *x,real *y);

/**
  * chooses pivot element for quick-sort, should not called directly
  * @param i beginning index of actual domain
  * @param j end index of actual domain
  * @returns index of pivot element
  */
int choose_pivot(int i,int j );

/**
  * quick sorts a real array of m element of n size
  * @param list pointer to the real array
  * @param m number of elements
  * @param n size of elements
  */
void quicksort(real* list,int m,int n);

/**
  * compares two integer variables
  * @param x pointer to one variable
  * @param y pointer to the other variable
  * @returns 0 if equal; 1 if *x > *y, -1 otherwise
  */
int compareIA(const void *x, const void *y);

int compareORIA(const void* x, const void* y);

/**
  * quick sorts an array of integer elemenets
  * @param t array to sort
  * @param n length of array
  */
void qsortIA(int* t, int n);

void qsortORIA(int* t, int n);

/**
  * compares two flag_t variables
  * @param x pointer to one variable
  * @param y pointer to the other variable
  * @returns 0 if equal; 1 if *x > *y, -1 otherwise
  */
int compareFA(const void *x, const void *y);

/**
  * quick sorts an array of flag_t elemenets
  * @param t array to sort
  * @param n length of array
  */
void qsortFA(flag_t* t, int n);

/**
  * compares two real variables
  * @param x pointer to one variable
  * @param y pointer to the other variable
  * @returns 0 if equal; 1 if *x > *y, -1 otherwise
  */
int compareRA(const void *x, const void *y);

/**
  * quick sorts an array of real elemenets
  * @param t array to sort
  * @param n length of array
  */
void qsortRA(real* t, int n);

/**
  * returns the unique elements in an array, the length of the array is modified
  * @param t pointer to the array
  * @param n pointer to the size of the array
  */
void uniqueIA(int** t, int* n);

/**
  * returns the unique elements in a newly allocated array, the length of the array is modified
  * @param input pointer to the array
  * @param n pointer to the size of the array
  * @param pointer to output array
  * @param pointer to output array length
  */
void uniqueIAN(int* input, int n, int** t, int* m);

/**
  * returns the unique elements in an array, the length of the array is modified
  * @param t pointer to the array
  * @param n pointer to the size of the array
  */
void uniqueRA(real** t, int* n);

/**
  * returns the unique elements in a newly allocated array, the length of the array is modified
  * @param input pointer to the array
  * @param n pointer to the size of the array
  * @param pointer to output array
  * @param pointer to output array length
  */
void uniqueRAN(real* input, int n, real** t, int* m);

real modeRMA(real* input, int n);

int modeRFA(real* input, int n);

void findEqualsIA(int* input, int n, int v, int* f, int* n_f);

void findNSmallerRA(real* input, int n, int N, real* output);

#endif
