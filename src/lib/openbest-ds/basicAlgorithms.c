#include <stdlib.h>

#include "openbest-ds/memFunctions.h"
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

int compareORIA(const void* x, const void* y)
{
    return outer_array[*(int*)x] > outer_array[*(int*)y];
}

int compareOTIA(const void* x, const void* y)
{
    return outer_arrayT[*(int*)x] > outer_arrayT[*(int*)y];
}

int compareOIIA(const void* x, const void* y)
{
    return outer_arrayI[*(int*)x] > outer_arrayI[*(int*)y];
}

void qsortIA(int* t, int n)
{
    qsort(t, n, sizeof(int), compareIA);
}

void qsortORIA(int* t, int n)
{
    qsort(t, n, sizeof(int), compareORIA);
}

void qsortOTIA(int* t, int n)
{
    qsort(t, n, sizeof(int), compareOTIA);
}

void qsortOIIA(int* t, int n)
{
    qsort(t, n, sizeof(int), compareOIIA);
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
    return *(real*)x > *(real*)y;
}

void qsortRA(real* t, int n)
{
    qsort(t, n, sizeof(real), compareRA);
}

void uniqueIA1(int* t, int* n1)
{
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
    *n1= d;
}

void uniqueIA(int** t1, int* n1)
{
    if ( *n1 == 0 )
    {
        return;
    }
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
    if ( n == 0 )
    {
        *output= NULL;
        *m= 0;
        return;
    }
    int k;
    /*printf("%d: ", n);
    for ( k= 0; k < n; ++k )
        printf("%d ", t[k]);
    printf("\n");*/
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

void uniqueRA1(real* t, int* n1)
{
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
    *n1= d;
}

void uniqueRA(real** t1, int* n1)
{
    if ( *n1 == 0 )
    {
        return;
    }
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
    if ( t[i-1] == t[i-2] )
    {
        ++d;
        t[j++]= t[i-1];
    }
    *t1= (real*)realloc(*t1, sizeof(real)*d);
    *n1= d;
}

void uniqueRAN(real* t, int n, real** output, int* m)
{
    if ( n == 0 )
    {
        *output= NULL;
        *m= 0;
        return;
    }
    *output= (real*)malloc(sizeof(real)*n);
    memcpy(*output, t, sizeof(real)*n);
    qsortRA(*output, n);
    int i, j, d;
    d= 1;
    j= 1;
    for ( i= 1; i < n; ++i )
        if ( t[i-1] != t[i] )
        {
            ++d;
            (*output)[j++]= t[i];
        }
    if ( t[i-1] == t[i-2] )
    {
        ++d;
        (*output)[j++]= t[i-1];
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
        if ( tmp[i] == tmp[i-1] )
            ++j;
        else if ( tmp[i] != tmp[i-1] )
        {
            if ( j > maxL )
            {
                maxL= j;
                maxV1= tmp[i-1];
            }
            j= 1;
        }
    if ( tmp[i-1] == tmp[i-2] )
        if ( j > maxL )
        {
            maxL= j;
            maxV1= tmp[i-1];
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
    if ( tmp[i-1] == tmp[i-2] )
        if ( j > maxL )
        {
            maxL= j;
            maxV= tmp[i-1];
        }

    free(tmp);
    /*printf("modeRFA %d - \n", n);
    for ( i= 0; i < n; ++i )
        printf("%f ", input[i]);
    printf("\n");*/
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

void uniqueIAN2(int* input, int n, int** collapsed, int* n_collapsed, int** expand_map, int* n_expand_map)
{
    int k;
    *collapsed= (int*)malloc(sizeof(int)*n);
    *expand_map= (int*)malloc(sizeof(int)*n);
    int* t= (int*)malloc(sizeof(int)*n);
    memcpy(t, input, sizeof(int)*n);

    qsortIA(t, n);
    int i, j, d;
    d= 1;
    j= 1;
    (*collapsed)[0]= t[0];
    for ( i= 1; i < n; ++i )
        if ( t[i-1] != t[i] )
        {
            ++d;
            (*collapsed)[j++]= t[i];
        }
    *collapsed= (int*)realloc(*collapsed, sizeof(int)*d);
    *n_collapsed= d;

    for ( i= 0; i < d; ++i )
        for ( j= 0; j < n; ++j )
            if ( (*collapsed)[i] == input[j] )
            {
                (*collapsed)[i]= j;
                break;
            }
    for ( i= 0; i < n; ++i )
        for ( j= 0; j < d; ++j )
            if ( input[i] == input[(*collapsed)[j]] )
                (*expand_map)[i]= j;
    *n_expand_map= n;
}

void maxIA2(int* t, int n_t, int* m, int* fk, int* n_fk)
{
    int m1= t[0];
    int i;
    for ( i= 0; i < n_t; ++i )
        if ( t[i] > m1 )
            m1= t[i];
    *n_fk= 0;
    *m= m1;
    for ( i= 0; i < n_t; ++i )
        if ( t[i] == m1 )
            fk[(*n_fk)++]= i;
}

void minRA2(real* t, int n_t, real* m, int* fk, int* n_fk)
{
    real m1= t[0];
    int i;
    for ( i= 0; i < n_t; ++i )
        if ( t[i] < m1 )
            m1= t[i];
    *n_fk= 0;
    *m= m1;
    for ( i= 0; i < n_t; ++i )
        if ( t[i] == m1 )
            fk[(*n_fk)++]= i;
}

void setdiffIA(int* a, int n_a, int* b, int n_b, int* c, int* n_c)
{
    *n_c= 0;
    int i, j;
    for ( i= 0; i < n_a; ++i )
    {
        for ( j= 0; j < n_b; ++j )
            if ( a[i] == b[j] )
                break;
        if ( j == n_b )
        {
            c[(*n_c)++]= a[i];
        }
    }
}

void createSubArrayIndex2Float(float* a, int n1, int n2, int* i1, int in1, int* i2, int in2, float** b, int* bn1, int* bn2)
{
    float* tmp= (float*)malloc(sizeof(float)*in1*in2);
    int i, j, k, l;
    for ( i= 0; i < in1; ++i )
    {
        for ( j= 0; j < in2; ++j )
            tmp[i*in2 + j]= a[i1[i]*n2 + i2[j]];
    }
    *b= tmp;
    *bn1= in1;
    *bn2= in2;
}

void createSubArrayMask2Float(float* a, int n1, int n2, int* i1, int in1, int* i2, int in2, float** b, int* bn1, int* bn2)
{
    int tmpn1= 0;
    int tmpn2= 0;

    int i, j, k, l;
    for ( i= 0; i < in1; ++i )
        tmpn1+= i1[i];
    for ( i= 0; i < in2; ++i )
        tmpn2+= i2[i];

    float* tmp= (float*)malloc(sizeof(float)*tmpn1*tmpn2);

    for ( i= 0; i < n1; ++i )
    {
        l= 0;
        for ( j= 0; j < n2; ++j )
            if ( i1[i] && i2[j] )
                tmp[k*tmpn2 + l++]= a[i*n2 + j];
        if ( i1[i] )
            ++k;
    }
    *b= tmp;
    *bn1= tmpn1;
    *bn2= tmpn2;
}


void createSubArrayIndexI(int* a, int n, int* idx, int n_idx, int** b, int* bn)
{
    int* tmp= inalloc(n_idx);
    int i;
    for ( i= 0; i < n_idx; ++i )
        tmp[i]= a[idx[i]];
    *b= tmp;
    *bn= n_idx;
}

void createSubArrayMaskI(int* a, int n, int* idx, int n_idx, int** b, int* bn)
{
    int tmpn= 0;
    int i;
    for ( i= 0; i < n_idx; ++i )
        tmpn+= idx[i];

    int* tmp= inalloc(tmpn);
    for ( i= 0; i < n_idx; ++i )
        if ( idx[i] )
            tmp[i++]= a[i];
    *b= tmp;
    *bn= tmpn;
}

void matrixMultiplicationNF(float* a, int na1, int na2, float* b, int nb1, int nb2, float** c, int* nc1, int* nc2)
{
    //tprintf("A: %d %d, b: %d %d\n", na1, na2, nb1, nb2);
    float* x= (float*)malloc(sizeof(float)*na1 * nb2);

    int i, j, k;

    for ( i= 0; i < na1; ++i )
        for ( j= 0; j < nb2; ++j )
        {
            x[i*nb2 + j]= 0;
            for ( k= 0; k < na2; ++k )
                x[i*nb2 + j]+= a[i*na2 + k]*b[k*nb2 + j];
        }
    *c= x;
    *nc1= na1;
    *nc2= nb2;
}

void matrixMultiplicationND(double* a, int na1, int na2, double* b, int nb1, int nb2, double** c, int* nc1, int* nc2)
{
    //tprintf("A: %d %d, b: %d %d\n", na1, na2, nb1, nb2);
    double* x= dnalloc(na1 * nb2);
    if ( x == NULL )
        eprintf("dnalloc failed in matrixMultiplicationND");

    int i, j, k;

    #pragma omp parallel for
    for ( i= 0; i < na1; ++i )
        for ( j= 0; j < nb2; ++j )
        {
            x[i*nb2 + j]= 0;
            for ( k= 0; k < na2; ++k )
            {
                x[i*nb2 + j]+= a[i*na2 + k]*b[k*nb2 + j];
            }
        }

    *c= x;
    *nc1= na1;
    *nc2= nb2;
}


double* transposeMatrixND(double* a, int n1, int n2)
{
    double* tmp= dnalloc(n1*n2);
    int i, j;
    for ( i= 0; i < n1; ++i )
        for ( j= 0; j < n2; ++j )
            tmp[j*n1 + i]= a[i*n2 + j];
    return tmp;
}

double* transposeMatrixNFloatD(float* a, int n1, int n2)
{
    double* tmp= dnalloc((n1*n2));
    int i, j;
    for ( i= 0; i < n1; ++i )
        for ( j= 0; j < n2; ++j )
            tmp[j*n1 + i]= a[i*n2 + j];
    return tmp;
}

float* transposeMatrixNFloat(float* a, int n1, int n2)
{
    float* tmp= (float*)malloc(sizeof(float)*(n1*n2));
    int i, j;
    for ( i= 0; i < n1; ++i )
        for ( j= 0; j < n2; ++j )
            tmp[j*n1 + i]= a[i*n2 + j];
    return tmp;
}


void matrixSubtractND(double* a, int na1, int na2,
                      double* b, int nb1, int nb2,
                      double** c, int* nc1, int* nc2)
{
    double* tmp= dnalloc(na1*na2);
    int i;
    for ( i= 0; i < na1*na2; ++i )
        tmp[i]= a[i] - b[i];
    *c= tmp;
    *nc1= na1;
    *nc2= na2;
}

void matrixSubtractVectorND(double* a, int na1, int na2,
                            double* b, int nb1, int nb2,
                            double** c, int* nc1, int* nc2)
{
    double* tmp= dnalloc(na1*na2);
    int i, j, k;
    if ( nb2 == 1 )
    {
        for( i= 0; i < na2; ++i )
            for ( j= 0; j < na1; ++j )
            {
                tmp[j*na2 + i]= a[j*na2 + i] - b[j];
            }
        *c= tmp;
        *nc1= na1;
        *nc2= na2;
    }
    else
    {
        for ( i= 0; i < na1; ++i )
            for ( j= 0; j < na2; ++j )
            {
                tmp[i*na2 + j]= a[i*na2 + j] - b[j];
            }
        *c= tmp;
        *nc1= na1;
        *nc2= na2;
    }
}

void matrixMultiplicationByVector(double* a, int na1, int na2,
                                  double* b, int nb,
                                  double** c, int* nc)
{
    double* tmp= dnalloc(na1);
    int i, j, k;
    for ( i= 0; i < na1; ++i )
    {
        tmp[i]= 0;
        for ( j= 0; j < na2; ++j )
            tmp[i]+= a[i*na2 + j] * b[j];
    }
    *c= tmp;
    *nc= na1;
}

void vectorMultiplicationByMatrix(double* a, int na,
                                  double* b, int nb1, int nb2,
                                  double**c, int* nc)
{
    double* tmp= dnalloc(nb2);
    int i, j, k;
    for ( i= 0; i < nb2; ++i )
    {
        tmp[i]= 0;
        for ( j= 0; j < nb1; ++j )
            tmp[i]+= b[j*nb2 + i] * a[j];
    }
    *c= tmp;
    *nc= nb2;
}
