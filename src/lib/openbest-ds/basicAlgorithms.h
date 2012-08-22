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

/**
  * creates a new float array by projecting the rows and columns of the input array along the indexes in the sets i1 and i2
  * @param a input array
  * @param n1 rows of the input array
  * @param n2 columns of the input array
  * @param i1 index array for rows
  * @param in1 size of index array for rows
  * @param i2 index array for columns
  * @param in2 size of index array for columns
  * @param b pointer for new array
  * @param bn1 pointer to the number of rows in the new array
  * @param bn2 pointer to the number of columns of the new array
  */
void createSubArrayIndex2Float(float* a, int n1, int n2, int* i1, int in1, int* i2, int in2, float** b, int* bn1, int* bn2);

/**
  * creates a new float array by projecting the rows and columns of the input array using the logical values in i1 and i2
  * @param a input array
  * @param n1 rows of the input array
  * @param n2 columns of the input array
  * @param i1 logical array for rows
  * @param in1 size of logical array for rows
  * @param i2 logical array for columns
  * @param in2 size of logical array for columns
  * @param b pointer for new array
  * @param bn1 pointer to the number of rows in the new array
  * @param bn2 pointer to the number of columns of the new array
  */
void createSubArrayMask2Float(float* a, int n1, int n2, int* i1, int in1, int* i2, int in2, float** b, int* bn1, int* bn2);

/**
  * creates a new integer array from array a by selecting the elements indexed by the indices in idx
  * @param a input array
  * @param n size of the input array
  * @param idx index array
  * @param n_idx size of the index array
  * @param b pointer to output array
  * @param bn pointer to the number of elements in the output array
  */
void createSubArrayIndexI(int* a, int n, int* idx, int n_idx, int** b, int* bn);

/**
  * creates a new integer array from array a by selecting the elements using the logical array idx
  * @param a input array
  * @param n size of the input array
  * @param idx logical array
  * @param n_idx size of the idx array
  * @param b pointer to output array
  * @param bn pointer to the number of elements in the output array
  */
void createSubArrayMaskI(int* a, int n, int* idx, int n_idx, int** b, int* bn);

/**
  * row-columns matrix multiplication for float
  * @param a left side matrix
  * @param na1 number of rows of a
  * @param na2 number of columns of a
  * @param b right side matrix
  * @param nb1 number of rows of b
  * @param nb2 number of columns of b
  * @param c pointer to output array (matrix)
  * @param nc1 pointer to rows of c
  * @param nc2 pointer to columns of c
  */
void matrixMultiplicationNF(float* a, int na1, int na2,
                            float* b, int nb1, int nb2,
                            float** c, int* nc1, int* nc2);

/**
  * row-columns matrix multiplication for double
  * @param a left side matrix
  * @param na1 number of rows of a
  * @param na2 number of columns of a
  * @param b right side matrix
  * @param nb1 number of rows of b
  * @param nb2 number of columns of b
  * @param c pointer to output array (matrix)
  * @param nc1 pointer to rows of c
  * @param nc2 pointer to columns of c
  */
void matrixMultiplicationND(double* a, int na1, int na2,
                            double* b, int nb1, int nb2,
                            double** c, int* nc1, int* nc2);

/**
  * transposes a double matrix and returns the pointer of the new matrix
  * @param a input matrix
  * @param n1 number of rows of a
  * @param n2 number of columns of a
  * @returns pointer to the transposed matrix
  */
double* transposeMatrixND(double* a, int n1, int n2);

/**
  * subtracts the double matrix a from b and returns the result in c
  * @param a left side matrix
  * @param na1 number of rows of a
  * @param na2 number of columns of a
  * @param b right side matrix
  * @param nb1 number of rows of b
  * @param nb2 number of columns of b
  * @param c pointer to output array (matrix)
  * @param nc1 pointer to rows of c
  * @param nc2 pointer to columns of c
  */
void matrixSubtractND(double* a, int na1, int na2,
                      double* b, int nb1, int nb2,
                      double** c, int* nc1, int* nc2);

/**
  * subtracts the vector b from each row of the matrix a, the result is returned in c
  * @param a left side matrix
  * @param na1 number of rows of a
  * @param na2 number of columns of a
  * @param b right side matrix
  * @param nb1 number of rows of b
  * @param nb2 number of columns of b
  * @param c pointer to output array (matrix)
  * @param nc1 pointer to rows of c
  * @param nc2 pointer to columns of c
  */
void matrixSubtractVectorND(double* a, int na1, int na2,
                            double* b, int nb1, int nb2,
                            double** c, int* nc1, int* nc2);

/**
  * matrix multiplication by vector from the right side
  * @param a left side matrix
  * @param na1 number of rows of a
  * @param na2 number of columns of a
  * @param b right side vector
  * @param nb number of elements in b
  * @param c pointer to output array (vector)
  * @param nc pointer to the number of elements of c
  */
void matrixMultiplicationByVector(double* a, int na1, int na2,
                                  double* b, int nb,
                                  double** c, int* nc);

/**
  * matrix multiplication by vector from the left side
  * @param a left side vector
  * @param na number of elements of a
  * @param b right side matrix
  * @param nb1 number of rows in b
  * @param nb2 number of columns in b
  * @param c pointer to output array (vector)
  * @param nc pointer to the number of elements of c
  */
void vectorMultiplicationByMatrix(double* a, int na,
                                  double* b, int nb1, int nb2,
                                  double**c, int* nc);

/**
  * transposes a float matrix to a new, double matrix
  * @param a input matrix
  * @param n1 number of rows of a
  * @param n2 number of columns of a
  * @returns pointer to the transposed matrix
  */
double* transposeMatrixNFloatD(float* a, int n1, int n2);

/**
  * transposes a float matrix to a new, float matrix
  * @param a input matrix
  * @param n1 number of rows of a
  * @param n2 number of columns of a
  * @return pointer to the transposed matrix
  */
float* transposeMatrixNFloat(float* a, int n1, int n2);

#endif
