#ifndef _EQUATION_SOLVERS_H_
#define _EQUATION_SOLVERS_H_

void solveLinEq(double* A, int rows, int columns, double* b, double* x);

void solveLinEqSquare(double* A, int rows, int columns, double* b, double* x);

void solveLinEqNonSquare(double* A, int rows, int columns, double* b, double* x);

#endif
