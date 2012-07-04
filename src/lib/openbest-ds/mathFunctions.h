#ifndef _MATH_FUNCTIONS_H_
#define _MATH_FUNCTIONS_H_

#include <stdio.h>

#include "openbest-ds/config.h"

int sgn(real v);

int isNaN(real* x, int n);

void isNaNV(real* x, int n, char* flags);

real min(real* x, int n);

real max(real* x, int n);

real mod1(real x, real y);

real* modA(real* x, real* y, int n);

real* rnalloc(int n);
char* cnalloc(int n);

void set(real* p, int n, real v);

void setc(char* p, int n, char v);

void add(real* a, real* b, int n, real* res);

void subtract(real* a, real* b, int n, real* res);

void multiply(real* a, real* b, int n, real* res);

void divide(real* a, real* b, int n, real* res);

void unionV(char* a, int an, char* b, int bn, char* c, int* cn);

void lessthenr(real* input, int n, real a, char* flags);

void equalsr(real* input, int n, real a, char* flags);

#endif