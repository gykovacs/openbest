#ifndef _MATH_FUNCTIONS_H_
#define _MATH_FUNCTIONS_H_

#include <stdio.h>

#include "openbest-ds/config-ds.h"

/**
  * signum of the parameter
  * @param v real number
  * @returns 1 if the paramter is positive, -1 if the parameter is negative, 0 otherwise
  */
int sgn(real v);

/**
  * returns 1 if any element of the parameter array is FLT_MAX
  * @param x input array of real numbers
  * @param n length of the array x
  * @returns logical true (1) of there is at least 1 FLT_MAX in the array, 0 otherwise
  */
int isNaN(real* x, int n);

/**
  * puts isNaN(x[i]) into flags[i]
  * @param x array of real numbers
  * @param n length of the array
  * @param flags output array
  */
void isNaNV(real* x, int n, char* flags);

/**
  * returns the minimum value in the parameter array
  * @param x array of real numbers
  * @param n length of the input array
  * @returns the minimum value in the array
  */
real minR(real* x, int n);

/**
  * returns the maximum value in the parameter array
  * @param x array of real numbers
  * @param n length of the input array
  * @returns the maximum value in the array
  */
real maxR(real* x, int n);

/**
  * returns the minimum value in the parameter array
  * @param x array of real numbers
  * @param n length of the input array
  * @returns the minimum value in the array
  */
int minI(int* x, int n);

/**
  * returns the maximum value in the parameter array
  * @param x array of real numbers
  * @param n length of the input array
  * @returns the maximum value in the array
  */
int maxI(int* x, int n);

/**
  * modulo division
  * @param x real number to divide
  * @param y real number to divide x with
  * @returns modulo value
  */
real mod1(real x, real y);

/**
  * applies mod1 for each element of the arrays x and y and creates and returns the pointer a new array with the results
  * @param x array of real numbers to divide
  * @param y array of real numbers to divide with
  * @param n length of arrays x and y
  * @returns resulting array
  */
real* modA(real* x, real* y, int n);

/**
  * allocates memory for n real numbers
  * @param n number of real numbers to allocate space for
  * @returns pointer to the allocated area
  */
real* rnalloc(int n);

/**
  * allocates memory for n char variables
  * @param n number of char variables to allocate space for
  * @returns pointer to the allocated area
  */
char* cnalloc(int n);

/**
  * sets the elements of the parameter array to a specific value
  * @param p input array
  * @param n length of the input array
  * @param v value to set the elements of the array to
  */
void set(real* p, int n, real v);

/**
  * sets the elements of the parameter array to a specific value
  * @param p input array
  * @param n length of the input array
  * @param v value to set the elements of the array to
  */
void setc(char* p, int n, char v);

/**
  * adds the elements of two arrays element-wise and puts into the output parameter array
  * @param a array to add
  * @param b array to add
  * @param n length of arrays a and b
  * @param res preallocated space for the result
  */
void add(real* a, real* b, int n, real* res);

/**
  * subtracts the elements of two arrays element-wise and puts into the output parameter array
  * @param a array to subtract from
  * @param b array to subtract
  * @param n length of arrays a and b
  * @param res preallocated space for the result
  */
void subtract(real* a, real* b, int n, real* res);

/**
  * multiplies the elements of two arrays element-wise and puts into the output parameter array
  * @param a array to multiply
  * @param b array to multiply with
  * @param n length of arrays a and b
  * @param res preallocated space for the result
  */
void multiply(real* a, real* b, int n, real* res);

/**
  * divides the elements of two arrays element-wise and puts into the output parameter array
  * @param a array to divide
  * @param b array to divide with
  * @param n length of arrays a and b
  * @param res preallocated space for the result
  */
void divide(real* a, real* b, int n, real* res);

/**
  * computes the union of two char vectors into a third vector
  * @param a array to compute union with array b
  * @param an array of array a
  * @param b array to compute union with array a
  * @param bn length of array b
  * @param preallocated space for the result
  * @param cn output parameter with the length of the output array
  */
void unionV(char* a, int an, char* b, int bn, char* c, int* cn);

/**
  * checks whether the elements of the input array are less then a specified value and puts the resulting flags into an output array
  * @param input input array
  * @param n length of the input array
  * @param a the value to compare to
  * @param flags output array
  */
void lessthenr(real* input, int n, real a, char* flags);

/**
  * checks whether the elements of the input array are equal with a specified value and puts the resulting flags into an output array
  * @param input input array
  * @param n length of the input array
  * @param a the value to compare to
  * @param flags output array
  */
void equalsr(real* input, int n, real a, char* flags);

/**
  * computes the differences of consecutive elements of the input array into array output
  * @param input input array
  * @param n length of input array
  * @param output output array
  */
void diffR(real* input, int n, real* output);

/**
  * computes the differences of consecutive elements of the input array into a newly allocated array of size n-1
  * @param input input array
  * @param n length of input array
  * @returns output array
  */
real* diffRN(real* input, int n);

real* copyRA(real* input, int n);

int* copyIA(int* input, int n);

flag_t* copyFA(flag_t* input, int n);

short* copySA(short* input, int n);

char* copyCA(char* input, int n);

temp_t* copyTA(temp_t* input, int n);

#endif
