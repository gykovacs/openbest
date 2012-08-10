#ifndef _EQUATION_SOLVERS_H_
#define _EQUATION_SOLVERS_H_


void solveLinEq(double* A, int rows, int columns, double* b, double* x);

void solveLinEqHD(double* A, int rows, int columns, double* b, int b_columns, double* x);

void solveLinEqSquare(double* A, int rows, int columns, double* b, double* x);

void solveLinEqNonSquare(double* A, int rows, int columns, double* b, double* x);

void solveLinEqSquare2(double* a, int rows, int columns, double* b, int bb_rows, int bb_columns);

void solveLinEqNonSquareLAPACK(double* a, int rows, int columns, double* b, int bb_rows, int bb_columns);

void invertMatrixFloatN(float* a, int rows, int columns, float** b);

void invertMatrixFloat(float* a, int rows, int columns, float* b);

void invertMatrixFloatP(float* a, int rows, int columns);

#endif
