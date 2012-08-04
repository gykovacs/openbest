#include <stdlib.h>

#include "openbest-ds/config-ds.h"
#include "openbest-ds/memFunctions.h"

real* rnalloc(int n)
{
  return (real*)malloc(sizeof(real)*n);
}

char* cnalloc(int n)
{
  return (char*)malloc(sizeof(char)*n);
}

flag_t* fnalloc(int n)
{
    return (flag_t*)malloc(sizeof(flag_t)*n);
}

int* inalloc(int n)
{
    return (int*)malloc(sizeof(int)*n);
}

short* snalloc(int n)
{
    return (short*)malloc(sizeof(short)*n);
}

temp_t* tnalloc(int n)
{
    return (temp_t*)malloc(sizeof(temp_t)*n);
}

bool* bnalloc(int n)
{
    return (bool*)malloc(sizeof(bool)*n);
}

double* dnalloc(int n)
{
    return (double*)malloc(sizeof(double)*n);
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

void sets(short* p, int n, short v)
{
  int i;
  for ( i= 0; i < n; ++i )
    p[i]= v;
}

void seti(int* p, int n, int v)
{
  int i;
  for ( i= 0; i < n; ++i )
    p[i]= v;
}

void setf(flag_t* p, int n, flag_t v)
{
    int i;
    for ( i= 0; i < n; ++i )
        p[i]= v;
}

void setb(bool* p, int n, bool v)
{
    int i;
    for ( i= 0; i < n; ++i )
        p[i]= v;
}

real* copyRA(real* input, int n)
{
    if ( n > 0 )
    {
        real* tmp= (real*)malloc(sizeof(real)*n);
        memcpy(tmp, input, n*sizeof(real));
        return tmp;
    }
    return NULL;
}

int* copyIA(int* input, int n)
{
    if ( n > 0 )
    {
        int* tmp= (int*)malloc(sizeof(int)*n);
        memcpy(tmp, input, n*sizeof(int));
        return tmp;
    }
    return NULL;
}

flag_t* copyFA(flag_t* input, int n)
{
    if ( n > 0 )
    {
        flag_t* tmp= (flag_t*)malloc(sizeof(flag_t)*n);
        memcpy(tmp, input, n*sizeof(flag_t));
        return tmp;
    }
    return NULL;
}

short* copySA(short* input, int n)
{
    if ( n > 0 )
    {
        short* tmp= (short*)malloc(sizeof(short)*n);
        memcpy(tmp, input, n*sizeof(short));
        return tmp;
    }
    return NULL;
}

char* copyCA(char* input, int n)
{
    if ( n > 0 )
    {
        char* tmp= (char*)malloc(sizeof(char)*n);
        memcpy(tmp, input, n*sizeof(char));
        return tmp;
    }
    return NULL;
}

temp_t* copyTA(temp_t* input, int n)
{
    if ( n > 0 )
    {
        temp_t* tmp= (temp_t*)malloc(sizeof(temp_t)*n);
        memcpy(tmp, input, n*sizeof(temp_t));
        return tmp;
    }
    return NULL;
}
