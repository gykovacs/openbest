#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "config-ds.h"

/**
 * struct to represent 3D vectors
 */
typedef struct
{
  real x;
  real y;
  real z;
} vector3;

/**
 * initialize vector3 with the parameters
 * @param x x coordinate
 * @param y y coordinate
 * @param z z coordinate
 * @returns initialized vector3
 */ 
vector3 createVector3(real x, real y, real z);

/**
 * set the fields of a vector3 instance
 * @param v vector3 instance to set
 * @param x x coordinate
 * @param y y coordinate
 * @param z z coordinate
 */ 
void setVector3(vector3* v, real x, real y, real z);

#endif