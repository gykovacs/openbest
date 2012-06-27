#ifndef _MATRIX3_H_
#define _MATRIX3_H_

#include <stdio.h>

#include <openbest-ds/vector3.h>

typedef struct
{
  float t[9];
} matrix3;

matrix3 createMatrix3(float a, float b, float c, float d, float e, float f, float g, float h, float i);

void setMatrix3(matrix3* m, float a, float b, float c, float d, float e, float f, float g, float h, float i);

vector3 vpm(vector3 v, matrix3 m);

vector3 mpv(matrix3 m, vector3 v);

matrix3 mpm(matrix3 a, matrix3 b);

float vpv_i(vector3 a, vector3 b);

matrix3 vpv(vector3 a, vector3 b);

void displayM(matrix3* m);

void displayV(vector3* v);

#endif