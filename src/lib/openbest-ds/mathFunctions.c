#include <math.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>

#include "openbest-ds/mathFunctions.h"
#include "openbest-ds/printOut.h"

int sgn(real v)
{
  return (0 < v) - (v < 0);
}

real mod1(real x, real y)
{
  if ( y != 0 )
    return x - y * floor(x/y);
  return x;
}

real* modA(real* x, real* y, int n)
{
  int i;
  real* res= (real*)malloc(sizeof(real)*n);
  if ( !res )
    eprintf("malloc failed in modA");
  
  for ( i= 0; i < n; ++i )
    res[i]= mod1(x[i], y[i]);
  
  return res;
}

real minR(real* x, int n)
{
  real t= FLT_MAX;
  int i;
  for ( i= 0; i < n; ++i )
    if ( x[i] < t )
      t= x[i];
  return t;
}

real maxR(real* x, int n)
{
  real t= -FLT_MAX;
  int i;
  for ( i= 0; i < n; ++i )
    if ( x[i] > t )
      t= x[i];
  return t;
}

int minI(int* x, int n)
{
  int t= INT_MAX;
  int i;
  for ( i= 0; i < n; ++i )
    if ( x[i] < t )
      t= x[i];
  return t;
}

int maxI(int* x, int n)
{
  int t= -INT_MAX;
  int i;
  for ( i= 0; i < n; ++i )
    if ( x[i] > t )
      t= x[i];
  return t;
}


int isNaN(real* x, int n)
{
  int i;
  for ( i= 0; i < n; ++i )
    if ( x[i] != x[i] || x[i] == FLT_MAX )
      return 1;
  return 0;
}

void isNaNV(real* x, int n, char* flags)
{
  int i;
  for ( i= 0; i < n; ++i )
    if ( x[i] != x[i] || x[i] == FLT_MAX )
      flags[i]= 1;
    else
      flags[i]= 0;
}

real* rnalloc(int n)
{
  return (real*)malloc(sizeof(real)*n);
}

char* cnalloc(int n)
{
  return (char*)malloc(sizeof(char)*n);
}

void set(real* p, int n, real v)
{
  int i;
  for ( i= 0; i < n; ++i )
    p[i]= v;
}

void setc(char* p, int n, char v)
{
  int i;
  for ( i= 0; i < n; ++i )
    p[i]= v;
}

void add(real* a, real* b, int n, real* res)
{
  --n;
  for ( ; n >= 0; --n )
    res[n]= a[n] + b[n];
}

void subtract(real* a, real* b, int n, real* res)
{
  --n;
  for ( ; n >= 0; --n )
    res[n]= a[n] - b[n];
}

void multiply(real* a, real* b, int n, real* res)
{
  --n;
  for ( ; n >= 0; --n )
    res[n]= a[n] * b[n];
}

void divide(real* a, real* b, int n, real* res)
{
  --n;
  for ( ; n >= 0; --n )
    res[n]= a[n] / b[n];
}

void unionV(char* a, int an, char* b, int bn, char* c, int* cn)
{
  int i, j;
  *cn= 0;
  for ( i= 0; i < an; ++i )
  {
    c[*cn]= a[i];
    (*cn)++;
  }
  for ( i= 0; i < bn; ++i )
  {
    for ( j= 0; j < an; ++j )
      if ( b[i] == a[j] )
	break;
    if ( j == an )
    {
      c[*cn]= b[i];
      ++*cn;
    }
  }
}

void lessthenr(real* input, int n, real a, char* flags)
{
  int i;
  for ( i= 0; i < n; ++i )
    if ( input[i] < a )
      flags[i]= 1;
    else
      flags[i]= 0;
}

void equalsr(real* input, int n, real a, char* flags)
{
  int i;
  for ( i= 0; i < n; ++i )
    if ( input[i] == a )
      flags[i]= 1;
    else
      flags[i]= 0;
}

void diffR(real* input, int n, real* output)
{
    if ( n <= 1 )
        return;

    int i;
    for ( i= 0; i < n-1; ++i )
        output[i]= input[i+1] - input[i];
}

real* diffRN(real* input, int n)
{
    if ( n <= 1 )
        return NULL;

    real* output= rnalloc(n-1);

    int i;
    for ( i= 0; i < n-1; ++i )
        output[i]= input[i+1] - input[i];

    return output;
}

real* copyRA(real* input, int n)
{
    real* tmp= (real*)malloc(sizeof(real)*n);
    memcpy(tmp, input, n*sizeof(real));
    return tmp;
}

int* copyIA(int* input, int n)
{
    int* tmp= (int*)malloc(sizeof(int)*n);
    memcpy(tmp, input, n*sizeof(int));
    return tmp;
}

flag_t* copyFA(flag_t* input, int n)
{
    flag_t* tmp= (flag_t*)malloc(sizeof(flag_t)*n);
    memcpy(tmp, input, n*sizeof(flag_t));
    return tmp;
}

short* copySA(short* input, int n)
{
    short* tmp= (short*)malloc(sizeof(short)*n);
    memcpy(tmp, input, n*sizeof(short));
    return tmp;
}

char* copyCA(char* input, int n)
{
    char* tmp= (char*)malloc(sizeof(char)*n);
    memcpy(tmp, input, n*sizeof(char));
    return tmp;
}

temp_t* copyTA(temp_t* input, int n)
{
    temp_t* tmp= (temp_t*)malloc(sizeof(temp_t)*n);
    memcpy(tmp, input, n*sizeof(temp_t));
    return tmp;
}
