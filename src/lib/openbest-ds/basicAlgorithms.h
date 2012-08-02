#ifndef _BASIC_ALGORITHMS_H_
#define _BASIC_ALGORITHMS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openbest-ds/config-ds.h"

/** outer array for indirect sorting using real values */
real* outer_array;
/** outer array for indirect sorting using temp_t values */
temp_t* outer_arrayT;
/** outer array for indirect sorting using int values */
int* outer_arrayI;

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

/**
  * compares the values of the outer_arrayR at indeces x and y
  * @param x pointer to one index
  * @param y pointer to other index
  * @returns 0 if equal; 1 if outer_arrayR[*x] > outer_arrayR[*y]; -1 otherwise
  */
int compareORIA(const void* x, const void* y);

/**
  * compares the values of the outer_arrayT at indeces x and y
  * @param x pointer to one index
  * @param y pointer to other index
  * @returns 0 if equal; 1 if outer_arrayT[*x] > outer_arrayT[*y]; -1 otherwise
  */
int compareOTIA(const void* x, const void* y);

/**
  * compares the values of the outer_arrayI at indeces x and y
  * @param x pointer to one index
  * @param y pointer to other index
  * @returns 0 if equal; 1 if outer_arrayI[*x] > outer_arrayI[*y]; -1 otherwise
  */
int compareOIIA(const void* x, const void* y);

/**
  * quick sorts an array of integer elemenets
  * @param t array to sort
  * @param n length of array
  */
void qsortIA(int* t, int n);

/**
  * quick sorts an array of integers using outer_array
  * @param t array to sort
  * @param n length of the array
  */
void qsortORIA(int* t, int n);

/**
  * quick sorts an array of integers using outer_arrayR
  * @param t array to sort
  * @param n length of the array
  */
void qsortOTIA(int* t, int n);

/**
  * quick sorts an array of integers using outer_arrayT
  * @param t array to sort
  * @param n length of the array
  */
void qsortOIIA(int* t, int n);

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

/**
  * Matlab-kind unique function
  * @param input array of integers
  * @param n length of input array
  * @param collapsed output variable - pointer to an int* variable, contains the indeces of the elements of the unique array
  * @param n_collapsed output variable - pointer to an int variable, it will contain the length of the collapsed array
  * @param expand_map output variable - pointer to an int* variable, that will contain the indeces of the elements of input in array collapsed
  * @param n_expand_map output variable - pointer to an int variable, it will contain the length of the array expand_map (equals n)
  */
void uniqueIAN2(int* input, int n, int** collapsed, int* n_collapsed, int** expand_map, int* n_expand_map);

/**
  * returns the mode value of a real array
  * @param input input array
  * @param length of input array
  * @returns mode value
  */
real modeRMA(real* input, int n);

/**
  * returns the number of occurances of the mode value of a real array
  * @param input input array
  * @param length of input array
  * @returns the number of occurances of the mode value
  */
int modeRFA(real* input, int n);

/**
  * finds those indeces of array input, that address elements which are equal to v
  * @param input input array of integers
  * @param n length of the input array
  * @param v value to check equality with
  * @param f output array - the matching indeces get in this array
  * @param n_f output variable - pointer to an integer variable, that will contain the length of f
  */
void findEqualsIA(int* input, int n, int v, int* f, int* n_f);

/**
  * finds the maximum element in t, puts it in m and puts the indeces of each occurances into fk
  * @param t input array
  * @param n_t length of input array
  * @param m output variable - the maximum element will be put in this address
  * @param fk output array - the indeces of occurances of the maximum element will be put in this array
  * @param n_fk output variable - the length of fk
  */
void maxIA2(int* t, int n_t, int* m, int* fk, int* n_fk);

/**
  * finds the maximum element in t, puts it in m and puts the indeces of each occurances into fk
  * @param t input array
  * @param n_t length of input array
  * @param m output variable - the maximum element will be put in this address
  * @param fk output array - the indeces of occurances of the maximum element will be put in this array
  * @param n_fk output variable - the length of fk
  */
void minRA2(real* t, int n_t, real* m, int* fk, int* n_fk);

/**
  * set difference of integer arrays
  * @param a set A
  * @param n_a length of set A
  * @param b set B
  * @param n_b length of set B
  * @param c output parameter - A/B
  * @param n_c output parameter - length of A/B
  */
void setdiffIA(int* a, int n_a, int* b, int n_b, int* c, int* n_c);

#endif
