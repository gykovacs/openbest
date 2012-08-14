#ifndef _MEM_FUNCTIONS_H_
#define _MEM_FUNCTIONS_H_

#include "openbest-ds/config-ds.h"

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
  * allocates memory for n flag_t variables
  * @param n number of flag_t variables to allocate space for
  * @returns pointer to the allocated area
  */
flag_t* fnalloc(int n);

/**
  * allocates memory for n int variables
  * @param n number of flag_t variables to allocate space for
  * @returns pointer to the allocated area
  */
int* inalloc(int n);

/**
  * allocates memory for n short variables
  * @param n number of flag_t variables to allocate space for
  * @returns pointer to the allocated area
  */
short* snalloc(int n);

/**
  * allocates memory for n temp_t variables
  * @param n number of flag_t variables to allocate space for
  * @returns pointer to the allocated area
  */
temp_t* tnalloc(int n);

/**
  * allocates memory for n bool variables
  * @param n number of flag_t variables to allocate space for
  * @returns pointer to the allocated area
  */
bool* bnalloc(int n);

double* dnalloc(int n);

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
  * sets the elements of the parameter array to a specific value
  * @param p input array
  * @param n length of the input array
  * @param v value to set the elements of the array to
  */
void setb(bool* p, int n, bool v);

/**
  * sets the elements of the parameter array to a specific value
  * @param p input array
  * @param n length of the input array
  * @param v value to set the elements of the array to
  */
void seti(int* p, int n, int v);

/**
  * sets the elements of the parameter array to a specific value
  * @param p input array
  * @param n length of the input array
  * @param v value to set the elements of the array to
  */
void sets(short* p, int n, short v);

/**
  * sets the elements of the parameter array to a specific value
  * @param p input array
  * @param n length of the input array
  * @param v value to set the elements of the array to
  */
void setf(flag_t* p, int n, flag_t v);

/**
  * sets the elements of the parameter array to a specific value
  * @param p input array
  * @param n length of the input array
  * @param v value to set the elements of the array to
  */
void setd(double* p, int n, double v);

/**
  * returns a newly allocated area with the copied contents of the parameter array
  * @param input input array to copy
  * @param n length of the input array
  * @returns pointer of the newly allocated and copied array
  */
real* copyRA(real* input, int n);

/**
  * returns a newly allocated area with the copied contents of the parameter array
  * @param input input array to copy
  * @param n length of the input array
  * @returns pointer of the newly allocated and copied array
  */
int* copyIA(int* input, int n);

/**
  * returns a newly allocated area with the copied contents of the parameter array
  * @param input input array to copy
  * @param n length of the input array
  * @returns pointer of the newly allocated and copied array
  */
flag_t* copyFA(flag_t* input, int n);

/**
  * returns a newly allocated area with the copied contents of the parameter array
  * @param input input array to copy
  * @param n length of the input array
  * @returns pointer of the newly allocated and copied array
  */
short* copySA(short* input, int n);

/**
  * returns a newly allocated area with the copied contents of the parameter array
  * @param input input array to copy
  * @param n length of the input array
  * @returns pointer of the newly allocated and copied array
  */
char* copyCA(char* input, int n);

/**
  * returns a newly allocated area with the copied contents of the parameter array
  * @param input input array to copy
  * @param n length of the input array
  * @returns pointer of the newly allocated and copied array
  */
temp_t* copyTA(temp_t* input, int n);

double* copyDA(double* input, int n);


#endif
