#ifndef _EQUATION_SOLVERS_H_
#define _EQUATION_SOLVERS_H_

/**
  * solves linear equation system
  * @param A array of the system
  * @param rows rows of A
  * @param columns columns of A
  * @param b b vector
  * @param x pointer to preallocated space for the result
  */
void solveLinEq(double* A, int rows, int columns, double* b, double* x);

/**
  * solves high dimensional linear equation system
  * @param A array of the system
  * @param rows rows of A
  * @param columns columns of A
  * @param array of b vectors
  * @param number of b columns
  * @param x pointer to preallocated space for the result
  */
void solveLinEqHD(double* A, int rows, int columns, double* b, int b_columns, double* x);

/**
  * solves high dimensional linear equation system of the form Ax=b
  * @param A array of the system
  * @param rows rows of A
  * @param columns columns of A
  * @param array of b vectors
  * @param number of b columns
  * @param x pointer to preallocated space for the result
  */
void solveLinEqHDAxb(double* A, int rows, int columns, double* b, int b_columns, double* x);

/**
  * solves high dimensional linear equation system of the form xA=b
  * @param A array of the system
  * @param rows rows of A
  * @param columns columns of A
  * @param array of b vectors
  * @param number of b columns
  * @param x pointer to preallocated space for the result
  */
void solveLinEqHDxAb(double* A, int rows, int columns, double* b, int b_columns, double* x);

/**
  * solves square linear equation system
  * @param A array of the system
  * @param rows rows of A
  * @param columns columns of A
  * @param b b vector
  * @param x pointer to preallocated space for the result
  */
void solveLinEqSquare(double* A, int rows, int columns, double* b, double* x);

/**
  * solves non-square linear equation system
  * @param A array of the system
  * @param rows rows of A
  * @param columns columns of A
  * @param b b vector
  * @param x pointer to preallocated space for the result
  */
void solveLinEqNonSquare(double* A, int rows, int columns, double* b, double* x);

/**
  * solves linear equation system in place
  * @param a matrix of the system
  * @param rows rows of a
  * @param columns columns of a
  * @param b array of b vectors
  * @param bb_rows rows of b
  * @param bb_columns columns of b
  */
void solveLinEqSquare2(double* a, int rows, int columns, double* b, int bb_rows, int bb_columns);

/**
  * solves linear equation system in place, using LAPACK
  * @param a matrix of the system
  * @param rows rows of a
  * @param columns columns of a
  * @param b array of b vectors
  * @param bb_rows rows of b
  * @param bb_columns columns of b
  */
void solveLinEqNonSquareLAPACK(double* a, int rows, int columns, double* b, int bb_rows, int bb_columns);

/**
  * inverts float matrix into a new matrix
  * @param a matrix to invert
  * @param rows rows of a
  * @param columns columns of a
  * @param b pointer to a float* variable for output
  */
void invertMatrixFloatN(float* a, int rows, int columns, float** b);

/**
  * inverts a float matrix into a preallocated space
  * @param a matrix to invert
  * @param rows rows of a
  * @param columns columns of a
  * @param b pointer of preallocated area
  */
void invertMatrixFloat(float* a, int rows, int columns, float* b);

/**
  * inverts a matrix in place
  * @param a matrix to invert
  * @param rows rows of a
  * @param columns columns of a
  */
void invertMatrixFloatP(float* a, int rows, int columns);

/**
  * estimates the condition number of a double matrix
  * @param a matrix
  * @param rows rows of a
  * @param columns columns of a
  * @returns estimated condition number
  */
double conditionNumberEstimateD(double* a, int rows, int columns);

#endif
