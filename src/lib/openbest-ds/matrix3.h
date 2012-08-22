#ifndef _MATRIX3_H_
#define _MATRIX3_H_

#include <stdio.h>

#include "openbest-ds/vector3.h"

/**
 * struct to represent a 3x3 matrix3
 */ 
typedef struct
{
  real t[9];
} matrix3;

/**
 * initializes a matrix3
 * @param a (0,0)
 * @param b (0,1)
 * @param c (0,2)
 * @param d (1,0)
 * @param e (1,1)
 * @param f (1,2)
 * @param g (2,0)
 * @param h (2,1)
 * @param i (2,2)
 * @returns initialized matrix3
 */ 
matrix3 createMatrix3(real a, real b, real c, real d, real e, real f, real g, real h, real i);

/**
 * sets the fields of the parameter matrix3 instance
 * @param m pointer to matrix3 instance
 * @param a (0,0)
 * @param b (0,1)
 * @param c (0,2)
 * @param d (1,0)
 * @param e (1,1)
 * @param f (1,2)
 * @param g (2,0)
 * @param h (2,1)
 * @param i (2,2)
 */ 
void setMatrix3(matrix3* m, real a, real b, real c, real d, real e, real f, real g, real h, real i);

/**
 * vector x matrix product
 * @param v vector to multiply
 * @param m matrix to multiply
 * @returns the row-column composition product
 */ 
vector3 vpm(vector3 v, matrix3 m);

/**
 * matrix x vector product
 * @param m matrix to multiply
 * @param v vector to multiply
 * @returns the row-column composition product
 */ 
vector3 mpv(matrix3 m, vector3 v);

/**
 * matrix x matrix product
 * @param a matrix to multiply (left)
 * @param b matrix to multiply (right)
 * @returns the row-column composition product
 */ 
matrix3 mpm(matrix3 a, matrix3 b);

/**
 * inner product of vectors
 * @param a vector a
 * @param b vector b
 * @returns (a,b)
 */ 
real vpv_i(vector3 a, vector3 b);

/**
 * row-column composition product of vectors
 * @param a vector a
 * @param b vector b
 * @returns product matrix (ab^T)
 */ 
matrix3 vpv(vector3 a, vector3 b);

/**
 * prints the elements of a matrix to stdout
 * @param m pointer to matrix to display
 */ 
void displayM(matrix3* m);

/**
 * prints the elements of a vector to stdout
 * @param v pointer to vector to display
 */ 
void displayV(vector3* v);

/**
  * initializes the matrix of rotation around the x-axis with angle a
  * @param a angle of rotation
  * @returns the rotation matrix
  */
matrix3 initRotX(real a);

/**
  * initializes the matrix of rotation around the y-axis with angle a
  * @param a angle of rotation
  * @returns the rotation matrix
  */
matrix3 initRotY(real a);

/**
  * initializes the matrix of rotation around the z-axis with angle a
  * @param a angle of rotation
  * @returns the rotation matrix
  */
matrix3 initRotZ(real a);

#endif
