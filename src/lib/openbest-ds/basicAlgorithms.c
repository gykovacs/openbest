#include <stdlib.h>

#include "openbest-ds/basicAlgorithms.h"

void swap(real *x,real *y)
{
   real temp;
   temp = *x;
   *x = *y;
   *y = temp;
}

int choose_pivot(int i,int j )
{
   return((i+j) /2);
}

void quicksort(real* list,int m,int n)
{
   int i,j,k;
   real key;
   if( m < n)
   {
      k = choose_pivot(m,n);
      swap(&list[m],&list[k]);
      key = list[m];
      i = m+1;
      j = n;
      while(i <= j)
      {
         while((i <= n) && (list[i] <= key))
                i++;
         while((j >= m) && (list[j] > key))
                j--;
         if( i < j)
                swap(&list[i],&list[j]);
      }
	  // swap two elements
      swap(&list[m],&list[j]);
	  // recursively sort the lesser list
      quicksort(list,m,j-1);
      quicksort(list,j+1,n);
   }
}

int compareIA(const void *x, const void *y)
{
    return *(int*)x - *(int*)y;
}

void qsortIA(int* t, int n)
{
    qsort(t, n, sizeof(int), compareIA);
}

int compareFA(const void *x, const void *y)
{
    return *(flag_t*)x - *(flag_t*)y;
}

void qsortFA(flag_t* t, int n)
{
    qsort(t, n, sizeof(int), compareFA);
}

int compareRA(const void *x, const void *y)
{
    return *(real*)x - *(real*)y;
}

void qsortRA(real* t, int n)
{
    qsort(t, n, sizeof(real), compareRA);
}

void uniqueIA(int** t1, int* n1)
{
    int* t= *t1;
    int n= *n1;
    qsortIA(t, n);
    int i, j, d;
    d= 1;
    j= 1;
    for ( i= 1; i < n; ++i )
        if ( t[i-1] != t[i] )
        {
            ++d;
            t[j++]= t[i];
        }
    *t1= (int*)realloc(*t1, sizeof(int)*d);
    *n1= d;
}

void uniqueIAN(int* t, int n, int** output, int* m)
{
    int k;
    printf("%d: ", n);
    for ( k= 0; k < n; ++k )
        printf("%d ", t[k]);
    printf("\n");
    *output= (int*)malloc(sizeof(int)*n);
    qsortIA(t, n);
    int i, j, d;
    d= 1;
    j= 1;
    (*output)[0]= t[0];
    for ( i= 1; i < n; ++i )
        if ( t[i-1] != t[i] )
        {
            ++d;
            (*output)[j++]= t[i];
        }
    *output= (int*)realloc(*output, sizeof(int)*d);
    *m= d;
}

void uniqueRA(real** t1, int* n1)
{
    real* t= *t1;
    int n= *n1;
    qsortRA(t, n);
    int i, j, d;
    d= 1;
    j= 1;
    for ( i= 1; i < n; ++i )
        if ( t[i-1] != t[i] )
        {
            ++d;
            t[j++]= t[i];
        }
    *t1= (real*)realloc(*t1, sizeof(real)*d);
    *n1= d;
}

void uniqueRAN(real* t, int n, real** output, int* m)
{
    *output= (real*)malloc(sizeof(real)*n);
    qsortRA(t, n);
    int i, j, d;
    d= 1;
    j= 1;
    for ( i= 1; i < n; ++i )
        if ( t[i-1] != t[i] )
        {
            ++d;
            (*output)[j++]= t[i];
        }
    *output= (real*)realloc(*output, sizeof(real)*d);
    *m= d;
}

real modeRMA(real* input, int n)
{
    real* tmp= copyRA(input, n);
    qsortRA(tmp, n);

    int maxL, i, j;
    real maxV1;
    maxL= 0;
    maxV1= input[0];
    j= 1;
    for ( i= 1; i < n; ++i )
        if ( i < n && tmp[i] == tmp[i-1] )
            ++j;
        else if ( i == n || tmp[i] != tmp[i-1] )
        {
            if ( j > maxL )
            {
                maxL= j;
                maxV1= tmp[i-1];
            }
            j= 1;
        }
    free(tmp);
    return maxV1;
}

int modeRFA(real* input, int n)
{
    real* tmp= copyRA(input, n);
    qsortRA(tmp, n);

    int maxL, i, j;
    real maxV;
    maxL= 0;
    maxV= input[0];
    j= 1;
    for ( i= 1; i < n; ++i )
        if ( tmp[i] == tmp[i-1] )
            ++j;
        else
        {
            if ( j > maxL )
            {
                maxL= j;
                maxV= tmp[i-1];
            }
            j= 1;
        }
    free(tmp);
    return maxL;
}

void findEqualsIA(int* input, int n, int v, int* f, int* n_f)
{
    int i;
    *n_f= 0;
    for ( i= 0; i < n; ++i )
        if ( input[i] == v )
            f[(*n_f)++]= i;
}
