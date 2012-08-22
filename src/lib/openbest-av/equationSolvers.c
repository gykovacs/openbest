#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <gsl/gsl_linalg.h>

#include "openbest-ds/memFunctions.h"
#include "openbest-av/equationSolvers.h"
#include "openbest-ds/printOut.h"
#include "openbest-ds/basicAlgorithms.h"

#include <openbest-av/lapacke/lapacke.h>

/* DGELS prototype */
//extern void dgels( char* trans, int* m, int* n, int* nrhs, double* a, int* lda,
//                double* b, int* ldb, double* work, int* lwork, int* info );

void solveLinEqNonSquareLAPACK(double* a, int rows, int columns, double* b, int bb_rows, int bb_columns)
{
    int info, m, n, lda, ldb, nrhs;

    m= rows;
    n= columns;
    nrhs= bb_columns;
    lda= columns;
    ldb= bb_columns;

    double wkopt;
    double work[10000];
    int lwork= 10000;
    int i;
    info= LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', m, n, nrhs, a, lda, b, ldb);
}

void solveLinEq(double* A, int rows, int columns, double* b, double* x)
{
    if ( rows == columns )
    {
        memcpy(x, b, sizeof(double)*rows);
        solveLinEqSquare2(A, rows, columns, x, rows, 1);
    }
    else
        solveLinEqNonSquare(A, rows, columns, b, x);
}

void solveLinEqHD(double* A, int rows, int columns, double* b, int b_columns, double* x)
{
    double* bb= dnalloc(rows * b_columns);
    memcpy(bb, b, sizeof(double)*rows * b_columns);
    solveLinEqNonSquareLAPACK(A, rows, columns, bb, rows, b_columns);
    memcpy(x, bb, sizeof(double)*b_columns * columns);
    free(bb);
}

void solveLinEqHDAxb(double* A, int rows, int columns, double* b, int b_columns, double* x)
{
    double* bb= dnalloc(rows * b_columns);
    memcpy(bb, b, sizeof(double)*rows * b_columns);
    solveLinEqNonSquareLAPACK(A, rows, columns, bb, rows, b_columns);
    memcpy(x, bb, sizeof(double)*b_columns * columns);
    free(bb);
}

void solveLinEqHDxAb(double* A, int rows, int columns, double* b, int b_columns, double* x)
{
    double* bb= dnalloc(rows * b_columns);
    memcpy(bb, b, sizeof(double)*rows * b_columns);

    double* AT= transposeMatrixND(A, rows, columns);
    double* bT= transposeMatrixND(b, rows, b_columns);

    solveLinEqNonSquareLAPACK(AT, columns, rows, bb, b_columns, rows);

    double* xT= transposeMatrixND(bb, b_columns, rows);

    memcpy(x, xT, sizeof(double)*b_columns * columns);
    free(xT);
    free(bT);
    free(AT);
    free(bb);
}

void solveLinEqSquare(double* A, int rows, int columns, double* bb, double* xx)
{
    int i;
    for ( i= 0; i < rows*columns; ++i )
        if ( A[i] != A[i] )
            printf("%d ", i);

    gsl_matrix_view m= gsl_matrix_view_array(A, rows, columns);
    gsl_vector_view b= gsl_vector_view_array(bb, rows);
    gsl_vector_view x= gsl_vector_view_array(xx, columns);

    gsl_matrix_fprintf(stdout, &m.matrix, "%g ");

    gsl_permutation* p= gsl_permutation_alloc(columns);

    int s;

    int e= gsl_linalg_LU_decomp(&m.matrix, p, &s);
    printf("s: %d\n", s);
    printf("gsl_linalg_LU_decomp: %d\n", e);
    gsl_matrix_fprintf(stdout, &m.matrix, "%g ");

    double det= gsl_linalg_LU_det(&m.matrix, s);
    printf("determinant: %f\n", det);

    //getchar();
    int err= gsl_linalg_LU_solve(&m.matrix, p, &b.vector, &x.vector);

    //gsl_vector_fprintf(stdout, &x.vector, "%g ");
    printf("gsl_linalg_LU_solve: %d\n", err);

    gsl_permutation_free(p);
}

void solveLinEqNonSquare(double* A, int rows, int columns, double* bb, double* xx)
{
    gsl_matrix_view m= gsl_matrix_view_array(A, rows, columns);
    gsl_vector_view b= gsl_vector_view_array(bb, rows);
    gsl_vector_view x= gsl_vector_view_array(xx, columns);

    gsl_vector *tau= gsl_vector_alloc(rows < columns ? rows : columns);
    gsl_vector *residual= gsl_vector_alloc(rows);

    gsl_linalg_QR_decomp(&m.matrix, tau);
    gsl_linalg_QR_lssolve(&m.matrix, tau, &b.vector, &x.vector, residual);

    gsl_vector_free(tau);
    gsl_vector_free(residual);
}

