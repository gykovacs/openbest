#ifndef _PRINT_OUT_H_
#define _PRINT_OUT_H_

#include "openbest-ds/config-ds.h"

/**
  * printf with timestamp
  * @param fmt format
  * @returns number of printed characters
  */
int tprintf(const char* fmt, ...);

/**
  * printf error message
  * @param fmt format
  * @returns number of printed characters
  */
int eprintf(const char* fmt, ...);

/**
  * printf warning message
  * @param fmt format
  * @returns number of printed characters
  */
int wprintf(const char* fmt, ...);

/**
  * print new line to stdout
  */
void newline();

/**
  * print new line to stderr
  */
void enewline();

/** used for debugging function*/
void dea();

/** used for debugging function*/
void deb();

/** used for debugging function*/
void dec();

/** used for debugging function*/
void ded();

/** used for debugging function*/
void dee();

/** used for debugging function*/
void def();

/** used for debugging function*/
void deg();

/** used for debugging function*/
void deh();

/** used for debugging function*/
void dei();

/** used for debugging function*/
void dej();

/** used for debugging function*/
void dek();

/**
  * prints the contents of the array a stdout
  * @param name name of the array
  * @param a array
  * @param n length of a
  */
void printArrayI(char* name, int* a, int n);

/**
  * prints the contents of the 2D array a stdout
  * @param name name of the array
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2I(char* name, int* a, int n1, int n2);

/**
  * prints the contents of the array a stdout
  * @param name name of the array
  * @param a array
  * @param n length of a
  */
void printArrayB(char* name, bool* a, int n);

/**
  * prints the contents of the 2D array a stdout
  * @param name name of the array
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2B(char* name, bool* a, int n1, int n2);

/**
  * prints the contents of the array a stdout
  * @param name name of the array
  * @param a array
  * @param n length of a
  */
void printArrayFloat(char* name, float* a, int n);

/**
  * prints the contents of the 2D array a stdout
  * @param name name of the array
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2Float(char* name, float* a, int n1, int n2);

/**
  * prints the contents of the array a stdout
  * @param name name of the array
  * @param a array
  * @param n length of a
  */
void printArrayD(char* name, double* a, int n);

/**
  * prints the contents of the 2D array a stdout
  * @param name name of the array
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2D(char* name, double* a, int n1, int n2);

/**
  * prints the contents of the array a stdout
  * @param name name of the array
  * @param a array
  * @param n length of a
  */
void printArrayR(char* name, real* a, int n);

/**
  * prints the contents of the 2D array a stdout
  * @param name name of the array
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2R(char* name, real* a, int n1, int n2);

/**
  * prints the contents of the array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n length of a
  */
void printArrayIFile(char* name, int* a, int n);

/**
  * prints the contents of the 2D array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2IFile(char* name, int* a, int n1, int n2);

/**
  * prints the contents of the array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n length of a
  */
void printArrayBFile(char* name, bool* a, int n);

/**
  * prints the contents of the 2D array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2BFile(char* name, bool* a, int n1, int n2);

/**
  * prints the contents of the array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n length of a
  */
void printArrayFloatFile(char* name, float* a, int n);

/**
  * prints the contents of the 2D array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2FloatFile(char* name, float* a, int n1, int n2);

/**
  * prints the contents of the array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n length of a
  */
void printArrayDFile(char* name, double* a, int n);

/**
  * prints the contents of the 2D array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2DFile(char* name, double* a, int n1, int n2);

/**
  * prints the contents of the array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n length of a
  */
void printArrayRFile(char* name, real* a, int n);

/**
  * prints the contents of the 2D array a file called "name"
  * @param name name of the file
  * @param a array
  * @param n1 rows of a
  * @param n2 columns of a
  */
void printArray2RFile(char* name, real* a, int n1, int n2);


#endif
