#ifndef _PRINT_OUT_H_
#define _PRINT_OUT_H_

#include "openbest-ds/config-ds.h"

int tprintf(const char* fmt, ...);

int eprintf(const char* fmt, ...);

int wprintf(const char* fmt, ...);

void newline();

void enewline();

void dea();

void deb();

void dec();

void ded();

void dee();

void def();

void deg();

void deh();

void dei();

void dej();

void dek();

void printArrayI(char* name, int* a, int n);

void printArray2I(char* name, int* a, int n1, int n2);

void printArrayB(char* name, bool* a, int n);

void printArray2B(char* name, bool* a, int n1, int n2);

void printArrayFloat(char* name, float* a, int n);

void printArray2Float(char* name, float* a, int n1, int n2);

void printArrayD(char* name, double* a, int n);

void printArray2D(char* name, double* a, int n1, int n2);

void printArrayR(char* name, real* a, int n);

void printArray2R(char* name, real* a, int n1, int n2);

void printArrayIFile(char* name, int* a, int n);

void printArray2IFile(char* name, int* a, int n1, int n2);

void printArrayBFile(char* name, bool* a, int n);

void printArray2BFile(char* name, bool* a, int n1, int n2);

void printArrayFloatFile(char* name, float* a, int n);

void printArray2FloatFile(char* name, float* a, int n1, int n2);

void printArrayDFile(char* name, double* a, int n);

void printArray2DFile(char* name, double* a, int n1, int n2);

void printArrayRFile(char* name, real* a, int n);

void printArray2RFile(char* name, real* a, int n1, int n2);


#endif