void solveLinEqSquare2(double* a, int rows, int columns, double* b, int bb_rows, int bb_columns)
{
    int i, icol, irow, j, k, l, ll, n= rows, m= bb_columns;

    double big, dum, pivinv, tmp;
    int* indxc= inalloc(n);
    int* indxr= inalloc(n);
    int* ipiv= inalloc(n);

    for ( j= 0; j < n; ++j )
        ipiv[j]= 0;
    for ( i= 0; i < n; ++i )
    {
        big= 0.0;
        for ( j= 0; j < n; ++j )
            if ( ipiv[j] != 1 )
                for ( k= 0; k < n; ++k )
                {
                    if ( ipiv[k] == 0 )
                    {
                        if ( fabs(a[j*columns + k]) >= big )
                        {
                            big= fabs(a[j*columns + k]);
                            irow= j;
                            icol= k;
                        }
                    }
                }
        ++(ipiv[icol]);
        if ( irow != icol )
        {
            for ( l= 0; l < n; ++l )
            {
                tmp= a[irow * columns + l];
                a[irow * columns + l]= a[icol*columns + l];
                a[icol * columns + l]= tmp;
            }
            for ( l= 0; l < m; ++l )
            {
                tmp= b[irow * bb_columns + l];
                b[irow * bb_columns + l]= b[icol*bb_columns + l];
                b[icol * bb_columns + l]= tmp;
            }
        }
        indxr[i]= irow;
        indxc[i]= icol;
        /*if ( a[icol*columns + icol] == 0.0 )
            eprintf("Singular Matrix\n");*/
        pivinv= 1.0/a[icol*columns + icol];
        a[icol*columns + icol]= 1.0;
        for ( l= 0; l < n; ++l )
            a[icol*columns + l]*= pivinv;
        for ( l= 0; l < m; ++l )
            b[icol*bb_columns + l]*= pivinv;

        for ( ll= 0; ll < n; ++ll )
        {
            if ( ll != icol )
            {
                dum= a[ll*columns + icol];
                a[ll*columns + icol]= 0.0;
                for ( l= 0; l < n; ++l )
                    a[ll*columns + l]-= a[icol*columns + l]*dum;
                for ( l= 0; l < m; ++l )
                    b[ll*bb_columns + l]-= b[icol*bb_columns + l]*dum;
            }
        }
    }
    for ( l= n-1; l>= 0; --l )
    {
        if ( indxr[l] != indxc[l] )
            for ( k= 0; k < n; ++k )
            {
                tmp= a[k*columns + indxr[l]];
                a[k*columns + indxr[l]]= a[k*columns + indxc[l]];
                a[k*columns + indxc[l]]= tmp;
            }
    }
}

void invertMatrixFloatN(float* a, int rows, int columns, float** b)
{
    *b= (float*)malloc(sizeof(float)*(rows*columns));

    invertMatrixFloat(a, rows, columns, *b);
}

void invertMatrixFloat(float* a, int rows, int columns, float* b)
{
    double* A= (double*)malloc(sizeof(double)*(rows*columns));
    int i, j, k;
    for ( i= 0; i < rows * columns; ++i )
        A[i]= a[i];

    double* B= (double*)malloc(sizeof(double)*(rows*columns));

    gsl_matrix_view m= gsl_matrix_view_array(A, rows, columns);
    gsl_matrix_view n= gsl_matrix_view_array(B, rows, columns);

    gsl_permutation* p= gsl_permutation_alloc(columns);

    int s;

    int e= gsl_linalg_LU_decomp(&m.matrix, p, &s);
    int err= gsl_linalg_LU_invert(&m.matrix, p, &n.matrix);

    for ( i= 0; i < rows*columns; ++i )
        (b)[i]= B[i];

    gsl_permutation_free(p);

    free(B);
    free(A);
}

void invertMatrixFloatP(float* a, int rows, int columns)
{

}

double conditionNumberEstimateD(double* a, int rows, int columns)
{
    int k;
    for ( k= 0; k < rows*columns; ++k )
        if ( a[k] > 10000 || a[k] < -10000 )
            printf("error: %f ", a[k]);
    int n_ipiv= rows < columns ? rows : columns;
    int* ipiv= inalloc(n_ipiv);

    double* atmp= dnalloc(rows*columns);
    memcpy(atmp, a, sizeof(double)*rows*columns);

    double anorm= fabs(atmp[0]);
    int i;
    for ( i= 0; i < rows*columns; ++i )
        if ( anorm < fabs(atmp[i]) )
            anorm= fabs(atmp[i]);

    int err= LAPACKE_dgetrf(LAPACK_ROW_MAJOR, rows, columns, atmp, rows, ipiv);
    if ( err )
    {
        free(ipiv);
        free(atmp);
        return FLT_MAX;
    }

    double rcond;
    err= LAPACKE_dgecon(LAPACK_ROW_MAJOR, '1', columns, atmp, rows, anorm, &rcond);
    if ( err )
    {
        free(ipiv);
        free(atmp);
        return FLT_MAX;
    }

    free(ipiv);
    free(atmp);
    return rcond;
}

