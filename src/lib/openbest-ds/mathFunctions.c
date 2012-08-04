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

flag_t minF(flag_t* x, int n)
{
  int t= 255;
  int i;
  for ( i= 0; i < n; ++i )
    if ( x[i] < t )
      t= x[i];
  return t;
}

flag_t maxF(flag_t* x, int n)
{
  int t= 0;
  int i;
  for ( i= 0; i < n; ++i )
    if ( x[i] > t )
      t= x[i];
  return t;
}

char minC(char* x, int n)
{
  int t= 255;
  int i;
  for ( i= 0; i < n; ++i )
    if ( x[i] < t )
      t= x[i];
  return t;
}

char maxC(char* x, int n)
{
  int t= 0;
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

flag_t* ismemberFA(flag_t* a, int n, flag_t* b, int m)
{
    flag_t* tmp= fnalloc(n);
    qsortFA(b, m);
    int i, j;
    for ( i= 0; i < n; ++i )
        for ( j= 0; j < m && b[j] < a[i]; ++j )
        {
            if ( a[i] == b[j] )
            {
                tmp[i]= true;
                break;
            }
        }
    return tmp;
}

int isemptyF(flag_t* a, int n)
{
    int i;
    for ( i= 0; i < n; ++i )
        if ( a[i] )
            return 0;
    return 1;
}

int isemptyI(int* a, int n)
{
    int i;
    for ( i= 0; i < n; ++i )
        if ( a[i] )
            return 0;
    return 1;
}

int anyI(int* a, int n)
{
    int i;
    for ( i= 0; i < n; ++i )
        if ( a[i] )
            return 1;
    return 0;
}

int anyF(flag_t* f, int n)
{
    int i;
    for ( i= 0; i < n; ++i )
        if ( f[i] )
            return 1;
    return 0;
}

int anyB(bool* b, int n)
{
    int i;
    for ( i= 0; i < n; ++i )
        if ( b[i] )
            return 1;
    return 0;
}

real stdR(real* a, int n)
{
    if ( n == 0 )
        return 0;
    real e= 0;
    real e2= 0;
    int i;
    for ( i= 0; i < n; ++i )
    {
        e+= a[i];
        e2+= a[i]*a[i];
    }
    e/= n;
    e2/= n;
    return sqrt(e2 - e*e);
}

temp_t stdT(temp_t* a, int n)
{
    if ( n == 0 )
        return 0;
    temp_t e= 0;
    temp_t e2= 0;
    int i;
    for ( i= 0; i < n; ++i )
    {
        e+= a[i];
        e2+= a[i]*a[i];
    }
    e/= n;
    e2/= n;
    return sqrt(e2 - e*e);
}

real sign(real x)
{
    if ( x < 0.0 )
        return -1.0;
    return 1.0;
}

/*int round(real x )
{
    return ((x - floor(x)) >= 0.5) ? ceil (x) : floor(x);
}*/

real polyval(real* p, int n_p, real x)
{
    int i;
    real res= 0;
    for ( i= 0; i < n_p; ++i )
        res+= p[i]*pow(x, n_p - i - 1);
    return res;
}
