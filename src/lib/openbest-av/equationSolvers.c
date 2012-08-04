#include <gsl/gsl_linalg.h>

#include "openbest-av/equationSolvers.h"

void solveLinEq(double* A, int rows, int columns, double* b, double* x)
{
    if ( rows == columns )
        solveLinEqSquare(A, rows, columns, b, x);
    else
        solveLinEqNonSquare(A, rows, columns, b, x);
}

void solveLinEqSquare(double* A, int rows, int columns, double* bb, double* xx)
{
    gsl_matrix_view m= gsl_matrix_view_array(A, rows, columns);
    gsl_vector_view b= gsl_vector_view_array(bb, rows);
    gsl_vector_view x= gsl_vector_view_array(xx, columns);

    gsl_permutation* p= gsl_permutation_alloc(columns);

    int s;

    gsl_linalg_LU_decomp(&m.matrix, p, &s);
    gsl_linalg_LU_solve(&m.matrix, p, &b.vector, &x.vector);

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
